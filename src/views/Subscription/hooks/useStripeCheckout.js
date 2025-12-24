import { useMutation } from "@tanstack/react-query";
import getStripeCheckoutPageUrl from "../services/getStripeCheckoutPageUrl";

export default function useStripeCheckout() {
    const { mutate, isPending, isError} = useMutation({
        mutationFn: getStripeCheckoutPageUrl,
        onSuccess: (data) => {
            window.electron?.openExternal(data.url);
        },
        onError: (error) => {
            showToast('error', 'Error getting stripe checkout page url');
        }
    })

    return {
        mutateGetStripeCheckoutPageUrl: mutate,
        isPendingGetStripeCheckoutPageUrl: isPending,
        isErrorGetStripeCheckoutPageUrl: isError
    }
}