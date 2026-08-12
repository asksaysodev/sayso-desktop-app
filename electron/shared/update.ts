/**
 * The update state contract, shared by both TypeScript programs.
 *
 * Main owns this state and pushes it over `update:state-changed`, so the
 * definition lives on the main side and `src/types/update.ts` re-exports it —
 * not the other way around. tsconfig.electron.json pins `rootDir` to
 * `electron/` (it emits each .js next to its .ts, so main.js stays at
 * electron/main.js), which makes electron → src imports a hard TS6059 error.
 * src → electron works because the renderer program is `noEmit` with bundler
 * resolution.
 *
 * TYPES ONLY. This file is pulled into the renderer's program, so it must
 * never import electron or node APIs — declarations here are erased at build
 * time and nothing from it may survive into the bundle.
 */

/**
 * 'blocked' = macOS app is outside /Applications, so updates can't be installed
 * at all (see electron/utils/applicationsFolder.ts).
 */
export type UpdatePhase = 'idle' | 'checking' | 'available' | 'downloading' | 'downloaded' | 'error' | 'blocked';

export interface UpdateState {
  phase: UpdatePhase;
  currentVersion: string;
  newVersion: string | null;
  progressPercent: number;
  errorMessage: string | null;
}
