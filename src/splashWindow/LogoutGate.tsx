import SaysoLoader from "@/components/SaysoLoader";
import { useEffect, useState } from "react";
import { supabase } from '@/config/supabase';

export default function LogoutGate({ children }: { children: React.ReactNode }) {
    const [ready, setReady] = useState(false);

    useEffect(() => {
        const params = new URLSearchParams(window.location.search);
        if (params.get('logout') === 'true') {
            supabase.auth.signOut().finally(() => setReady(true));
        } else {
            setReady(true);
        }
    }, []);

    if (!ready) return <SaysoLoader />;
    return <>{children}</>;
}