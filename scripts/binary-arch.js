const fs = require('fs');

/**
 * Reads a compiled binary's architecture from its own header.
 *
 * Replaces the Unix `file` utility, which has no stock Windows equivalent and so
 * aborted `electron-builder --win` in the beforePack hook (SAYSO-399). Kept
 * dependency-free and shellout-free: this runs before `build:electron`, so it
 * cannot require anything under `electron/`.
 *
 * Recognises only what this project builds: macOS x64/arm64 and Windows x64.
 * Anything else returns 'unknown', which callers treat as "don't warn".
 *
 * @param {string} filePath
 * @returns {'x64'|'arm64'|'unknown'}
 */
function archOfBinary(filePath) {
  let fd;
  try {
    fd = fs.openSync(filePath, 'r');
    const head = Buffer.alloc(64);
    if (fs.readSync(fd, head, 0, 64, 0) < 64) return 'unknown';
    const magic = head.readUInt32BE(0);

    // Mach-O 64-bit: cputype at offset 4, endianness follows the magic.
    if (magic === 0xfeedfacf || magic === 0xcffaedfe) {
      const cpu = magic === 0xcffaedfe ? head.readUInt32LE(4) : head.readUInt32BE(4);
      if (cpu === 0x01000007) return 'x64';
      if (cpu === 0x0100000c) return 'arm64';
      return 'unknown';
    }

    // PE: "MZ", e_lfanew at 0x3c, then "PE\0\0" and the Machine field.
    if (head[0] === 0x4d && head[1] === 0x5a) {
      const coff = Buffer.alloc(6);
      if (fs.readSync(fd, coff, 0, 6, head.readUInt32LE(0x3c)) < 6) return 'unknown';
      if (coff.toString('latin1', 0, 4) !== 'PE\0\0') return 'unknown';
      const machine = coff.readUInt16LE(4);
      if (machine === 0x8664) return 'x64';
      if (machine === 0xaa64) return 'arm64';
      return 'unknown';
    }

    return 'unknown';
  } catch (error) {
    return 'unknown';
  } finally {
    if (fd !== undefined) fs.closeSync(fd);
  }
}

module.exports = { archOfBinary };
