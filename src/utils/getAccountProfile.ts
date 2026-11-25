import type { Account } from '@/types/user';

const PROFILE_REPLY_TIMEOUT_MS = 1000;

/**
 * Account profile as main holds it right now, or null if it hasn't loaded. Asked per
 * use because main pushes `user-auth` only to the splash and tray windows.
 */
export const getAccountProfile = (): Promise<Account | null> => {
    const ipc = window.electron?.ipcRenderer;
    if (!ipc) return Promise.resolve(null);

    return new Promise((resolve) => {
        const finish = (account: Account | null) => {
            clearTimeout(timeoutId);
            unsubscribe?.();
            resolve(account);
        };
        const timeoutId = setTimeout(() => finish(null), PROFILE_REPLY_TIMEOUT_MS);
        const unsubscribe = ipc.on('user-auth', (state) => {
            finish((state as { authUser?: Account | false | null } | undefined)?.authUser || null);
        });
        ipc.send('get-user-auth');
    });
};
