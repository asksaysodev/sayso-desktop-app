import { useEffect, useState } from 'react';

export function useNetworkState(): { isReconnecting: boolean } {
  const [isReconnecting, setIsReconnecting] = useState(false);

  useEffect(() => {
    const ipc = window.electron?.ipcRenderer;
    if (!ipc) return;

    const handleNetworkState = (state: unknown) => {
      setIsReconnecting((state as string) === 'reconnecting');
    };

    const offNetworkState = ipc.on('network:state-changed', handleNetworkState);
    ipc.invoke('network:get-state')
      .then((s: unknown) => setIsReconnecting((s as string) === 'reconnecting'))
      .catch(() => {});

    return () => {
      offNetworkState?.();
    };
  }, []);

  return { isReconnecting };
}
