import type { Account } from '@/types/user';

/**
 * Mirrors the server's requireBillingAccess and the web app's useCanManageBilling:
 * only team accounts are restricted, to owners and superadmins (not admins).
 * Picks copy only; the server enforces the rule.
 */
export const canManageBilling = (account: Account): boolean => (
    account.account_type !== 'team' || account.role === 'owner' || account.role === 'superadmin'
);
