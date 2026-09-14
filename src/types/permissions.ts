// Single definition lives on the main side, next to the handler that builds
// the payload — see electron/shared/permissions.ts for why the re-export points
// this direction. Type-only, so it is erased before Vite bundles anything.
export type { PermissionRequirements, PermissionsCheckResult } from '../../electron/shared/permissions';
