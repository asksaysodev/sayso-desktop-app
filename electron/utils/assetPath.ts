import { app } from 'electron';
import path from 'node:path';

/**
 * Resolves a static asset (tray glyphs, window icons) for the main process.
 *
 * Packaged builds ship these via `extraResources` into `Resources/assets`, not
 * inside app.asar: electron-builder turns each filtered extraResources entry
 * into an exclude pattern for the app files, so `public/assets/**` is stripped
 * from the asar (SAYSO-432). Dev runs straight from the source tree.
 */
export function getAssetPath(fileName: string): string {
  return app.isPackaged
    ? path.join(process.resourcesPath, 'assets', fileName)
    : path.join(__dirname, '../../public/assets', fileName);
}
