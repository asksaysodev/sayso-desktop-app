import { useMutation } from "@tanstack/react-query";
import getStripeCancellationPageUrl from "../services/getStripeCancellationPageUrl";
import { useToast } from "@/context/ToastContext";
import * as Sentry from "@sentry/electron/renderer";

export default function useStripeCancellation() {
    const { showToast } = useToast();
    
    const { mutate, isPending, isError} = useMutation({
        mutationFn: getStripeCancellationPageUrl,
        onSuccess: (data) => {
            window.electron?.openExternal(data.url);
        },
        onError: (error) => {
            Sentry.captureException(error);
            showToast('error', 'Error getting stripe cancellation page url');
        }
    })

    return {
        mutateGetStripeCancellationPageUrl: mutate,
        isPendingGetStripeCancellationPageUrl: isPending,
        isErrorGetStripeCancellationPageUrl: isError
    }
}