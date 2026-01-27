import { useMutation } from "@tanstack/react-query";
import getStripeCheckoutPageUrl from "../services/getStripeCheckoutPageUrl";
import { useToast } from "@/context/ToastContext";
import * as Sentry from "@sentry/electron/renderer";

export default function useStripeCheckout() {
    const { showToast } = useToast();

    const { mutate, isPending, isError} = useMutation({
        mutationFn: getStripeCheckoutPageUrl,
        onSuccess: (data) => {
            window.electron?.openExternal(data.url);
        },
        onError: (error) => {
            Sentry.captureException(error);
            showToast('error', 'Error getting stripe checkout page url');
        }
    })

    return {
        mutateGetStripeCheckoutPageUrl: mutate,
        isPendingGetStripeCheckoutPageUrl: isPending,
        isErrorGetStripeCheckoutPageUrl: isError
    }
}