# Windows: el micrófono entrega silencio digital durante toda la sesión

**Estado:** diagnosticado desde logs, sin causa raíz confirmada. Falta un
experimento en una máquina Windows.
**Plataforma:** solo Windows. macOS no lo tiene.
**Versión observada:** 1.3.3 · log del 2026-09-22
**Impacto:** el lado del agente (`/cue/stream/user`) no se transcribe **nunca**.
Medio producto muerto en Windows. Reportado por varios usuarios.

Este documento está escrito para una sesión que arranca **sin contexto previo**.
Todo lo afirmado abajo está verificado contra el código y contra un log real de
producción; lo que es hipótesis está marcado como tal.

---

## 1. El síntoma

En la ventana del coach aparece el banner rojo:

> Sayso can't hear your microphone — it's sending silence. Check that your input
> device isn't muted.

El usuario que lo reportó creía dos cosas, y **las dos son falsas**:

1. *"Se dispara solo si no hablo ~58 segundos y luego suena algo en la PC."*
   La duración del silencio y el sonido de la PC no lo **causan**. El micrófono
   está muerto desde el primer buffer. El sonido de la PC solo **desbloquea** la
   alerta (ver §3).
2. *"Después se va solo porque detecta el micrófono perfectamente."*
   No lo detecta nunca. `Microphone signal restored` aparece **cero** veces en
   todo el log. El banner desaparece porque el usuario lo cierra o porque la
   sesión se reinicia.

---

## 2. Lo que el log prueba

Log local de referencia: `~/Desktop/sayso-2026-09-22 (2).log` (en la máquina del
autor). En Windows los logs viven en
`C:\Users\<usuario>\AppData\Roaming\sayso-app\logs\`.

Las 4 alertas del log son idénticas en lo esencial:

```json
[WARN] [Cue] Microphone is delivering buffers but no signal while the call has audio {
  "silentForMs": 20015,
  "rmsDb": -120,
  "gainDb": 0,
  "peakDb": -120,
  "prospectRmsDb": -4.5,
  "sampleRate": 48000,
  "channels": 2,
  "bitDepth": 32
}
```

### 2.1 `rmsDb: -120` y `peakDb: -120` significan ceros absolutos

`electron/audio/audioManager.ts:257`:

```ts
function toDb(linear: number): number {
  return linear > 0 ? 20 * Math.log10(linear) : -120;
}
```

−120 es el valor centinela que devuelve **solo** cuando el lineal es exactamente
`0`. Que el **peak** también sea −120 significa que *todas y cada una* de las
muestras del bloque son cero.

Esto **no** es un cuarto silencioso. El comentario de SAYSO-428 en
`audioManager.ts:67-72` documenta que un cuarto callado con AirPods mide entre
−70 y −75 dBFS. Esto es silencio digital puro: buffers de ceros.

### 2.2 Es desde el primer buffer, no una degradación

| `First mic buffer` | Alerta | Δ |
|---|---|---|
| 21:13:08.232 | 21:13:28.250 | 20.018 s |
| 21:13:45.499 | 21:14:05.507 | 20.008 s |
| 21:15:13.353 | 21:15:33.356 | 20.003 s |
| 21:15:36.811 | 21:15:56.812 | 20.001 s |

`CUE_MIC_SILENT_ALERT_MS = 20000` (`audioManager.ts:79`). La alerta salta a los
20 s exactos del primer buffer, **en las cuatro sesiones**. El micrófono entrega
ceros desde el paquete número uno y no para.

### 2.3 Nunca se recupera, y el lado del prospect está sano

| Comprobación | Resultado |
|---|---|
| `Microphone signal restored` | **0 ocurrencias** |
| `WebSocketClient:prospect` | 32 mensajes, con transcripciones reales |
| `WebSocketClient:user` | 4 mensajes, **todos** `"type": "connected"`. Cero transcripciones. |
| `prospectRmsDb` | −4.5 dB — el audio del sistema funciona perfecto |

Formato reportado: mic `48000Hz/2ch/32bit float`, prospect `48000Hz/2ch/32bit`.

---

## 3. Por qué el banner NO es un falso positivo

La lógica está en `trackCueUserLevel`, `audioManager.ts:283-340`. Dos condiciones
deben cumplirse para alertar (`audioManager.ts:307-308`):

```ts
const callHasAudio = cueMicSilence.lastProspectSpeechMs >= cueMicSilence.silentSinceMs;
if (silentForMs < CUE_MIC_SILENT_ALERT_MS || !callHasAudio) return;
```

1. El mic lleva ≥ 20 s por debajo de `CUE_MIC_SILENCE_FLOOR_DB = -80`
   (`audioManager.ts:73`), **y**
2. el prospect habló durante ese silencio.

La condición (2) existe para no gritar cuando simplemente no hay llamada. Por eso
"poner un sonido en la PC" parece disparar el bug: satisface (2). El micrófono
ya estaba en cero.

**El watchdog está diciendo la verdad.** La alerta es correcta; lo roto es la
captura.

---

## 4. Por qué macOS no lo tiene

macOS y Windows son implementaciones **completamente independientes** detrás del
mismo contrato de 13 métodos (`docs/NATIVE_AUDIO_CONTRACT.md`):

| | macOS | Windows |
|---|---|---|
| Archivo | `src/audio_device_manager.mm` | `src/audio_device_manager_win.cpp` |
| Mic | AVAudioEngine | WASAPI shared mode |
| AGC | **Sí** (`.mm:1957`, `.mm:2153` reporta `gainDb`) | **No existe** |
| Downmix a mono | Sí (`.mm:2128`) | No — entrega 2 canales |

En el log, `gainDb: 0` siempre, consistente con que el `.cpp` de Windows no tiene
AGC ni reporta el campo. Eso **no es el bug** (el watchdog resta `gainDb`, así
que ambas plataformas comparan el nivel pre-ganancia), pero explica por qué las
dos rutas se comportan distinto y por qué el umbral de −80 dBFS se calibró
midiendo en macOS.

---

## 5. Hipótesis (ninguna confirmada)

### H1 — Privacidad de micrófono de Windows *(la más probable)*

Windows **no falla** las llamadas de WASAPI cuando el acceso al micrófono está
denegado: inicializa bien, entrega paquetes, y **todas las muestras son cero**.
Es exactamente la firma observada.

Agravante: `electron/permissions/WindowsPermissionsProvider.ts` **falla abierto**
en las líneas 30 y 34 —

```ts
console.warn(`[Permissions] Mic access status is '${status}' — treating as granted`);
console.warn('[Permissions] Could not read mic access status; treating as granted:', e);
```

— cualquier estado que no sepa interpretar se trata como concedido. En el log de
producción **no hay ni una línea de `[Permissions]`**, así que no sabemos qué
devolvió.

Comprobación barata: Configuración → Privacidad → Micrófono. Son **dos** switches
(el global y "permitir que las aplicaciones de escritorio accedan al micrófono").

### H2 — `AUDCLNT_BUFFERFLAGS_SILENT` puesto en cada paquete

`audio_device_manager_win.cpp:717-721` rellena con ceros cuando Windows marca ese
flag:

```cpp
if (flags & AUDCLNT_BUFFERFLAGS_SILENT) {
  std::fill(payload->audioData.begin(), payload->audioData.end(), 0);
} else {
  memcpy(payload->audioData.data(), data, payload->audioData.size());
}
```

Eso es lo correcto según MSDN. Pero si Windows lo está marcando siempre, el
resultado es idéntico a lo observado y la causa está río arriba.

### H3 — Endpoint equivocado

`win.cpp:762` y `win.cpp:989` usan:

```cpp
g_enumerator->GetDefaultAudioEndpoint(eCapture, eConsole, &device);
```

Windows distingue el rol **eConsole** del rol **eCommunications**, y con auriculares
o headsets el micrófono real suele ser el default de *comunicaciones*. Además,
`channels: 2` para un micrófono es inusual (la mayoría son mono) y merece que se
loguee el nombre del endpoint que se abrió.

---

## 6. El experimento que separa las tres

Instrumentar `PushCapturedBuffer` (`win.cpp:713`) para ver los bytes **tal como
los entrega WASAPI**, antes de que nuestro propio código los toque.

### 6.0 Antes de recompilar: la ruta que no toca C++

**Hacer esto primero.** Recompilar el módulo nativo en una máquina con Smart App
Control activo lo deja bloqueado (§7). Cualquier diagnóstico escrito en TS/JS
esquiva ese problema por completo, porque no produce un binario nuevo — solo el
`.node` dispara SAC.

Y la hipótesis más probable (H1) se puede probar sin tocar una línea de C++:

1. **Cero código.** Configuración → Privacidad → Micrófono. Son **dos** switches:
   el global y "permitir que las aplicaciones de escritorio accedan al
   micrófono". Si alguno está apagado, ya está la respuesta.

2. **Una línea de TS.** `WindowsPermissionsProvider.ts:27` ya llama a
   `systemPreferences.getMediaAccessStatus('microphone')`, pero solo loguea en
   los caminos de warn (líneas 30 y 34). En el log de producción **no aparece ni
   una línea de `[Permissions]`**, así que no sabemos qué devuelve. Loguearlo
   siempre, no solo cuando falla. Después `npm run build:electron` — sin rebuild
   nativo, sin SAC.

   Importa porque el provider **falla abierto**: cualquier estado que no sepa
   interpretar lo trata como concedido.

3. **Cero código.** Cambiar el dispositivo de entrada por defecto, o enchufar y
   desenchufar un headset. Si con otro dispositivo sí hay señal, apunta al
   endpoint (`eConsole` vs `eCommunications`, H3).

Lo único que JS **no** puede contestar es si `AUDCLNT_BUFFERFLAGS_SILENT` está
puesto, porque el nativo rellena con ceros y no reporta el flag. Para eso sí hace
falta el rebuild y el bloque de §6.2. Pero si los pasos 1 o 2 dan la respuesta,
se ahorra la pelea con SAC entera.

### 6.1 Dónde va

Al principio del cuerpo de `PushCapturedBuffer`, **antes** del `resize` y del
`std::fill`. Si se pone después, siempre se leerán ceros — porque los pusimos
nosotros.

### 6.2 Versión mínima

Asume float32, que es lo que el log reporta para el mic.

```cpp
  // ── DIAG TEMPORAL — quitar antes de cualquier commit. ──
  if (channel == &g_micChannel) {
    static std::atomic<uint64_t> nextTick{0};
    const bool silentFlag = (flags & AUDCLNT_BUFFERFLAGS_SILENT) != 0;
    int64_t peakPpm = 0;
    if (!silentFlag && data) {
      for (size_t i = 0, n = (size_t)framesAvailable * blockAlign; i + 4 <= n; i += 4) {
        float v; memcpy(&v, data + i, 4);
        int64_t a = (int64_t)((v < 0 ? -v : v) * 1000000.0f);
        if (a > peakPpm) peakPpm = a;
      }
    }
    uint64_t now = GetTickCount64(), due = nextTick.load(std::memory_order_relaxed);
    if (now >= due && nextTick.compare_exchange_strong(due, now + 1000, std::memory_order_relaxed)) {
      EmitLifecycleEvent("mic_diag silent_flag=" + std::to_string(silentFlag ? 1 : 0) +
                         " peak_ppm=" + std::to_string(peakPpm) +
                         " ch=" + std::to_string(channels) + " float=" + (isFloat ? "1" : "0"));
    }
  }
```

### 6.3 Versión completa

Cubre los dos paths (para comparar el mic contra el system audio, que sí
funciona), los tres formatos, y cuenta paquetes por ventana para distinguir "el
flag aparece a veces" de "siempre".

```cpp
  // ── DIAG TEMPORAL — quitar antes de cualquier commit. ──
  // Antes del relleno de ceros, para ver los bytes tal como los dio WASAPI.
  // No lee `data` si el flag SILENT está puesto: MSDN dice que en ese caso el
  // contenido del buffer es indefinido. Una línea por segundo y por path.
  {
    const int slot = (channel == &g_micChannel) ? 0 : 1;
    static std::atomic<uint64_t> diagWindowEnd[2];
    static std::atomic<uint64_t> diagPackets[2];
    static std::atomic<uint64_t> diagSilentFlagged[2];
    static std::atomic<int64_t> diagPeakPpm[2];   // pico * 1e6, entero para el atomic

    int64_t peakPpm = 0;
    if (!(flags & AUDCLNT_BUFFERFLAGS_SILENT) && data != nullptr) {
      const size_t bytes = static_cast<size_t>(framesAvailable) * blockAlign;
      double peak = 0.0;
      if (bitDepth == 32 && isFloat) {
        for (size_t i = 0; i + 4 <= bytes; i += 4) {
          float v; memcpy(&v, data + i, sizeof(v));
          const double a = v < 0 ? -(double)v : (double)v;
          if (a > peak) peak = a;
        }
      } else if (bitDepth == 32) {
        for (size_t i = 0; i + 4 <= bytes; i += 4) {
          int32_t v; memcpy(&v, data + i, sizeof(v));
          const double a = (v < 0 ? -(double)v : (double)v) / 2147483648.0;
          if (a > peak) peak = a;
        }
      } else if (bitDepth == 16) {
        for (size_t i = 0; i + 2 <= bytes; i += 2) {
          int16_t v; memcpy(&v, data + i, sizeof(v));
          const double a = (v < 0 ? -(double)v : (double)v) / 32768.0;
          if (a > peak) peak = a;
        }
      }
      peakPpm = static_cast<int64_t>(peak * 1000000.0);
    } else {
      diagSilentFlagged[slot].fetch_add(1, std::memory_order_relaxed);
    }
    diagPackets[slot].fetch_add(1, std::memory_order_relaxed);
    for (int64_t cur = diagPeakPpm[slot].load(std::memory_order_relaxed);
         peakPpm > cur && !diagPeakPpm[slot].compare_exchange_weak(cur, peakPpm, std::memory_order_relaxed); ) {}

    const uint64_t nowTick = GetTickCount64();
    uint64_t due = diagWindowEnd[slot].load(std::memory_order_relaxed);
    if (nowTick >= due &&
        diagWindowEnd[slot].compare_exchange_strong(due, nowTick + 1000, std::memory_order_relaxed)) {
      EmitLifecycleEvent(
          std::string(channel->name) + "_diag" +
          " packets=" + std::to_string(diagPackets[slot].exchange(0, std::memory_order_relaxed)) +
          " silent_flag=" + std::to_string(diagSilentFlagged[slot].exchange(0, std::memory_order_relaxed)) +
          " peak_ppm=" + std::to_string(diagPeakPpm[slot].exchange(0, std::memory_order_relaxed)) +
          " rate=" + std::to_string(static_cast<int>(sampleRate)) +
          " ch=" + std::to_string(channels) +
          " bits=" + std::to_string(bitDepth) +
          " float=" + (isFloat ? "1" : "0"));
    }
  }
```

> Ambos bloques pasaron `clang++ -std=c++20 -fsyntax-only -Wall -Wextra` con
> stubs de los tipos de Windows, en macOS. **No se pudieron compilar de verdad**
> (no hay SDK de Windows en esa máquina). Si `node-gyp` se queja, el error es
> nuevo y hay que leerlo, no asumir que el bloque es correcto.

`peak_ppm` es el pico × 1.000.000 como entero: `0` = ceros absolutos,
`500000` = pico 0.5. Se usa entero porque no hay `std::atomic<float>`.

### 6.4 Compilar y leer la salida

```bash
npm run rebuild-native-win
```

> ⚠️ En una máquina con Smart App Control activo, este rebuild produce un `.node`
> sin firma ni reputación y **SAC lo bloquea** al arrancar la app. Ya pasó una
> vez. Leer §7 **antes** de correr esto — apagar SAC es irreversible sin
> reinstalar Windows.

`EmitLifecycleEvent` (`win.cpp:158-159`) llama a `OutputDebugStringA`
**primero e incondicionalmente**, antes de mirar si el canal hacia JS está
conectado:

```cpp
void EmitLifecycleEvent(const std::string& message) {
  OutputDebugStringA(("[native_audio] " + message + "\n").c_str());
```

Así que **DebugView** (Sysinternals, portable, no requiere instalación) muestra
estos eventos aunque el puente a JS esté roto — y en el log de producción no
aparece ni un lifecycle event, así que conviene asumir que lo está. Filtrar por
`[native_audio]`.

### 6.5 Tabla de decisión

| Salida | Significado | Siguiente paso |
|---|---|---|
| `silent_flag` ≈ `packets` | Windows dice "esto es silencio". Nuestro código hace lo correcto. | H1 o H3. Revisar permisos y el endpoint. |
| `silent_flag=0`, `peak_ppm=0` | Windows dice que hay audio y son ceros. Firma clásica de privacidad. | **H1.** Los dos switches de Configuración. |
| `silent_flag=0`, `peak_ppm>0` | **El módulo nativo está bien.** El bug está río abajo. | No tocar C++. Empezar por `electron/streaming/audioBuffer.js`. |

La tercera fila es la que ahorra una semana. Por eso este experimento va **antes**
de leer WASAPI.

---

## 7. Smart App Control bloquea el `.node` recompilado localmente

**Observado en una máquina de desarrollo Windows:** tras aplicar el bloque de
§6.2 y correr `npm run rebuild-native-win`, Smart App Control bloqueó el módulo
de audio al arrancar la app en local. El experimento no se pudo correr.

**Por qué pasa:** SAC evalúa **reputación**, no solo firma. Un `.node` recién
compilado en local no tiene ninguna de las dos: ni firma Authenticode ni
historial. Es exactamente el perfil que SAC está diseñado para bloquear.

**Por qué las builds de staging de GitHub Releases nunca lo dispararon:** llegan
dentro de un instalador firmado por un publisher conocido y descargado por el
canal normal, y eso le presta reputación a lo que instala. No es que el `.node`
publicado esté firmado — no lo está (§8) — es que el contexto es distinto.

> ⚠️ **Smart App Control, una vez apagado, no se puede volver a encender sin
> reinstalar Windows.** Es comportamiento documentado de Microsoft. No admite
> exclusiones por archivo: es todo o nada. Conviene agotar la ruta de §6.0 antes
> de considerarlo, o correr el experimento en una máquina o VM sin SAC.

**Esto no es la causa del bug de producción.** El log de producción lo descarta:

```
🎤 [AUDIO MANAGER] Native module loaded successfully
[Cue] First mic buffer: 48000Hz/2ch/32bit float
```

El módulo cargó y entregó buffers. Si SAC lo hubiera bloqueado allá, el `require`
fallaría y no habría ni un buffer — `audioManager.ts` carga el provider de forma
lazy precisamente para que un `.node` ausente no tumbe el arranque. El síntoma
sería un fallo de carga, no buffers sanos llenos de ceros.

---

## 8. Hueco aparte: el `.node` se publica sin firmar en Windows

Descubierto mientras se investigaba §7. **No es la causa de este bug**, pero es
un problema real del pipeline y merece su propio ticket.

| Evidencia | Qué dice |
|---|---|
| `package.json` → `build.win.sign` = `./scripts/sign-windows.js` | No hay `signExts` ni `signDlls`. electron-builder por defecto firma `.exe` y `.dll`; **`.node` no está en esa lista.** |
| `build.asarUnpack` = `["electron/native-audio/**"]` | El `.node` queda suelto en disco, fuera del asar — firmable, pero nadie lo firma. |
| `scripts/sign-native-module.js` | Usa `codesign` y una ruta de bundle `.app`: es **solo macOS**. No existe equivalente de Windows. |
| `.github/workflows/release-windows.yml:458+` | El paso "Verify the signatures" solo comprueba el instalador, el exe de la app y opcionalmente `elevate.exe`. **`native_audio.node` no se verifica.** |

Es decir: no solo va sin firmar, sino que el propio verify —que existe
precisamente para que no se escape una build sin firmar— no lo mira. Por eso
nadie lo notó.

Consecuencias plausibles, sin confirmar: usuarios con SAC activo o con políticas
de WDAC podrían ver el módulo bloqueado en producción, lo que se vería como la
app arrancando pero sin audio en absoluto. **Ese sería un síntoma distinto al de
este documento** (ahí no habría buffers; acá los hay, llenos de ceros), pero vale
la pena tenerlo en el radar por si aparecen reportes que no encajan.

---

## 9. Restricciones

- **El diagnóstico es temporal y local.** `electron/native-audio/` lo mantiene
  otro equipo. Este bloque es para reproducir en local, no para commitear.
  Revertirlo antes de abrir cualquier PR.
- **Después de tocar `electron/*.ts` hay que correr `npm run build:electron`** —
  Electron carga el `.js` compilado, no el `.ts`.
- **No hay tests.** `npm test` es un stub que sale con 1. TypeScript es el único
  chequeo estático.
- `tsc --noEmit` **no está en cero**: hay un error conocido preexistente en
  `AudioSettings.tsx:85`. Comparar contra esa línea base.
- No meter `.ts` nuevos en `electron/streaming/` — ver el `CLAUDE.md` de la raíz,
  esa carpeta no tiene regla de ignore y su `.js` es escrito a mano.

---

## 10. Preguntas abiertas

1. ¿`silent_flag` está puesto o no? Es la pregunta que parte el problema en tres.
2. ¿Qué endpoint se abre realmente? Loguear el friendly name del `IMMDevice` en
   `win.cpp:762`. ¿Por qué `channels: 2` en un micrófono?
3. ¿Qué devuelve `systemPreferences.getMediaAccessStatus('microphone')` en una
   máquina afectada? El fail-open de `WindowsPermissionsProvider.ts:30,34` lo
   está ocultando. Vale la pena loguearlo siempre, no solo en el camino de warn.
4. ¿Por qué el canal de lifecycle events no aparece en los logs de producción?
   ¿Está cableado `setLifecycleEventCallback` desde JS? Es el instrumento
   principal del módulo y está mudo.
5. ¿Cuántos usuarios? `audioManager.ts:333` manda
   `[Cue] Microphone delivering silence mid-session` a Sentry una vez por sesión.
   Eso da el conteo exacto y desde qué versión empezó.
6. ¿Reproduce en una máquina Windows sana, o solo en las de los usuarios
   afectados? Si reproduce en cualquiera, H1 baja de probabilidad.
7. ¿Se puede correr el experimento de §6.2 sin apagar SAC — en una VM, en otra
   máquina, o firmando el `.node` local con un certificado en el que la máquina
   ya confíe?
8. ¿Debería `build.win.signExts` incluir `.node`, o hace falta un hook propio
   como el de macOS? ¿Rompe algo firmar un binario que va dentro de
   `app.asar.unpacked`? (Ticket aparte — ver §8.)
