import { useMutation } from "@tanstack/react-query";
import getStripeCancellationPageUrl from "../services/getStripeCancellationPageUrl";

export default function useStripeCancellation() {
    const { mutate, isPending, isError} = useMutation({
        mutationFn: getStripeCancellationPageUrl,
        onSuccess: (data) => {
            window.electron?.openExternal(data.url);
        },
        onError: (error) => {
            showToast('error', 'Error getting stripe cancellation page url');
        }
    })

    return {
        mutateGetStripeCancellationPageUrl: mutate,
        isPendingGetStripeCancellationPageUrl: isPending,
        isErrorGetStripeCancellationPageUrl: isError
    }
}