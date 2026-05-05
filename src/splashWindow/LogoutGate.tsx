import SaysoLoader from "@/components/SaysoLoader";
import { useEffect, useState } from "react";

export default function LogoutGate({ children }: { children: React.ReactNode }) {
    const [ready, setReady] = useState(false);

    useEffect(() => {
        const params = new URLSearchParams(window.location.search);
        if (params.get('logout') === 'true') {
            window.electron?.ipcRenderer?.invoke('auth:sign-out').finally(() => setReady(true));
        } else {
            setReady(true);
        }
    }, []);

    if (!ready) return <SaysoLoader />;
    return <>{children}</>;
}