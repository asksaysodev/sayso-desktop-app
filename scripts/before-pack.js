const rebuildNativeForArch = require('./rebuild-native-for-arch').default;
const { tryBuildMicActivity } = require('./rebuild-mic-activity');

// electron-builder takes a single beforePack hook. This runs the native-audio
// hook exactly as before — first, unchanged, failures still fatal — then the
// call-detection addon, whose failure is only a warning so it can never block a
// release (see rebuild-mic-activity.js).
const ARCH_ENUM = { 0: 'ia32', 1: 'x64', 2: 'armv7l', 3: 'arm64', 4: 'universal' };

exports.default = async function (context) {
  await rebuildNativeForArch(context);

  const arch = typeof context.arch === 'number' ? ARCH_ENUM[context.arch] : context.arch;
  if (arch !== 'x64' && arch !== 'arm64') return;
  const electronVersion = context.packager?.config?.electronVersion;
  console.log(`[BEFORE_PACK] Building mic-activity for ${arch}`);
  tryBuildMicActivity(arch, electronVersion);
};
