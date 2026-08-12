// Single definition lives on the main side, next to the state machine that
// owns it — see electron/shared/update.ts for why the re-export points this
// direction. Type-only, so it is erased before Vite bundles anything.
export type { UpdatePhase, UpdateState } from '../../electron/shared/update';
