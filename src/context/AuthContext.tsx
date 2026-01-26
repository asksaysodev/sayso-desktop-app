import { createContext, useContext, useEffect, useState, useRef } from 'react'
import { supabase } from '../config/supabase'
import { useAccounts } from '../hooks/useAccounts'
import { useLocation } from 'react-router-dom'
import * as Sentry from "@sentry/electron/renderer"
import { Account, AuthResult, SignInData, SignUpData, User } from '@/types/user'

interface AuthContextValue {
  signUp: (data: SignUpData) => Promise<AuthResult>;
  signIn: (data: SignInData) => Promise<AuthResult>;
  handleSignOut: () => Promise<void>;
  user: User | null;
  globalUser: Account | null;
  authToken: string | null;
  userLoading: boolean;
  loading: boolean;
  updateGlobalUser: (accountEmail: string) => Promise<void>;
}

const AuthContext = createContext<AuthContextValue>({} as AuthContextValue)

export const AuthProvider = ({ children }: { children: React.ReactNode }) => {
  const [user, setUser] = useState<User | null>(null)
  const [loading, setLoading] = useState(true)
  const [globalUser, setGlobalUser] = useState(null)
  const [authToken, setAuthToken] = useState<string | null>(null)
  const [userLoading, setUserLoading] = useState(true)
  const prevUserRef = useRef<User | null>(null)
  const location = useLocation()

  const { createAccount, getAccount } = useAccounts()

  // Wrapper function to handle localStorage updates
  const updateGlobalUserState = (newGlobalUser: any) => { // $FixTS
    const ipcRenderer = window.electron?.ipcRenderer;
    if (newGlobalUser === null) {
      if (ipcRenderer) {
        window.electron?.ipcRenderer?.send('update-user-auth', { userAuthenticated: null });
      }
      localStorage.removeItem('sayso-global-user')
    } else {
      if (ipcRenderer) {
        ipcRenderer.send('update-user-auth', { userAuthenticated: newGlobalUser });
      }

      localStorage.setItem('sayso-global-user', JSON.stringify(newGlobalUser))
    }
    setGlobalUser(newGlobalUser)
  }

  const updateGlobalUser = async (accountEmail: string): Promise<void> => {
    try{
      const account = await getAccount(accountEmail);
      updateGlobalUserState(account);
    } catch (error) {
      console.error('Error updating global user:', error);
      Sentry.captureException(error);
    }
  }

  const resetUser = () => {
    setUser(null);
    updateGlobalUserState(null);
    setAuthToken(null);
    Sentry.setUser(null);
  }

  const handleSignOut = async () => {
    await supabase.auth.signOut();

    resetUser();
    
    if (window.electron?.ipcRenderer) {
      window.electron.ipcRenderer.send('update-user-auth', { userAuthenticated: null });
    }
  }

  // Handle session expiration
  useEffect(() => {
    const handleSessionExpired = () => {
      console.log('🔐 AuthContext: Session expired event received');
      resetUser();
    };

    window.addEventListener('auth:session-expired', handleSessionExpired);

    return () => {
      window.removeEventListener('auth:session-expired', handleSessionExpired);
    };
  }, []);

  useEffect(() => {
    // Skip auth check for /zoom-success
    if (location.pathname === '/zoom-success') {
      setLoading(false)
      return
    }

    // Check active sessions and sets the user
    supabase.auth.getSession().then(({ data: { session } }) => {
      setUser(session?.user as User | null)
      prevUserRef.current = session?.user as User | null
      setLoading(false)
    })

    // Listen for changes on auth state (sign in, sign out, etc.)
    const { data: { subscription } } = supabase.auth.onAuthStateChange((event, session) => {
      
      // Only update state for actual auth events
      if (event === 'SIGNED_IN' || event === 'SIGNED_OUT' || event === 'USER_UPDATED' || event === 'TOKEN_REFRESHED') {
        // Prevent unnecessary state updates if the user hasn't actually changed
        const newUser = session?.user as User | null
        // For token refresh, we might just need to update the token even if user is same
        if (event === 'TOKEN_REFRESHED' || JSON.stringify(newUser) !== JSON.stringify(prevUserRef.current)) {
          setUser(newUser)
          prevUserRef.current = newUser
          setAuthToken(session?.access_token ?? null)
          setLoading(false)
        }
      }
    })

    return () => subscription.unsubscribe()
  }, [location])

  useEffect(() => {
    let timeoutId: NodeJS.Timeout | undefined;

    // Always set userLoading to true when user changes (even if user is null)
    setUserLoading(true);

    if (user) {
      // Add a small delay to prevent rapid re-fetching
      timeoutId = setTimeout(() => {
        getAccount(user.email).then((account) => {
          updateGlobalUserState(account)
          Sentry.setUser({ 
            id: account?.id,
            email: account?.email,
            name: account?.name,
            lastname: account?.lastname,
            company_id: account?.company_id,
            subscription_monthly_minutes: account?.subscription_monthly_minutes,
            subscription_plan_id: account?.subscription_plan_id,
            subscription_status: account?.subscription_status
          });
          setUserLoading(false)
        })
      }, 300) // 300ms delay
    } else {
      updateGlobalUserState(null)
      setUserLoading(false)
    }

    return () => {
      if (timeoutId) {
        clearTimeout(timeoutId)
      }
    }
  }, [user])

  const values = {
    signUp: async (data) => {
      // First, sign up with Supabase Auth
      const result = await supabase.auth.signUp(data)
      // If sign up is successful, create the account in the DB
      if (!result.error) {
        // Try to get user info from the data/options
        const { email, options } = data
        const { name, lastname, company } = options?.data || {}
        try {
          await createAccount({ email, name, lastname, company })
        } catch (err) {
          console.error('Error creating account in DB:', err)
          Sentry.captureException(err)
        }
      }
      return result
    },
    signIn: (data) => supabase.auth.signInWithPassword(data),
    handleSignOut,
    user,
    globalUser,
    authToken,
    userLoading,
    loading,
    updateGlobalUser,
  } as AuthContextValue;

  return (
    <AuthContext.Provider value={values}>
      {!loading && children}
    </AuthContext.Provider>
  )
}

export const useAuth = () => {
  return useContext(AuthContext)
} 