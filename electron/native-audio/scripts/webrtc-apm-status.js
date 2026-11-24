// Run by binding.gyp: prints 1 when the WebRTC echo-cancellation library
// (built by build-webrtc-apm.ps1, committed with Git LFS) is present for this
// platform, 0 when not.
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

// The library is committed with Git LFS. A checkout without Git LFS has a small
// text pointer file in its place, which the linker would reject as a corrupt
// library; say what to do instead.
function isLfsPointer(file) {
  try {
    if (fs.statSync(file).size > 1024) return false;
    return fs.readFileSync(file, 'utf8').startsWith('version https://git-lfs.github.com/spec/');
  } catch {
    return false;
  }
}

if (fs.existsSync(lib) && isLfsPointer(lib)) {
  console.error(
    `\nnative_audio: the WebRTC echo-cancellation library is a Git LFS pointer, not the library:\n  ${lib}\n` +
      'Install Git LFS and fetch it, from the repository root:\n' +
      '  git lfs install\n' +
      '  git lfs pull\n' +
      (required ? '' : 'Building without echo cancellation for now.\n')
  );
  if (required) process.exit(1);
  process.stdout.write('0');
  process.exit(0);
}

if (fs.existsSync(lib)) {
  process.stdout.write('1');
  process.exit(0);
}

if (required) {
  console.error(
    `\nnative_audio: the WebRTC echo-cancellation library is missing:\n  ${lib}\n` +
      'This build is required to include echo cancellation (SAYSO_REQUIRE_WEBRTC_APM=1).\n' +
      'It is committed with Git LFS: run "git lfs install" and "git lfs pull" from the\n' +
      'repository root. Or build it with electron/native-audio/scripts/build-webrtc-apm.ps1\n' +
      '(docs/WEBRTC_LIBRARY_BUILD.md).\n'
  );
  process.exit(1);
}

process.stdout.write('0');
