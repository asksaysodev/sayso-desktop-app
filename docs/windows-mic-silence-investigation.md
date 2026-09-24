# Windows: el banner "Sayso can't hear your microphone" es un falso positivo

**Estado (2026-09-23):** resuelto como diagnóstico. El mic **funciona**. Lo que
falla es la **alerta**: el umbral que usa se calibró en macOS y no sirve para
los drivers de Windows, que convierten el silencio en ceros digitales.
**Fix aplicado:** el banner se quitó en Mac y Windows hasta nuevo aviso
([SAYSO-468](https://linear.app/sayso-app/issue/SAYSO-468), commit `7f9001f`,
detalle en §7). La detección sigue corriendo, pero solo loguea. El rediseño va
en [SAYSO-458](https://linear.app/sayso-app/issue/SAYSO-458), que empieza
investigando cómo lo hace Discord.
**Versión observada:** 1.3.3 · logs del 2026-09-22 (usuario) y 2026-09-23
(máquina de desarrollo Windows).
**Pendiente:** confirmar con el usuario que reportó si habló durante sus
sesiones (§6).

Este documento está escrito para una sesión que arranca **sin contexto previo**.
La §1 es el resumen. El resto es el camino que llevó ahí, con las hipótesis
iniciales marcadas como descartadas.

---

## 1. Resumen

- El banner salta cuando el mic lleva **20 s seguidos por debajo de −80 dBFS**
  **mientras el prospect habla** (`trackCueUserLevel`, `audioManager.ts:282`).
- Ese −80 se calibró en macOS, donde un cuarto callado mide entre −70 y −75.
- En Windows, muchos drivers y headsets aplican supresión de ruido o un *gate*,
  y cuando no hablás entregan **silencio digital**:

  | Mic (misma máquina Windows) | Callado | Hablando |
  |---|---|---|
  | Realtek de la laptop ("Varios micrófonos"), dev / staging | −100 dBFS | funciona |
  | Realtek de la laptop, build de testers | **−120 (cero exacto)** | funciona (−27.8) |
  | HyperX Cloud Flight (inalámbrico) | −165 dBFS | funciona (−11.0) |

- Resultado: en Windows el banner aparece **cada vez que el agente escucha 20 s
  en silencio** mientras habla el otro lado. Eso es rutina en una llamada de
  ventas. En cuanto el agente habla, el log muestra `Microphone signal
  restored`. **No se pierde audio.**
- Se descartaron: permisos de Windows, flag `AUDCLNT_BUFFERFLAGS_SILENT`,
  endpoint equivocado, el módulo nativo, el código JS de audio y Smart App
  Control (§4).

---

## 2. Cómo leer los logs

Ubicación en Windows:

| Build | Carpeta |
|---|---|
| Producción / testers (`Sayso`) | `%APPDATA%\sayso-app\logs\` |
| Beta (`Sayso [beta]`) | `%APPDATA%\sayso-app-staging\logs\` |
| Dev (`npm run dev`) | a la terminal; hoy no escribe archivo |

### 2.1 Escala de dBFS

| Valor | Qué es |
|---|---|
| 0 a −10 | Muy fuerte |
| −20 a −40 | Voz normal en el mic |
| −70 a −100 | Cuarto callado (ruido de fondo real) |
| **−120 exacto** | **Cero absoluto.** Es el centinela de `toDb()` (`audioManager.ts:257`) cuando la muestra vale 0 |
| < −120 (p. ej. −165) | Casi cero: el driver/headset silenció la señal |

### 2.2 Las líneas que importan

1. **`[Cue] Session teardown complete — capture stats`**: la más útil, porque
   resume toda la sesión. Solo se escribe si el Cue se para **desde la app**. Si
   se cierra la app o se corta el proceso, esta línea no queda.
   - `userPeakDb`: la muestra más fuerte de la sesión. Si es normal (−11, −30),
     **el mic funcionó**. Si es −120.0, no hubo ni una muestra distinta de cero.
   - `userSilentSeconds`: segundos **acumulados** por debajo de −80. No son
     seguidos.
2. **`[Cue] Microphone is delivering buffers but no signal…`**: el momento en
   que saltó el banner, con `rmsDb` y `peakDb` del bloque. `prospectRmsDb` alto
   solo dice que el otro lado sonaba.
3. **`[Cue] Microphone signal restored`**: el mic volvió a pasar −80. Si aparece,
   el mic funciona.
4. **`[NativeAudio] lifecycle: …`**: eventos del módulo nativo. En una sesión
   normal no aparece ninguno, y eso está bien (§5.3).

Comando rápido:

```powershell
Select-String -Path "C:\ruta\sayso-XXXX.log" -Pattern 'First mic buffer|Microphone is delivering|rmsDb|peakDb|teardown complete|signal restored' | ForEach-Object { "$($_.LineNumber): $($_.Line.Trim())" }
```

---

## 3. Los datos

### 3.1 Log del usuario que reportó (2026-09-22)

Las 4 sesiones, sin un solo valor distinto de −120:

| Sesión | Duración | `userSilentSeconds` | `userPeakDb` | Alerta tras `First mic buffer` |
|---|---|---|---|---|
| 1 | 35.3 s | 35.3 | −120.0 | 20.018 s |
| 2 | 85.4 s | 85.4 | −120.0 | 20.008 s |
| 3 | 21.3 s | 21.3 | −120.0 | 20.003 s |
| 4 | 139.7 s | 139.7 | −120.0 | 20.001 s |

`Microphone signal restored`: **0 ocurrencias**. Formato del mic:
`48000Hz/2ch/32bit float`.

### 3.2 Máquina de desarrollo Windows (2026-09-23)

Hardware: laptop con "Varios micrófonos (Realtek(R) Audio)" (array, por eso
`ch=2`) y, en una prueba, headset HyperX Cloud Flight Wireless. Prueba estándar:
hablar un poco y después quedarse callado 25 s con un video sonando como
"prospect".

| Build | Mic | Alerta: `rmsDb` / `peakDb` | `signal restored` | Teardown `userPeakDb` |
|---|---|---|---|---|
| Dev (`npm run dev`) | Realtek | −100.9 / −94.3 | — (se cortó el proceso) | — |
| Staging 1.3.3 (build local) | Realtek | −100.9 / −91.3 | sí, a los 3 s | −33.5 |
| Testers 1.3.3 (firmada por CI) | Realtek | **−120 / −120** | no se escribió | −27.8 |
| Testers 1.3.3 | HyperX | −165.2 / −156.5 | sí, a los 3 s | −11.0 |

En todos los casos el mic entregó voz cuando se habló.

### 3.3 Qué no sabemos del usuario

Su `userPeakDb=-120.0` en 4.7 minutos admite dos lecturas:

1. **No habló** (probaba poniendo sonido en la PC, como él mismo contó). Entonces
   es el **mismo falso positivo** que en §3.2.
2. **Habló y todo fue cero.** Entonces es un mic muerto de verdad, que es otro
   bug.

Las transcripciones no lo resuelven: con ceros no hay nada que transcribir en
ninguno de los dos casos. Además, el cliente nunca recibe transcripciones del
lado user por WebSocket (§5.4). **Hay que preguntarle** (§6).

---

## 4. Hipótesis iniciales: todas descartadas

Descartadas en la máquina de desarrollo. Las herramientas están en el Apéndice.

| Hipótesis | Prueba | Resultado |
|---|---|---|
| **H1: privacidad de mic de Windows** | Configuración: los tres switches encendidos. Registro `CapabilityAccessManager\ConsentStore\microphone`: `Allow` global y por app (`electron.exe`, `Sayso.exe`) | Descartada |
| **H2: `AUDCLNT_BUFFERFLAGS_SILENT` en cada paquete** | Probe C# con la misma secuencia WASAPI que `win.cpp`, más el bloque diag de §A.3 dentro del `.node` | `silent_flag=0` siempre. Descartada |
| **H3: endpoint equivocado (eConsole vs eCommunications)** | Solo hay un endpoint de captura activo en la máquina | Descartada. `ch=2` se explica porque es un array |
| **Módulo nativo** | El `.node` de dev y el de testers, cargados solos con `ELECTRON_RUN_AS_NODE` | Los dos entregan −74 a −81 dBFS en silencio, sin ceros |
| **Proceso `Sayso.exe`** | El `.node` de testers corriendo dentro del `Sayso.exe` instalado | Sin ceros |
| **JS de audio** | `audioManager.js`, `recorder.js` y `audioBuffer.js` de la build de testers vs staging | Idénticos |
| **Otra app ocupando el mic** | Registro de uso del mic de Windows | Nadie más lo usaba durante la prueba |
| **Smart App Control** | Carga del `.node` en producción: `Native module loaded successfully` y buffers llegando | No bloquea en producción (ver §5.1 para dev) |

**Abierto, sin importancia práctica:** por qué la build de testers dio −120
exacto y staging −100 con el mismo Realtek. Las dos builds solo difieren en
`build_env` y `app_id`, y nada de eso toca el audio. Hipótesis: la supresión de
ruido del driver es adaptativa y no siempre llega a cero. No se repitió la
prueba para comprobarlo.

### 4.1 Por qué macOS no lo tiene (hipótesis, sin verificar)

- Windows aplica por defecto "Mejoras de audio" (supresión de ruido y APOs del
  driver) en los micrófonos, y esos efectos pueden llevar el silencio a cero.
- macOS entrega el audio crudo salvo que se active el procesamiento de voz, y
  `audio_device_manager.mm` no lo activa (usa entrada cruda más su propio AGC,
  `.mm:1893+`).
- Prueba pendiente: desactivar "Mejoras de audio" en el mic desde Configuración
  → Sonido y repetir. Si el silencio pasa de −120/−165 a unos −90, son las
  mejoras de Windows. Probar también el HyperX en Mac: si ahí no da ceros, el
  headset no es el que silencia.

---

## 5. Hallazgos laterales

### 5.1 Smart App Control bloquea un `.node` recompilado en local

Recompilar el nativo en una máquina con SAC activo produjo un bloqueo
(Code Integrity, evento 3077, 11:32). Después de eso, el mismo binario cargó sin
bloqueo en todas las pruebas. SAC ya había bloqueado antes instaladores
compilados en local (`release\Sayso-Setup-1.3.0.exe`, `Sayso-Beta-1.3.x`). Las
builds de CI no se bloquean porque el `.exe` y el instalador van firmados.

- SAC **no se puede reactivar** sin reinstalar Windows, y no admite excepciones
  por archivo.
- **No es la causa** del banner: en producción el módulo carga y entrega
  buffers.

### 5.2 El `.node` se publica sin firmar en Windows

Verificado en el `native_audio.node` instalado de la build de testers:
`NotSigned`, mientras que `Sayso.exe` está firmado `Valid` (AskSayso, Inc.).

- electron-builder 24.13.3 (`winPackager.js` `shouldSignFile`) firma solo `.exe`.
  Los `.dll` se firman solo con `signDlls: true`, y los `.node` solo si están en
  `signExts`. No tenemos ninguno de los dos configurado.
- El paso "Verify the signatures" de `release-windows.yml` no revisa el `.node`.
- La key está en el HSM de Azure Artifact Signing y **no es exportable**: no se
  puede firmar en local.
- Consecuencia posible, sin confirmar: un usuario con SAC o WDAC podría quedarse
  sin audio. Merece un ticket aparte (fix probable: `signExts: [".node"]` y
  sumarlo al verify).

### 5.3 El canal de lifecycle events funciona

No está roto: `audioManager.ts:128` registra el callback y `native-audio/index.js`
junto con `win.cpp` lo exportan. En producción no aparece nada porque todos los
`EmitLifecycleEvent` de `win.cpp` son de fallos o cambios de dispositivo
(`*_failed`, `*_timeout`, `default_capture_device_changed`…). Consecuencia
útil: cualquier diagnóstico emitido desde el nativo sale directo en el log de la
app como `[NativeAudio] lifecycle: …`, sin DebugView.

### 5.4 El socket user nunca recibe transcripciones

Incluso con el mic funcionando, `WebSocketClient:user` solo recibe
`"type": "connected"`. Los `insight` y `smart_capture` llegan por el socket del
prospect. Por eso "cero transcripciones del user" **no prueba** que el mic esté
muerto.

### 5.5 Detalles del entorno de dev

- `npm run dev 2>&1 | Tee-Object …` en Windows PowerShell 5 muestra los warnings
  de stderr como `NativeCommandError` en rojo. No es un fallo.
- `git restore` sobre `win.cpp` no cambia el binario: `build/` está ignorado.
  Hay que correr `npm run rebuild-native-win`.

---

## 6. Pendientes

1. **Preguntar al usuario que reportó:**
   > En esas pruebas donde te apareció el banner rojo del micrófono, ¿vos estabas
   > hablando, o solo pusiste audio en la PC y te quedaste callado? Si hablaste:
   > ¿el banner se quedaba mientras hablabas, o desaparecía?

   Si se quedó callado, es este mismo falso positivo. Si habló y el banner no se
   iba, es un mic muerto real: correr en su máquina la probe de §A.1.
2. ~~**SAYSO-468:** quitar el banner.~~ Hecho (§7), falta el PR.
3. **SAYSO-458:** detectar de forma confiable un mic muteado o muerto (reemplaza a SAYSO-469, marcado duplicado; SAYSO-461 cancelado). Investigar cómo lo hacen Discord, Zoom
   y Meet, y distinguir un mic muerto de un silencio que el driver pone en cero.
4. Ticket aparte: firmar `native_audio.node` en CI (§5.2).
5. Opcional: la prueba de "Mejoras de audio" y la del HyperX en Mac (§4.1).
6. ~~Limpieza local: el `.node` de `build/Release` todavía tiene el bloque diag.~~
   Recompilado limpio durante SAYSO-459.
7. **SAYSO-459:** lo que en §3.3 no sabíamos ahora llega a Sentry, sin pedir
   logs. En Windows, `getMicInputDiagnostics` reporta nombre e id del mic, mute,
   volumen, form factor y cuántos paquetes marcó WASAPI como `SILENT`. La alerta
   de silencio lleva el contexto de la sesión. Hay un evento al recuperarse
   (`silentTotalMs`) y otro al cerrar una sesión con el mic ≥80 % en silencio.

---

## 7. El fix: SAYSO-468 (banner quitado hasta nuevo aviso)

**Por qué:** la alerta es demasiado sensible para Windows. Cuando el agente
habla, el mic se detecta bien. Pero cuando se queda callado, el mic o el driver
(lo que sea) entregan entre −120 dBFS (Realtek) e incluso −165 dBFS (HyperX).
Eso queda muy por debajo del piso de −80, así que cualquier tramo de 20 s
escuchando al prospect disparaba el banner con un mic que funciona. Se quita en
**las dos plataformas** hasta rediseñar la detección (SAYSO-458).

**Qué cambió** (branch
`fix/sayso-468-remove-sayso-cant-hear-your-microphone-alert-dialog`, commit
`7f9001f`, plan en `docs/plans/sayso-468-remove-mic-silent-banner.md`):

| Archivo | Cambio |
|---|---|
| `electron/audio/audioManager.ts` | `trackCueUserLevel` ya no envía `cue-mic-silent` / `cue-mic-silent-cleared`. Se borraron el re-aviso cada 30 s (`CUE_MIC_SILENT_RENOTIFY_MS`, `alertedAtMs`) y `sendToCoachWindow`. `alerted` queda como latch por tramo de silencio |
| `src/coachWindow/components/CoachWindowMain.tsx` | Se borraron `MIC_SILENT_MESSAGE` y los listeners `onMicSilent` / `onMicSilentCleared` |
| `electron/preload.ts`, `src/types/electron.d.ts` | Se quitaron `onMicSilent` y `onMicSilentCleared` |
| `docs/IPC_CONTRACT.md` | Se quitaron las filas de los dos canales |

**Qué se mantiene**, como datos para el rediseño:
- El log `[Cue] Microphone is delivering buffers but no signal…`, una vez por
  tramo de silencio.
- `[Cue] Microphone signal restored` cuando vuelve la señal.
- El aviso a Sentry `[Cue] Microphone delivering silence mid-session`, una vez
  por sesión.
- Las stats de teardown (`userPeakDb`, `userSilentSeconds`).

**Qué NO se tocó:** el banner de mic perdido sin recuperación
(`mic_route_recovery_failed`, SAYSO-353). Ese sí detecta una pérdida real de
dispositivo y sigue activo.

---

## Apéndice: herramientas de diagnóstico

Ninguna requiere recompilar el módulo nativo, así que Smart App Control no
interviene.

### A.1 Probe WASAPI standalone (PowerShell + C#)

Abre el mic por defecto con la misma secuencia que `win.cpp` (`GetMixFormat` →
`Initialize` en modo shared, 200 ms → polling) y reporta formato, mute, volumen,
paquetes con flag SILENT y pico capturado. Sirve para la máquina de un usuario:
dice si Windows le entrega audio real a cualquier app. Correr en PowerShell,
**hablando** durante la captura:

```powershell
powershell -ExecutionPolicy Bypass -File .\mic-probe.ps1 -Seconds 5
```

- `peak(captured)` con dBFS y `silent_flag=0`: Windows entrega audio y el
  problema estaría en Sayso.
- `ZEROS` o `silent_flag` igual a `packets`: el problema es de la máquina
  (driver, mute o privacidad).

### A.2 Probe del módulo nativo real

Carga un `native-audio` (el de dev o el copiado de una instalación) dentro de
Electron como Node y mide el pico por segundo. Uso:

```powershell
$env:ELECTRON_RUN_AS_NODE='1'
& .\node_modules\electron\dist\electron.exe .\node-compare.js .\electron\native-audio 8 2>&1 | Out-String
# o con el exe instalado: & "$env:LOCALAPPDATA\Programs\Sayso\Sayso.exe" .\node-compare.js <dir> 8
```

`| Out-String` hace falta porque `electron.exe` es una app GUI y, sin él,
PowerShell no espera ni captura la salida.

```js
// ELECTRON_RUN_AS_NODE=1 electron.exe node-compare.js <native-audio dir> <seconds>
const path = require('path');
const na = require(path.resolve(process.argv[2]));
const seconds = Number(process.argv[3] || 8);

const win = { peak: 0, zeroChunks: 0, chunks: 0 };
function measure(buf, f) {
  let peak = 0;
  for (let i = 0; i + 4 <= buf.length; i += 4) {
    const a = Math.abs(f.isFloat ? buf.readFloatLE(i) : buf.readInt32LE(i) / 2147483648);
    if (a > peak) peak = a;
  }
  return peak;
}
const db = p => (p > 0 ? (20 * Math.log10(p)).toFixed(1) : '-120 (ZERO)');
(async () => {
  await na.startMicrophoneCapture({
    streamingCallback: (buf, f) => {
      const p = measure(buf, f);
      win.chunks++; if (p === 0) win.zeroChunks++; if (p > win.peak) win.peak = p;
    },
  });
  for (let s = 1; s <= seconds; s++) {
    await new Promise(r => setTimeout(r, 1000));
    console.log(`t=${s}s chunks=${win.chunks} zeroChunks=${win.zeroChunks} peakDb=${db(win.peak)}`);
    win.peak = 0; win.zeroChunks = 0; win.chunks = 0;
  }
  await na.stopMicrophoneCapture();
  process.exit(0);
})().catch(e => { console.error('ERR', e); process.exit(1); });
```

### A.3 Bloque diag dentro de `win.cpp` (temporal, no commitear)

Va al principio de `PushCapturedBuffer`, **antes** del `resize` y del
`std::fill`: si va después, siempre lee ceros, porque los pusimos nosotros.
Emite una línea por segundo y por path (`microphone_diag` / `system_audio_diag`)
con `packets`, `silent_flag`, `peak_ppm` (pico × 1e6: `0` = ceros absolutos),
`rate`, `ch`, `bits` y `float`. Compiló con MSVC (`npm run rebuild-native-win`).
Sale en el log de la app gracias a §5.3.

```cpp
  // ── DIAG TEMPORAL — quitar antes de cualquier commit. ──
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

### A.4 Código de `mic-probe.ps1`

```powershell
param([int]$Seconds = 5, [int]$Role = 0)  # Role: 0=eConsole (what win.cpp uses), 2=eCommunications
$src = @'
using System;
using System.Runtime.InteropServices;
using System.Threading;

[ComImport, Guid("BCDE0395-E52F-467C-8E3D-C4579291692E")] class MMDeviceEnumeratorCo {}
[Guid("A95664D2-9614-4F35-A746-DE8DB63617E6"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
interface IMMDeviceEnumerator {
  int EnumAudioEndpoints(int flow, int mask, out IntPtr devs);
  int GetDefaultAudioEndpoint(int flow, int role, out IMMDevice dev);
}
[Guid("D666063F-1587-4E43-81F1-B948E807363F"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
interface IMMDevice {
  int Activate(ref Guid iid, int ctx, IntPtr p, [MarshalAs(UnmanagedType.IUnknown)] out object o);
  int OpenPropertyStore(int access, out IntPtr store);
  int GetId([MarshalAs(UnmanagedType.LPWStr)] out string id);
  int GetState(out int state);
}
[Guid("1CB9AD4C-DBFA-4c32-B178-C2F568A703B2"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
interface IAudioClient {
  int Initialize(int shareMode, int flags, long bufDur, long period, IntPtr fmt, IntPtr session);
  int GetBufferSize(out uint n);
  int GetStreamLatency(out long l);
  int GetCurrentPadding(out uint p);
  int IsFormatSupported(int mode, IntPtr fmt, out IntPtr closest);
  int GetMixFormat(out IntPtr fmt);
  int GetDevicePeriod(out long d, out long m);
  int Start(); int Stop(); int Reset();
  int SetEventHandle(IntPtr h);
  int GetService(ref Guid iid, [MarshalAs(UnmanagedType.IUnknown)] out object o);
}
[Guid("C8ADBD64-E71E-48a0-A4DE-185C395CD317"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
interface IAudioCaptureClient {
  int GetBuffer(out IntPtr data, out uint frames, out uint flags, out ulong pos, out ulong qpc);
  int ReleaseBuffer(uint frames);
  int GetNextPacketSize(out uint n);
}
[Guid("5CDF2C82-841E-4546-9722-0CF74078229A"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
interface IAudioEndpointVolume {
  int RegisterControlChangeNotify(IntPtr p); int UnregisterControlChangeNotify(IntPtr p);
  int GetChannelCount(out uint n);
  int SetMasterVolumeLevel(float l, ref Guid g); int SetMasterVolumeLevelScalar(float l, ref Guid g);
  int GetMasterVolumeLevel(out float l); int GetMasterVolumeLevelScalar(out float l);
  int SetChannelVolumeLevel(uint c, float l, ref Guid g); int SetChannelVolumeLevelScalar(uint c, float l, ref Guid g);
  int GetChannelVolumeLevel(uint c, out float l); int GetChannelVolumeLevelScalar(uint c, out float l);
  int SetMute(bool m, ref Guid g); int GetMute(out bool m);
}
[Guid("C02216F6-8C67-4B5B-9D00-D008E73E0064"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
interface IAudioMeterInformation { int GetPeakValue(out float p); }

public static class MicProbe {
  public static string Run(int seconds, int role) {
    var en = (IMMDeviceEnumerator)new MMDeviceEnumeratorCo();
    IMMDevice dev; Check(en.GetDefaultAudioEndpoint(1 /*eCapture*/, role, out dev), "GetDefaultAudioEndpoint");
    string id; dev.GetId(out id);
    object o;
    Guid gVol = typeof(IAudioEndpointVolume).GUID; dev.Activate(ref gVol, 23, IntPtr.Zero, out o);
    var vol = (IAudioEndpointVolume)o; bool mute; float scalar; vol.GetMute(out mute); vol.GetMasterVolumeLevelScalar(out scalar);
    Guid gMeter = typeof(IAudioMeterInformation).GUID; dev.Activate(ref gMeter, 23, IntPtr.Zero, out o);
    var meter = (IAudioMeterInformation)o;
    Guid gAc = typeof(IAudioClient).GUID; Check(dev.Activate(ref gAc, 23, IntPtr.Zero, out o), "Activate IAudioClient");
    var ac = (IAudioClient)o;
    IntPtr fmt; Check(ac.GetMixFormat(out fmt), "GetMixFormat");
    ushort tag = (ushort)Marshal.ReadInt16(fmt, 0); int ch = Marshal.ReadInt16(fmt, 2); int rate = Marshal.ReadInt32(fmt, 4);
    int blockAlign = Marshal.ReadInt16(fmt, 12); int bits = Marshal.ReadInt16(fmt, 14);
    bool isFloat = tag == 3;
    if (tag == 0xFFFE) { var sub = new byte[16]; Marshal.Copy(IntPtr.Add(fmt, 24), sub, 0, 16); isFloat = new Guid(sub) == new Guid("00000003-0000-0010-8000-00aa00389b71"); }
    Check(ac.Initialize(0 /*shared*/, 0, 200 * 10000, 0, fmt, IntPtr.Zero), "Initialize");
    Guid gCc = typeof(IAudioCaptureClient).GUID; Check(ac.GetService(ref gCc, out o), "GetService");
    var cc = (IAudioCaptureClient)o;
    Check(ac.Start(), "Start");
    long packets = 0, silentFlagged = 0, frames = 0; double peak = 0; float meterPeak = 0;
    var until = DateTime.UtcNow.AddSeconds(seconds);
    while (DateTime.UtcNow < until) {
      float mp; meter.GetPeakValue(out mp); if (mp > meterPeak) meterPeak = mp;
      uint n; Check(cc.GetNextPacketSize(out n), "GetNextPacketSize");
      if (n == 0) { Thread.Sleep(5); continue; }
      IntPtr data; uint fa, flags; ulong p1, p2;
      Check(cc.GetBuffer(out data, out fa, out flags, out p1, out p2), "GetBuffer");
      packets++; frames += fa;
      if ((flags & 0x2) != 0) silentFlagged++;
      else if (data != IntPtr.Zero) {
        int bytes = (int)fa * blockAlign; var buf = new byte[bytes]; Marshal.Copy(data, buf, 0, bytes);
        for (int i = 0; i + 4 <= bytes && bits == 32; i += 4) {
          double a = isFloat ? Math.Abs(BitConverter.ToSingle(buf, i)) : Math.Abs((double)BitConverter.ToInt32(buf, i)) / 2147483648.0;
          if (a > peak) peak = a;
        }
        for (int i = 0; i + 2 <= bytes && bits == 16; i += 2) {
          double a = Math.Abs((double)BitConverter.ToInt16(buf, i)) / 32768.0; if (a > peak) peak = a;
        }
      }
      cc.ReleaseBuffer(fa);
    }
    ac.Stop();
    return string.Format(
      "endpoint={0}\nrole={1} format: rate={2} ch={3} bits={4} float={5} blockAlign={6}\nendpoint mute={7} volume={8:F2}\npackets={9} frames={10} silent_flag={11}\npeak(captured)={12:F6} ({13})\npeak(endpoint meter)={14:F6}",
      id, role == 0 ? "eConsole" : "eCommunications", rate, ch, bits, isFloat, blockAlign, mute, scalar,
      packets, frames, silentFlagged, peak, peak > 0 ? (20 * Math.Log10(peak)).ToString("F1") + " dBFS" : "ZEROS", meterPeak);
  }
  static void Check(int hr, string what) { if (hr < 0) throw new Exception(what + " failed hr=0x" + hr.ToString("X8")); }
}
'@
Add-Type -TypeDefinition $src -Language CSharp
[MicProbe]::Run($Seconds, $Role)
```
