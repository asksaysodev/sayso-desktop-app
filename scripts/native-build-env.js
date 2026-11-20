// Environment for the node-gyp builds that end up in an installer.
//
// Diagnostic call recording (SAYSO_AEC_DIAG_RECORDING) is never compiled into
// these builds, on any platform.
//
// On Windows, echo cancellation is part of the native audio module, so a
// packaged build must link the WebRTC library: SAYSO_REQUIRE_WEBRTC_APM=1 makes
// electron/native-audio/binding.gyp fail when it is missing, rather than
// quietly building a module without echo cancellation.
//
// SAYSO_ALLOW_BUILD_WITHOUT_AEC=1 lifts that requirement for one build, for
// emergencies only; the build then says loudly that it ships without echo
// cancellation.

function nativeBuildEnv() {
  const env = { ...process.env };
  // Diagnostic call recording is for developer builds only; never ship it.
  delete env.SAYSO_AEC_DIAG_RECORDING;
  if (process.platform !== 'win32') {
    return env;
  }
  if (process.env.SAYSO_ALLOW_BUILD_WITHOUT_AEC === '1') {
    console.warn(
      '\n⚠️  SAYSO_ALLOW_BUILD_WITHOUT_AEC=1: building the native audio module WITHOUT the ' +
        'echo-cancellation library requirement. If the library is missing, this build ships ' +
        'without echo cancellation.\n'
    );
    env.SAYSO_REQUIRE_WEBRTC_APM = '0';
  } else {
    env.SAYSO_REQUIRE_WEBRTC_APM = '1';
  }
  return env;
}

module.exports = { nativeBuildEnv };
