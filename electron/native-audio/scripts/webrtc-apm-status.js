// Run by binding.gyp: prints 1 when the WebRTC echo-cancellation library
// (built by build-webrtc-apm.ps1) is present for this platform, 0 when not.
//
// A developer build without the library still works, without echo
// cancellation (the addon reports aec_engine_unavailable reason=not_built).
// With SAYSO_REQUIRE_WEBRTC_APM=1 a missing library fails the build instead,
// so a release can never ship without echo cancellation unnoticed; the
// release packaging scripts set it (scripts/native-build-env.js at the repo
// root).

const fs = require('fs');
const path = require('path');

const required = process.env.SAYSO_REQUIRE_WEBRTC_APM === '1';

// Only Windows x64 has a library so far; macOS comes with its own build.
if (process.platform !== 'win32') {
  process.stdout.write('0');
  process.exit(0);
}

const lib = path.join(__dirname, '..', 'deps', 'webrtc_apm', 'win-x64', 'sayso_webrtc_apm.lib');
if (fs.existsSync(lib)) {
  process.stdout.write('1');
  process.exit(0);
}

if (required) {
  console.error(
    `\nnative_audio: the WebRTC echo-cancellation library is missing:\n  ${lib}\n` +
      'This build is required to include echo cancellation (SAYSO_REQUIRE_WEBRTC_APM=1).\n' +
      'Place the released sayso_webrtc_apm.lib there, or build it with\n' +
      '  electron/native-audio/scripts/build-webrtc-apm.ps1\n'
  );
  process.exit(1);
}

process.stdout.write('0');
