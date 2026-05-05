import { createContext, useContext, useCallback, useEffect, useState, useRef } from 'react'
import { useSessionExpiry } from '@/hooks/useSessionExpiry'
import * as Sentry from "@sentry/electron/renderer"
import { useAccounts } from '../hooks/useAccounts'
import { useLocation } from 'react-router-dom'
import { Account, AuthResult, SignInData, User } from '@/types/user'
import { AALLevel, MFAServiceError } from '@/types/supabaseMFA'
import type { Factor } from '@supabase/supabase-js'

interface AuthContextValue {
  signIn: (data: SignInData) => Promise<AuthResult>;
  handleSignOut: () => Promise<void>;
  user: User | null;
  globalUser: Account | null;
  authToken: string | null;
  userLoading: boolean;
  loading: boolean;
  updateGlobalUser: (accountEmail: string) => Promise<void>;
  mfaRequired: boolean;
  currentAAL: AALLevel | null;
  mfaFactors: Factor[];
  checkMFAStatus: () => Promise<boolean>;
  verifyMFA: (code: string) => Promise<{ success: boolean; error: MFAServiceError | null }>;
  clearMFARequired: () => void;
  checkIfNeedsMFA: (currentLevel: AALLevel | null | undefined, nextLevel: AALLevel | null | undefined) => boolean;
}

const AuthContext = createContext<AuthContextValue>({} as AuthContextValue)

export const AuthProvider = ({ children }: { children: React.ReactNode }) => {
  const [user, setUser] = useState<User | null>(null)
  const [loading, setLoading] = useState(true)
  const [globalUser, setGlobalUser] = useState(null)
  const [authToken, setAuthToken] = useState<string | null>(null)
  const [userLoading, setUserLoading] = useState(true)
  const accountCreationRef = useRef<Promise<void> | null>(null)
  const [mfaRequired, setMfaRequired] = useState(false)
  const [currentAAL, setCurrentAAL] = useState<AALLevel | null>(null)
  const [mfaFactors, setMfaFactors] = useState<Factor[]>([])

  const { createAccount, getAccount } = useAccounts()

  const updateGlobalUserState = (newGlobalUser: any) => {
    const ipcRenderer = window.electron?.ipcRenderer
    if (newGlobalUser === null) {
      ipcRenderer?.send('update-user-auth', { userAuthenticated: null })
      localStorage.removeItem('sayso-global-user')
    } else {
      ipcRenderer?.send('update-user-auth', { userAuthenticated: newGlobalUser })
      localStorage.setItem('sayso-global-user', JSON.stringify(newGlobalUser))
    }
    setGlobalUser(newGlobalUser)
  }

  const updateGlobalUser = async (accountEmail: string): Promise<void> => {
    try {
      const account = await getAccount(accountEmail)
      updateGlobalUserState(account)
    } catch (error) {
      console.error('Error updating global user:', error)
      Sentry.captureException(error)
    }
  }

  const resetUser = useCallback(() => {
    setUser(null)
    updateGlobalUserState(null)
    setAuthToken(null)
    Sentry.setUser(null)
    setMfaRequired(false)
    setCurrentAAL(null)
    setMfaFactors([])
  }, [])

  const checkIfNeedsMFA = (currentLevel: AALLevel | null | undefined, nextLevel: AALLevel | null | undefined): boolean => {
    if (!currentLevel || !nextLevel) return false
    return currentLevel === 'aal1' && nextLevel === 'aal2'
  }

  const checkMFAStatus = async (): Promise<boolean> => {
    if (mfaFactors.length === 0) return false
    const needsMFA = checkIfNeedsMFA(currentAAL, mfaFactors.length > 0 ? 'aal2' : 'aal1')
    setMfaRequired(needsMFA)
    return needsMFA
  }

  const verifyMFA = async (code: string): Promise<{ success: boolean; error: MFAServiceError | null }> => {
    if (mfaFactors.length === 0) {
      return { success: false, error: { message: 'No MFA factors enrolled' } }
    }

    const factorId = mfaFactors[0].id
    const result = await window.electron?.ipcRenderer?.invoke('auth:verify-mfa', { factorId, code }) as
      { success: boolean; error?: string } | undefined

    if (result?.success) {
      setMfaRequired(false)
      setCurrentAAL('aal2')
      // Fetch the fresh token so authToken state is up to date
      const token: string | null = await window.electron?.ipcRenderer?.invoke('auth:get-token') ?? null
      setAuthToken(token)
    }

    return {
      success: result?.success ?? false,
      error: result?.error ? { message: result.error } : null,
    }
  }

  const clearMFARequired = () => setMfaRequired(false)

  const handleSignOut = async () => {
    await window.electron?.ipcRenderer?.invoke('auth:sign-out')
    resetUser()
  }

  // ─── Sign-in ─────────────────────────────────────────────────────────────
  const signIn = async (data: SignInData): Promise<AuthResult> => {
    const result = await window.electron?.ipcRenderer?.invoke('auth:sign-in', {
      email: data.email,
      password: data.password,
    }) as { success: boolean; mfaRequired?: boolean; mfaFactors?: any[]; error?: string } | undefined

    if (!result?.success) {
      // Shape into the AuthResult the rest of the app expects
      return {
        data: { user: null, session: null },
        error: { message: result?.error ?? 'Sign in failed' } as any,
      } as AuthResult
    }

    if (result.mfaRequired && result.mfaFactors) {
      setMfaRequired(true)
      setCurrentAAL('aal1')
      setMfaFactors(result.mfaFactors as Factor[])
      // Return a partial success — MFA still pending
      return { data: { user: null, session: null }, error: null } as AuthResult
    }

    // Full auth — pull the token and user from main
    const token: string | null = await window.electron?.ipcRenderer?.invoke('auth:get-token') ?? null
    const state = await window.electron?.ipcRenderer?.invoke('auth:get-state') as
      { user: { id: string; email: string } | null; isAuthenticated: boolean } | undefined

    if (state?.user && token) {
      setUser(state.user as User)
      setAuthToken(token)
      Sentry.setUser({ id: state.user.id, email: state.user.email })
    }

    return { data: { user: state?.user ?? null, session: null }, error: null } as AuthResult
  }

  // ─── Bootstrap: read auth state from main on mount ───────────────────────
  // The splash window may open after a successful silent auth (authManager.init
  // already ran in main). We just read that state instead of re-authenticating.
  useEffect(() => {
    const bootstrap = async () => {
      const state = await window.electron?.ipcRenderer?.invoke('auth:get-state') as
        { user: { id: string; email: string } | null; isAuthenticated: boolean; accessToken: string | null } | undefined

      if (state?.isAuthenticated && state.user) {
        setUser(state.user as User)
        setAuthToken(state.accessToken ?? null)
        Sentry.setUser({ id: state.user.id, email: state.user.email })
      }

      setLoading(false)
    }

    bootstrap()
  }, [])

  // ─── Auth state broadcasts from main ─────────────────────────────────────
  useEffect(() => {
    const ipcRenderer = window.electron?.ipcRenderer
    if (!ipcRenderer) return

    const offState = ipcRenderer.on('auth:state', (data: any) => {
      if (data?.isAuthenticated && data.user) {
        setUser(data.user as User)
        Sentry.setUser({ id: data.user.id, email: data.user.email })
      } else {
        setUser(null)
        Sentry.setUser(null)
      }
    })

    return () => offState?.()
  }, [])

  // ─── Session expiry ───────────────────────────────────────────────────────
  const handleSessionExpired = useCallback(() => {
    console.log('🔐 AuthContext: Session expired event received')
    resetUser()
  }, [resetUser])
  useSessionExpiry(handleSessionExpired)

  // ─── Tray-triggered logout ────────────────────────────────────────────────
  useEffect(() => {
    const ipcRenderer = window.electron?.ipcRenderer
    if (!ipcRenderer) return
    ipcRenderer.on('trigger-logout', handleSignOut as any)
    return () => { ipcRenderer.off('trigger-logout', handleSignOut as any) }
  }, [])

  // ─── Load account profile whenever user changes ───────────────────────────
  useEffect(() => {
    let timeoutId: NodeJS.Timeout | undefined

    setUserLoading(true)

    if (user) {
      timeoutId = setTimeout(async () => {
        try {
          if (accountCreationRef.current) {
            await accountCreationRef.current
          }
          const account = await getAccount(user.email)
          updateGlobalUserState(account)
          Sentry.setUser({
            id: account?.id,
            email: account?.email,
            name: account?.name,
            lastname: account?.lastname,
            company_id: account?.company_id,
            subscription_monthly_minutes: account?.subscription_monthly_minutes,
            subscription_plan_id: account?.subscription_plan_id,
            subscription_status: account?.subscription_status,
          })
          setUserLoading(false)
        } catch (error) {
          console.error('Error fetching account:', error)
          Sentry.captureException(error)
          setUserLoading(false)
        }
      }, 300)
    } else {
      updateGlobalUserState(null)
      setUserLoading(false)
    }

    return () => { if (timeoutId) clearTimeout(timeoutId) }
  }, [user])

  const values: AuthContextValue = {
    signIn,
    handleSignOut,
    user,
    globalUser,
    authToken,
    userLoading,
    loading,
    updateGlobalUser,
    mfaRequired,
    currentAAL,
    mfaFactors,
    checkMFAStatus,
    verifyMFA,
    clearMFARequired,
    checkIfNeedsMFA,
  }

  return (
    <AuthContext.Provider value={values}>
      {!loading && children}
    </AuthContext.Provider>
  )
}

export const useAuth = () => useContext(AuthContext)
