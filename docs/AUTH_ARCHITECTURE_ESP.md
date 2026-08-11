# Arquitectura de Autenticación

> **Tipo de documento:** Postmortem híbrido + referencia arquitectural.
> **Audiencia:** Cualquier persona que necesite entender cómo Sayso autentica usuarios, por qué el sistema de auth fue rediseñado, y cómo modificarlo de forma segura en el futuro.
> **Última reescritura mayor:** Mayo 2026 (este documento cubre esa reescritura de punta a punta).

---

## Tabla de Contenidos

1. [TL;DR](#tldr)
2. [El Problema](#el-problema)
3. [Contexto Histórico: El Fix Parcial Anterior (Abril 2026)](#contexto-histórico-el-fix-parcial-anterior-abril-2026)
4. [Análisis de Causa Raíz](#análisis-de-causa-raíz)
5. [El Fix: Inversión Arquitectural](#el-fix-inversión-arquitectural)
6. [Referencia de Componentes](#referencia-de-componentes)
7. [Superficie IPC](#superficie-ipc)
8. [Flujos de Auth (Detallados)](#flujos-de-auth-detallados)
9. [Referencia de Archivos](#referencia-de-archivos)
10. [Gotchas y Lecciones Aprendidas](#gotchas-y-lecciones-aprendidas)
11. [Qué Vigilar](#qué-vigilar)

---

## TL;DR

La auth de Sayso solía tener **dos sistemas compitiendo por la propiedad de la sesión del usuario**: el Supabase JS SDK en cada renderer (leyendo/escribiendo `localStorage` vía `cryptoStorage`) y el proceso main de Electron (guardando tokens en variables `global`). Cuando un renderer se montaba o recargaba, empujaba un token viejo del `localStorage` sobre el token fresco que main acababa de refrescar. La protección "detect compromised refresh tokens" de Supabase entonces revocaba la sesión y el usuario era desloggeado — más reproducible después de estar AFK 10–15 minutos durante una sesión de coach.

Un fix **parcial** en Abril 2026 (commit `c1fdef5`) centralizó el *acto* de refrescar, pero dejó la propiedad dividida. La reescritura de Mayo 2026 **invierte la arquitectura**: el proceso main es ahora el único dueño de la sesión a través de una nueva clase `AuthManager`, y los renderers son consumidores de solo lectura vía IPC. El Supabase JS SDK ya no participa en auth para nada.

---

## El Problema

### Síntomas

- Los usuarios eran desloggeados de forma inesperada, frecuentemente después de un período de inactividad (~10–15 minutos era la ventana más reproducible).
- El reproductor más concreto: **iniciar una sesión de coach, irse AFK ~10–15 min, volver e intentar parar/resetear → error de "token expirado" y re-login forzado**.
- Sentry: issue **SAYSO-APP-46**, `Token refresh failed: 400`.

### Impacto en el usuario

El producto es una herramienta de coaching de ventas en tiempo real. Los desloggeos a mitad de llamada son catastróficos — matan el audio stream, dropean el thread de AI, y obligan al rep a re-autenticarse mientras un prospect está en la línea.

### Lo que queríamos en cambio

Se buscaba persistencia de sesión al nivel de **Slack/Discord**: loggearse una vez, nunca volver a loggearse salvo que se cierre sesión explícitamente. Las configuraciones del proyecto Supabase fueron verificadas para soportar esto (Time-box = never, Inactivity timeout = never), así que las sesiones permanentes eran alcanzables — la falla estaba del lado del cliente.

---

## Contexto Histórico: El Fix Parcial Anterior (Abril 2026)

Este contexto es importante porque el mismo problema había sido *parcialmente* atendido antes.

**Commit:** `c1fdef5` — `fix(auth): centralize token refresh in main process to prevent multi-window race condition`

### Qué hizo ese fix

Agregó un handler `refresh-auth-tokens` en main con una cola:

```ts
let isRefreshingTokens = false;
let pendingRefreshResolvers = [];

ipcMain.handle('refresh-auth-tokens', async () => {
  if (isRefreshingTokens) {
    return new Promise(resolve => pendingRefreshResolvers.push(resolve));
  }
  isRefreshingTokens = true;
  try {
    const tokens = await refreshAuthTokens(global.authRefreshToken);
    // broadcast a todas las ventanas...
  } finally {
    isRefreshingTokens = false;
    pendingRefreshResolvers = [];
  }
});
```

Esto resolvía el caso donde múltiples ventanas intentaban refrescar **al mismo tiempo**: solo una llamada de red iría a Supabase; las otras esperaban la misma Promise en vuelo.

### Por qué fue insuficiente

Centralizó la *operación* de refresh, pero **no la propiedad de los tokens**. La arquitectura después de ese fix:

```
Renderer (Supabase SDK + localStorage)  ←→  Main (global.authAccessToken / global.authRefreshToken)
         ↑                                        ↑
    FUENTE DE VERDAD                          COPIAS CACHEADAS
```

- El Supabase JS SDK en cada renderer seguía siendo dueño de la sesión — persistida en `localStorage` vía el adaptador `cryptoStorage`.
- Main solo guardaba *copias* de los tokens que el renderer le empujaba vía `update-auth-tokens`.
- Cada vez que el `AuthContext` de un renderer se inicializaba, leía `localStorage` (que podía tener tokens viejos) y los empujaba a main, sobreescribiendo lo que main tuviera.

Entonces, aunque dos refreshes **simultáneos** ya no podían ocurrir, dos fuentes de verdad **secuenciales** sí podían — y lo hacían, cada vez que una ventana se montaba o recargaba.

El fix de Abril atacó un síntoma. La reescritura de Mayo ataca la arquitectura.

---

## Análisis de Causa Raíz

### Los actores

- **Proceso main** — tiene `global.authAccessToken`, `global.authRefreshToken`. No persiste nada propio (pre-reescritura).
- **Renderer del splash window** — ejecuta `AuthProvider` de `AuthContext.tsx`. Tiene una instancia del Supabase JS SDK con `cryptoStorage` persistiendo en `localStorage`.
- **Renderer del coach window** — `BrowserWindow` separado, React root separado, instancia de Supabase JS SDK separada, pero **comparte `localStorage`** con el splash window (mismo origen).
- **Renderer del tray menu** — separado también, también comparte `localStorage`.
- **Backend de Supabase Auth** — la fuente de verdad sobre la validez de los refresh tokens, con estas características relevantes:
  - **Rotación de refresh tokens**: cada llamada de refresh invalida el refresh token anterior y emite uno nuevo.
  - **Intervalo de reuso de 10 segundos**: un refresh token puede usarse dos veces dentro de 10s sin activar la protección (para que un retry breve no mate la sesión).
  - **Detect compromised refresh tokens**: si el *mismo* refresh token se usa **dos veces después del intervalo de reuso**, Supabase asume que fue filtrado y revoca toda la sesión.

### El bug, paso a paso

```
T=0s    Main tiene el refresh token R0. El usuario se loggea.
        El Supabase SDK del splash renderer persiste R0 en localStorage (vía cryptoStorage).
        El splash AuthContext envía R0 a main vía `update-auth-tokens`.
        global.authRefreshToken = R0. ✅ Alineados.

T=3590s Se aproxima la expiración del token. Algún 401 dispara (o una ventana hace
        algo que triggerea un refresh).
        Main llama a Supabase /token?grant_type=refresh_token con R0.
        Supabase devuelve access token A1 y refresh token R1, invalida R0.
        global.authRefreshToken = R1.
        Main hace broadcast de `auth-tokens-refreshed` { R1 } a todas las ventanas.

        PERO — localStorage todavía tiene R0 si el splash window no procesó el
        broadcast aún, o si cryptoStorage no se actualizó aún, o si una
        ventana diferente está en medio de montarse.

T=3591s El splash window navega / re-monta el AuthProvider por cualquier motivo
        (cambio de ruta, hot reload, evento INITIAL_SESSION del Supabase SDK).
        AuthProvider lee localStorage → todavía ve R0.
        AuthProvider llama `update-auth-tokens` con R0.
        global.authRefreshToken = R0 (sobreescribiendo R1!)  ❌ Desync.

T=3601s+ Alguna llamada de axios necesita un token. Main triggerea refresh con R0.
        Supabase: "Emití R1 hace 11 segundos y ahora me mandás R0 de nuevo
        — eso está fuera de la ventana de reuso de 10s. Esto es un reuso de token comprometido."
        → Revoca toda la sesión.
        → Retorna 400 invalid_grant.
        → El usuario es desloggeado a mitad de llamada.
```

### Por qué el reproductor "AFK por 10 minutos" funciona

El camino 401 → refresh era el trigger más común, y el lifetime del JWT era ~1 hora. Pero el driver real era el patrón **window-mount-reads-stale-localStorage**. Cualquier cosa que hiciera que un renderer re-inicializara su `AuthProvider` podía re-introducir el refresh token viejo. Irse AFK y volver ejercía esto porque el SO suspendía los timers, el WebSocket droppea, volver triggereaba intentos de reconexión, esos triggereaban lecturas de token, y el desync salía a la luz.

### Factores contribuyentes (también arreglados)

- **`autoRefreshToken: false` en el cliente Supabase** — significaba que el SDK nunca refrescaba proactivamente. Los refreshes solo ocurrían de forma reactiva ante un 401, aumentando la ventana donde tokens viejos vivían.
- **`updateToken()` del WebSocket nunca era llamado** — la clase `AudioStreamer` tenía un método para cambiar tokens en un WS en vivo, pero main nunca lo llamaba al refrescar. Así que las reconexiones del WS podían dispararse con tokens viejos.
- **El evento `INITIAL_SESSION` en Supabase JS v2 no se manejaba** — solo se manejaba `SIGNED_IN`. Esto es parte del camino de boot silencioso que re-introducía tokens viejos.

---

## El Fix: Inversión Arquitectural

```
ANTES                                            DESPUÉS
─────────────────────────────────────────       ─────────────────────────────────────────
Renderer = fuente de verdad                      Main = fuente de verdad
    (Supabase SDK + localStorage)                    (clase AuthManager)
                ↓                                              ↓
Main = copia cacheada                            Renderer = consumidor de solo lectura
    (variables global)                               (pregunta vía IPC, nunca escribe)
```

El renderer ya no puede empujar tokens a main — ese canal fue eliminado.
El Supabase JS SDK en los renderers ya no está involucrado en auth para nada (las operaciones de storage que antes agarraban `session.access_token` del SDK ahora le preguntan a main vía IPC).

### Los cuatro pilares

1. **`AuthManager`** — una sola clase basada en `EventEmitter` en main que posee todo el estado de auth, lógica de refresh, MFA, y persistencia. Vive en `electron/auth/AuthManager.ts`.
2. **Refresh proactivo** — `AuthManager` programa un `setTimeout` para disparar **60 segundos antes** de que el JWT expire. Los usuarios casi nunca ven un 401 en operación normal.
3. **Promise única en vuelo para refresh** — `AuthManager` usa un campo `refreshPromise: Promise<void> | null` como mutex. Los callers concurrentes todos hacen `await` de la misma Promise. No se necesita cola; el lenguaje lo da gratis.
4. **IPC como única superficie de auth** — cinco handlers (`auth:sign-in`, `auth:get-token`, `auth:get-state`, `auth:sign-out`, `auth:verify-mfa`) son la forma en que los renderers interactúan con auth. Nada más.

---

## Referencia de Componentes

### `electron/auth/AuthManager.ts`

La única fuente de verdad para auth en el proceso main. ~350 líneas.

**Estado (campos privados):**
- `accessToken: string | null` — el JWT actual.
- `refreshToken: string | null` — el refresh token actual (también persistido en disco).
- `expiresAt: number | null` — timestamp Unix, decodificado del claim `exp` del JWT.
- `user: AuthUser | null` — `{ id, email }` decodificado del JWT.
- `refreshTimer: NodeJS.Timeout | null` — timer de refresh proactivo.
- `refreshPromise: Promise<void> | null` — mutex para callers de refresh concurrentes.

**API pública:**
- `init(): Promise<void>` — llamar una vez al inicio de la app. Carga el refresh token encriptado del disco, lo intercambia por un access token fresco, programa el próximo refresh proactivo. Silencioso: si no hay token guardado o el intercambio falla, el usuario queda desloggeado.
- `signIn(email, password): Promise<SignInResult>` — hace POST a `/auth/v1/token?grant_type=password`. Verifica factores TOTP verificados vía `/auth/v1/user`. Retorna `{ success: true, mfaRequired: true, mfaFactors }` si MFA está enrollado y el token está en AAL1, de lo contrario `{ success: true }`.
- `verifyMFA(factorId, code): Promise<{ success, error? }>` — challenge → verify contra Supabase. En caso de éxito, reemplaza el par de tokens AAL1 por el nuevo par AAL2 y persiste el nuevo refresh token.
- `signOut(): Promise<void>` — POST fire-and-forget a `/auth/v1/logout`, luego limpia el estado en memoria y el token en disco. El sign-out local no espera la llamada de red.
- `getAccessToken(): Promise<string | null>` — el entry point IPC para renderers. Refresca si el token está dentro de los 60s de expirar, de lo contrario retorna el token cacheado. Todos los callers concurrentes comparten un único refresh en vuelo.
- `getState(): AuthState` — snapshot sincrónico de `{ user, isAuthenticated, accessToken }`.

**Eventos:**
- `signed-in`, `signed-out`, `token-refreshed`, `session-expired` — main los escucha y hace broadcast a todas las ventanas + actualiza variables global + reapunta los WebSockets.

**Persistencia:**
- El refresh token es lo único que se persiste, y pasa por `safeStorage` (keychain del SO — Keychain en macOS, DPAPI en Windows, libsecret en Linux). Implementado en `electron/utils/tokenStore.ts`. En macOS el ítem del keychain es **`Sayso Safe Storage`**, nombrado a partir del nombre de la app en producción definido en `main.ts` (SAYSO-272).
- `loadRefreshToken()` se auto-repara: si el blob en disco existe pero no se puede desencriptar — ej. la clave de `safeStorage` cambió tras el renombrado del keychain o una migración de firma/certificado — descarta el archivo viejo con `clearRefreshToken()` y devuelve `null`, así el usuario simplemente vuelve a iniciar sesión en vez de quedar bloqueado (sin crash, sin diálogo del keychain). Un archivo inexistente (`ENOENT`) es el caso normal de "no autenticado" y se deja intacto.
- El access token es solo en memoria — expira de todas formas, no tiene sentido persistirlo.

### `electron/main.ts` — Wiring

Main crea el singleton `AuthManager` al cargarse el módulo (línea ~31) y:

1. **Puentes de eventos a broadcasts** — cada evento de `AuthManager` se convierte en mensajes IPC a todas las ventanas Y actualiza el `global.authAccessToken` legado (todavía leído por código no-auth) Y actualiza los tokens de los streamers WebSocket Y, en `session-expired`, detiene los loops de reconexión del WS.

2. **Llama `authManager.init()` cuando la app está lista** — reemplaza el viejo bloque manual de silent-auth. Si `getState().isAuthenticated`, el splash window se saltea y el tray menu se construye inmediatamente. De lo contrario el splash se abre para sign-in.

3. **Expone la superficie IPC** — cinco registraciones `ipcMain.handle()` que simplemente delegan a métodos de `authManager`.

### `src/context/AuthContext.tsx`

Consumidor puro. **No** importa el Supabase JS SDK para nada (excepto el *tipo* `Factor`).

- **Bootstrap (`useEffect` al montarse):** llama al IPC `auth:get-state`. Si está autenticado, setea estado local de React. Setea `loading=false`.
- **Escucha broadcasts IPC `auth:state`** — cuando main dice que el auth cambió, AuthContext actualiza el estado de React.
- **Escucha `auth:session-expired`** vía `useSessionExpiry` — cierra ventanas secundarias de forma elegante.
- **`signIn`** — llama a `auth:sign-in`, maneja la rama MFA-required seteando el estado `mfaRequired`.
- **`verifyMFA`** — llama a `auth:verify-mfa`.
- **`handleSignOut`** — llama a `auth:sign-out`.

El `AuthContext` expone `mfaRequired` y `currentAAL` como estado de React para que componentes de UI como `AuthGuard` y `useLoginForm` puedan reaccionar sin hacer sus propias llamadas de red.

### `src/config/axios.ts`

Consumidor puro. Ya **no** mantiene su propia cola de refresh (main tiene el mutex).

- **Request interceptor:** `await ipcRenderer.invoke('auth:get-token')` y adjuntar como `Bearer`.
- **Response interceptor en 401:** llamar `auth:get-token` de nuevo (el mutex de main serializa cualquier refresh en vuelo), reintentar el request original una vez. Si main retorna null, despachar un DOM event `auth:session-expired` para que `useSessionExpiry` pueda reaccionar.
- **Lógica de retry de red** (3 intentos con backoff para `ERR_NETWORK` / `ERR_EMPTY_RESPONSE`) fue preservada.

### `electron/streaming/audioStreamer.ts` (y `cueAudioStreamer`)

Ya tenía un método `updateToken(newToken)` pero **nunca era llamado pre-reescritura**. Main ahora lo llama en cada evento `token-refreshed` de `AuthManager`, de modo que las reconexiones de WebSocket en vivo siempre llevan un token actual.

---

## Superficie IPC

Estos cinco handlers son la **única** forma en que los renderers interactúan con auth.

| Canal IPC          | Dirección          | Args                              | Retorna                                     | Propósito                                             |
| ------------------ | ------------------ | --------------------------------- | ------------------------------------------- | ----------------------------------------------------- |
| `auth:sign-in`     | renderer → main    | `{ email, password }`             | `{ success, mfaRequired?, mfaFactors?, error? }` | Sign-in con password. Retorna mfaRequired:true si TOTP está enrollado. |
| `auth:verify-mfa`  | renderer → main    | `{ factorId, code }`              | `{ success, error? }`                       | Elevar AAL1 → AAL2 con código TOTP.                   |
| `auth:sign-out`    | renderer → main    | —                                 | `void`                                      | Sign-out local + logout al servidor best-effort.      |
| `auth:get-token`   | renderer → main    | —                                 | `string \| null`                            | JWT fresco. Refresca si está dentro de 60s de expirar. Con mutex. |
| `auth:get-state`   | renderer → main    | —                                 | `{ user, isAuthenticated, accessToken }`    | Lectura sincrónica del estado de auth.                |
| `auth:state`       | main → todas las ventanas | `{ user, isAuthenticated }` | (broadcast)                                 | Auth cambió (sign-in / sign-out / token refresh).     |
| `auth:token-refreshed` | main → todas las ventanas | —                          | (broadcast)                                 | Token fue refrescado — útil si alguna UI lo necesita. |
| `auth:session-expired` | main → todas las ventanas | —                          | (broadcast)                                 | La sesión se fue. Los renderers deben cerrarse.       |

También hay dos aliases de compatibilidad hacia atrás (`auth-tokens-refreshed`, `auth-session-expired`) que todavía se disparan para que cualquier listener legado siga funcionando durante la transición.

---

## Flujos de Auth (Detallados)

### Sign-in sin MFA

```
Usuario              Renderer                       Main (AuthManager)            Supabase
 │                     │                                  │                          │
 │  ingresa creds      │                                  │                          │
 ├────────────────────►│                                  │                          │
 │                     │  IPC: auth:sign-in {email,pwd}   │                          │
 │                     ├─────────────────────────────────►│                          │
 │                     │                                  │  POST /auth/v1/token     │
 │                     │                                  ├─────────────────────────►│
 │                     │                                  │◄─────────────── A0 / R0 ─┤
 │                     │                                  │  GET /auth/v1/user       │
 │                     │                                  ├─────────────────────────►│
 │                     │                                  │◄────────── factors: [] ──┤
 │                     │                                  │  saveRefreshToken(R0)    │
 │                     │                                  │  scheduleRefresh(exp-60) │
 │                     │  { success: true }               │                          │
 │                     │◄─────────────────────────────────┤                          │
 │                     │  navegar al dashboard            │                          │
 │◄────────────────────┤                                  │                          │
```

### Sign-in con MFA enrollado

```
Usuario              Renderer                       Main (AuthManager)            Supabase
 │  ingresa creds      │                                  │                          │
 ├────────────────────►│  IPC: auth:sign-in               │                          │
 │                     ├─────────────────────────────────►│                          │
 │                     │                                  │  POST /auth/v1/token     │
 │                     │                                  ├─────────────────────────►│
 │                     │                                  │◄────── token AAL1 A0/R0 ─┤
 │                     │                                  │  GET /auth/v1/user       │
 │                     │                                  ├─────────────────────────►│
 │                     │                                  │◄──── factors: [TOTP] ────┤
 │                     │  { mfaRequired: true,            │                          │
 │                     │    mfaFactors: [TOTP] }          │                          │
 │                     │◄─────────────────────────────────┤                          │
 │                     │  ⚠️ R0 NO se persiste todavía    │                          │
 │                     │  navegar a /mfa-verify           │                          │
 │  ingresa código TOTP│                                  │                          │
 ├────────────────────►│  IPC: auth:verify-mfa            │                          │
 │                     ├─────────────────────────────────►│                          │
 │                     │                                  │  POST /factors/:id/      │
 │                     │                                  │       challenge          │
 │                     │                                  ├─────────────────────────►│
 │                     │                                  │  POST /factors/:id/      │
 │                     │                                  │       verify             │
 │                     │                                  ├─────────────────────────►│
 │                     │                                  │◄────── token AAL2 A1/R1 ─┤
 │                     │                                  │  saveRefreshToken(R1)    │
 │                     │                                  │  scheduleRefresh(exp-60) │
 │                     │  { success: true }               │                          │
 │                     │◄─────────────────────────────────┤                          │
 │                     │  navegar al dashboard            │                          │
```

Nota: el token AAL1 del paso de password se guarda **solo en memoria** hasta que MFA se completa. Si el usuario abandona el flujo MFA, no se persiste ningún refresh token — entonces tendrá que ingresar su password de nuevo la próxima vez.

### Refresh proactivo

El método `_scheduleRefresh()` calcula `delayMs = (expiresAt - now - 60) * 1000` y setea un `setTimeout`. El timer tiene `unref()` para que nunca bloquee el quit de la app.

```
T=now+0s     Sign-in. El token expira en T+3600s.
             scheduleRefresh setea timer para T+3540s.
T=now+3540s  El timer dispara. Llama a _refresh().
             _refresh no ve ninguna refreshPromise en vuelo, empieza una.
             POST /token?grant_type=refresh_token (R0)
             Recibe A1, R1.
             saveRefreshToken(R1). emit 'token-refreshed'.
             scheduleRefresh setea próximo timer para T+7080s.
T=now+3600s  (El token viejo habría expirado acá — pero ya fue reemplazado.)
```

El usuario nunca ve un 401 por expiración de token en operación normal.

### Refresh reactivo ante un 401

Si algo con race condition ocurre (clock skew, falla de red justo en la expiración, etc.) y un HTTP request retorna 401:

```
request de axios → 401
response interceptor de axios:
  marca el request _retried = true
  invoke('auth:get-token')
    └─ AuthManager.getAccessToken()
         needsRefresh = true (expiresAt - now < 60)
         if (refreshPromise) await it
         else crear uno y ejecutar _doRefresh()
  si main retorna null → despachar DOM event 'auth:session-expired'
  sino → setear header Authorization → reintentar una vez → retornar response
```

Los 401 concurrentes de múltiples ventanas todos golpean la misma `refreshPromise` — main hace una sola llamada de red, todos los callers obtienen el mismo token fresco.

### Restauración de sesión al iniciar la app

```
app.whenReady()
  ├─ loadEnvironmentVariables()  ← crítico: debe correr antes de AuthManager.init()
  └─ await authManager.init()
        ├─ loadRefreshToken()  → desencripta blob del disco vía safeStorage (lo borra si no se puede desencriptar)
        ├─ si no hay token → return (el usuario se loggea)
        ├─ _exchangeRefreshToken(stored)
        │     POST /token?grant_type=refresh_token
        │     recibe A1, R1 → actualiza estado en memoria
        ├─ saveRefreshToken(R1)  ← Supabase rotó; persistir el nuevo
        └─ scheduleRefresh()

  si authManager.getState().isAuthenticated:
      construir tray menu, saltear splash
  sino:
      abrir splash window para sign-in
```

Si `_exchangeRefreshToken` lanza un error (ej. el usuario cerró sesión desde otro dispositivo, o pasaron 90 días sin uso, o el token fue revocado), `init()` llama `clearRefreshToken()` y al usuario se le muestra la pantalla de login. Silencioso, no fatal.

### Sign-out

```
Renderer:  IPC auth:sign-out
Main (AuthManager.signOut):
  si accessToken: POST fire-and-forget a /auth/v1/logout (no await)
  _clearSession()  → nullear todos los tokens en memoria, cancelar timer de refresh
  clearRefreshToken()  → eliminar archivo encriptado
  emit 'signed-out'
Wiring de main en 'signed-out':
  global.authAccessToken = null
  broadcast auth:state {user:null, isAuthenticated:false} a todas las ventanas
  detener streamers WebSocket (shouldReconnect=false, luego stop())
```

### Expiración de sesión

Se dispara cuando un refresh falla (Supabase retornó 400 — refresh token revocado, expirado, o replay comprometido).

```
AuthManager._doRefresh captura el error
  → Sentry.captureException
  → _handleExpired()
       _clearSession()
       clearRefreshToken()
       emit 'session-expired'
Wiring de main en 'session-expired':
  global.authAccessToken = null; global.authRefreshToken = null
  broadcast auth:state {user:null, isAuthenticated:false}
  broadcast auth:session-expired
  broadcast auth-session-expired (alias legado)
  detener streamers WebSocket
```

`useSessionExpiry` en los renderers recibe el DOM event (axios hace de puente entre el evento IPC y un `window.dispatchEvent`) y cierra ventanas secundarias de forma elegante. El `AuthContext` del splash window llama `resetUser()` que rutea al login.

### Ciclo de vida de auth del WebSocket

Los WebSockets se autentican vía un query param `?token=<jwt>` al conectarse — Supabase / el servidor de Sayso verifica el JWT en el query string.

```
La sesión de coach empieza
  → cueAudioStreamer.start(sessionId, token)  ← token de auth:get-token
  → WS se abre con ?token=<jwt>

T=connect+3540s
  → AuthManager dispara refresh proactivo
  → emite 'token-refreshed'
  → listener de main llama cueAudioStreamer.updateToken(newToken)
       (esto actualiza el campo token interno del streamer; en la próxima reconexión,
        el WS usará el token fresco; algunas implementaciones envían un mensaje
        auth-update in-band para evitar reconectarse)

WS droppea a mitad de sesión
  → lógica de reconexión se activa
  → usa streamer.token (ahora fresco)
  → se reconecta exitosamente

La sesión expira
  → AuthManager emite 'session-expired'
  → listener de main: streamer.shouldReconnect = false; streamer.stop()
       (previene loops de reconexión con un token muerto)
```

Pre-reescritura, `updateToken` existía pero nunca era llamado, así que las reconexiones después de un refresh usaban el token original (viejo) de cuando el WS se abrió por primera vez. Esto fue un contribuyente mayor al reproductor "irse AFK y romper todo".

---

## Referencia de Archivos

### Creados en Mayo 2026

| Archivo | Propósito |
|---|---|
| `electron/auth/AuthManager.ts` | La clase única fuente de verdad. Posee todos los tokens, maneja sign-in / refresh / MFA / sign-out. |
| `client/docs/AUTH_ARCHITECTURE.md` | El documento original en inglés. |
| `client/docs/AUTH_ARCHITECTURE_ESP.md` | Este documento. |

### Significativamente reescritos en Mayo 2026

| Archivo | Qué cambió |
|---|---|
| `electron/main.ts` | Instancia `AuthManager`, conecta sus eventos a broadcasts IPC + actualizaciones de WS + sync de variables global, reemplaza el viejo bloque de silent-auth con `authManager.init()`, reemplaza los viejos handlers IPC con la superficie `auth:*`. |
| `src/context/AuthContext.tsx` | Sin más Supabase SDK. Bootstrapea desde `auth:get-state`. Escucha broadcast `auth:state`. signIn/verifyMFA/signOut todos pasan por IPC. |
| `src/config/axios.ts` | Sin más cola de refresh (main tiene el mutex). El request interceptor llama `auth:get-token`. El handler de 401 reintenta vía el mismo IPC. |

### Migrados en Mayo 2026 (edits pequeños — swap de fuente de token)

Estos archivos solían llamar `supabase.auth.getSession()` para extraer un JWT; ahora llaman al IPC `auth:get-token`.

> Registro histórico. Algunas filas de abajo (`useCue.tsx`, `AuthGuard.tsx`)
> nombran archivos que después fueron eliminados como código muerto en SAYSO-354.

| Archivo | Qué hace |
|---|---|
| `src/coachWindow/services/cueService.ts` | Inicia la sesión de streaming WS de cue; necesita un JWT para auth en la URL del WS. |
| `src/coachWindow/services/audioUploadService.ts` | Sube archivos de audio grabados vía IPC a main; pasa el JWT. |
| `src/coachWindow/hooks/useAudioUpload.tsx` | Igual que el anterior pero como React hook. |
| `src/coachWindow/hooks/useCue.tsx` | Igual que `cueService.ts` pero como hook. |
| `src/trayMenu/TrayMenuApp.tsx` | Construye una URL autenticada cuando el usuario hace click en "My Account" — abre la web app en `app.asksayso.com/settings#access_token=...`. |
| `src/splashWindow/LogoutGate.tsx` | Dispara sign-out cuando el splash se reabre con `?logout=true`; ahora pasa por IPC. |
| `src/components/AuthGuard.tsx` | Solía llamar `getAAL()` (que necesitaba sesión del Supabase SDK); ahora lee `mfaRequired` de `AuthContext`. |
| `src/splashWindow/routes/Login/hooks/useLoginForm.tsx` | Solía llamar `getAAL()` post-signIn; ahora infiere si se necesita MFA del valor de retorno de `signIn()` (`user: null` ⇒ MFA pendiente). |

### Eliminados en Mayo 2026

En `electron/main.ts`:
- El handler IPC `update-auth-tokens`. **Este era el bug** — el canal que permitía a los renderers empujar tokens viejos a main.
- El handler IPC `get-auth-tokens`.
- El handler IPC `refresh-auth-tokens` con su cola `isRefreshingTokens` / `pendingRefreshResolvers` (el fix parcial de Abril 2026).
- La importación `const { refreshAuthTokens } = require('./utils/authTokens')`.

### Eliminados en la limpieza SAYSO-354 (Agosto 2026)

Los restos del Supabase JS SDK que figuraban acá como "muertos pero no eliminados" ya no están:
`src/config/supabase.ts`, `src/utils/tokenEncryption.ts`, `src/utils/supabaseClient.ts`,
`src/utils/debugStorage.ts`, `src/utils/authDebug.ts` y `src/services/mfaServices.ts`.

El SDK ya no se importa para auth en runtime en ningún lado — la única referencia
que queda es un `import type { Factor }` en `src/context/AuthContext.tsx`.

### Tocados pero sin cambios sustanciales

| Archivo | Por qué importa |
|---|---|
| `electron/utils/tokenStore.ts` | Persistencia del refresh token respaldada por `safeStorage`. Correcto tal como está, usado por `AuthManager`. |
| `electron/streaming/*Streamer.ts` | El método `updateToken()` ya existía, ahora realmente es llamado por main. |
| `src/hooks/useSessionExpiry.ts` | Todavía se necesita — escucha el DOM event que `axios.ts` despacha cuando la sesión termina. |

---

## Gotchas y Lecciones Aprendidas

### 1. Race condition entre carga del módulo y carga de env vars

**Síntoma:** el sign-in fallaba con `Failed to parse URL from /auth/v1/token?grant_type=password`.

**Causa:** `AuthManager` se instanciaba al cargarse el módulo (línea 31 de `main.ts`) pero `loadEnvironmentVariables()` corre ~900 líneas después. El constructor cacheaba `process.env.VITE_SUPABASE_URL` (todavía `undefined`) en `this.supabaseUrl`, así que `fetch("" + path)` recibía una URL relativa y el fetch de Node no puede parsearlas.

**Fix:** convertir `supabaseUrl` y `supabaseAnonKey` a **lazy getters** que leen `process.env` al momento de ser llamados. Elimina la dependencia de orden por completo.

**Lección:** cualquier cosa en main que dependa de env vars debe o bien inicializarse después de `loadEnvironmentVariables()` o leer las env vars de forma lazy. No cachear valores derivados de env vars al momento de carga del módulo.

### 2. Auth propiedad del renderer en una app Electron multi-ventana es estructuralmente frágil

`localStorage` se comparte entre ventanas del mismo origen, lo que suena como que debería hacer la auth multi-ventana fácil. Pero lo hace *peor*: cualquier ventana puede escribir un valor viejos en cualquier momento, y no hay buena forma de coordinar "quién tiene el token más fresco ahora mismo" entre procesos renderer. El proceso main es el único lugar donde realmente podés serializar estado.

Si estás construyendo una app Electron multi-ventana, **sé dueño del auth en main desde el día uno**. No intentes compartir stores del lado del renderer.

### 3. `autoRefreshToken: false` sin refresh proactivo en otro lado es una trampa

Setear esto en el Supabase JS SDK es correcto *si y solo si* algo más está haciendo refresh proactivo. De lo contrario convertiste un SDK de "refresh en background" en uno que solo refresca reactivamente ante un 401, que es lo peor de los dos mundos: todavía tenés 401s y no tenés buffer.

### 4. La rotación de refresh tokens requiere secuenciamiento cuidadoso

Supabase rota los refresh tokens en cada uso (con un intervalo de reuso de 10s como red de seguridad). Cualquier camino de código que guarde una copia del viejo refresh token después de un refresh exitoso es una bomba de revocación de sesión esperando explotar. La arquitectura debe garantizar que **solo un componente recuerde el refresh token actual en un momento dado**, y cualquier otro componente que lo necesite le pregunte a ese componente de forma fresca.

### 5. Escuchar `INITIAL_SESSION` si mantenés el Supabase SDK cerca

En Supabase JS v2, el SDK dispara `INITIAL_SESSION` al construirse (independientemente de si existía una sesión). Si solo escuchás `SIGNED_IN`, te perdés el camino de boot silencioso y tu estado de auth se desynceará del SDK. Pre-reescritura, esto contribuyó al bug de splash-remount-pushes-stale-token.

En la nueva arquitectura esto no importa (sin SDK, sin listener), pero si los servicios de enrollment MFA son re-habilitados alguna vez, vigilá esto.

### 6. Los loops de reconexión de WebSocket con un token muerto son letales

Si una sesión expira mientras un WS está abierto, y el WS intenta reconectarse con el token muerto, obtenés un loop infinito de reconexiones rechazadas con 4xx, cada una escribiendo en Sentry. Siempre `shouldReconnect = false` *antes* de `stop()` en session-expired.

### 7. El fix de Abril 2026 nos enseñó una lección real

Ese fix era lógicamente correcto para lo que afirmaba resolver (refreshes concurrentes), e hizo los síntomas menos frecuentes. Pero dado que la arquitectura subyacente no cambió, el bug persistió en una forma ligeramente diferente. **Un patch que no cambia la arquitectura cambia el modo de fallo, no la tasa de fallos.**

Cuando podés identificar una *clase* de bug en lugar de una instancia, arreglá la clase.

---

## Qué Vigilar

### Cuando se cambia código de auth

1. **Nunca re-introducir un canal renderer → main de "push tokens".** El punto entero de la reescritura es que esa dirección no existe.
2. **No agregar llamadas a `setSession()` en ningún lado.** Si un pedazo de código necesita un JWT, le pregunta a main vía `auth:get-token`. No intentar setearlo en el cliente Supabase JS para "hacer que storage funcione" — en cambio, refactorizar el código para que tome un argumento token.
3. **No cachear tokens en estado del renderer por más tiempo que la duración de una sola llamada async.** El estado sobrevive a la intención; un token cacheado en un store de Zustand es un futuro bug de token viejos.
4. **Si agregás una conexión de larga duración (WS, SSE) que usa el JWT, conectala al evento `token-refreshed` en `AuthManager`.** De lo contrario usará silenciosamente un token viejo después del próximo refresh.
5. **Si agregás nuevos métodos a `AuthManager`, mantené el patrón de mutex.** Cualquier operación que toque `accessToken` / `refreshToken` debe pasar por `_refresh()` o ser exclusiva con él.

### Consideraciones del lado servidor (fuera de alcance de esta reescritura)

- El middleware del servidor (`server/middleware/supabaseauth.js`) actualmente llama `supabase.auth.getUser(token)` por request, que es una llamada HTTP a Supabase por request. Para mejor latencia bajo carga, esto podría reemplazarse con verificación local de JWT usando `jose` contra el endpoint JWKS de Supabase. Fue identificado durante la auditoría como una optimización futura pero estaba fuera de alcance para la reescritura de Mayo 2026.
- El camino de auth WebSocket (`server/utils/websocketAuth.js`) tiene la misma propiedad y la misma optimización potencial.

### Enrollment MFA

**No hay enrollment MFA en la app de escritorio.** `mfaServices.ts` — un wrapper
`enroll`/`unenroll` basado en el SDK que ninguna pantalla llamaba — fue eliminado
en SAYSO-354. Los usuarios se enrolan desde la web app; la app de escritorio solo
*verifica* al login, vía `AuthManager.verifyMFA()`
(`/auth/v1/factors/{id}/challenge` + `/verify`), que no se ve afectado.

Si alguna vez se quiere enrollment desde escritorio, construirlo sobre
`AuthManager` con un handler IPC nuevo — no sobre el SDK del renderer, que ya no
tiene sesión y no debe tenerla (ver la regla "nunca pushear tokens desde el
renderer" más arriba).

### Lifetime de sesión

El proyecto Supabase está configurado con **Time-box: never** e **Inactivity timeout: never**. Combinado con `AuthManager.init()` intercambiando el refresh token persistido en cada inicio de la app, las sesiones son ahora efectivamente permanentes — los usuarios quedan loggeados hasta que explícitamente cierren sesión, exactamente como Slack/Discord. Si esas configuraciones de Supabase alguna vez cambian, las sesiones empezarán a expirar de nuevo y `AuthManager.init()` silenciosamente caerá al login screen — lo cual es comportamiento correcto, pero vale la pena saber.

---

*Si este documento está desactualizado, actualizalo. Los futuros ingenieros (y el futuro-vos) le agradecerán al presente-vos.*
