import { useAuth } from "@/context/AuthContext";
import { LuReceiptText } from "react-icons/lu";
import getActivePlan from "../services/getActivePlan";
import { useQuery } from "@tanstack/react-query";
import { useMemo } from "react";
import dayjs from "dayjs";
import useStripeCancellation from "../hooks/useStripeCancellation";
import SaysoButton from "@/components/SaysoButton";
import ActiveSubscriptionInformationSkeleton from "./ActiveSubscriptionInformationSkeleton";
import ActiveSubscriptionInformationError from "./ActiveSubscriptionInformationError";

export default function ActiveSubscriptionInformation() {
    const { 
        mutateGetStripeCancellationPageUrl,
        isPendingGetStripeCancellationPageUrl,
        isErrorGetStripeCancellationPageUrl 
    } = useStripeCancellation();

    const { data: activePlan, isLoading: isLoadingActivePlan, isError: isErrorActivePlan, refetch } = useQuery({
        queryKey: ['active-plan'],
        queryFn: getActivePlan
    });

    const {subscription, invoices} = activePlan || {};

    const billingPeriod = useMemo(() => {
        return subscription?.billing?.cycle === 'month' ? 'Monthly' : 'Yearly';
    }, [subscription]);

    const renewalDate = useMemo(() => {
        const billingPeriod = subscription?.billing?.period;
        if (!billingPeriod) return '';
        return dayjs(billingPeriod.end).format('MMM D, YYYY');
    },[subscription]);

    const handleCancelSubscription = () => {
        mutateGetStripeCancellationPageUrl(subscription?.plan);
    }

    if (isLoadingActivePlan) {
        return <ActiveSubscriptionInformationSkeleton />;
    }

    if (isErrorActivePlan) {
        return <ActiveSubscriptionInformationError onRetry={refetch} />;
    }

    return (
        <div className="active-plan-information-container">
            <div className="subscription-section">
                <div className="subscription-section-content">
                    <div className="plan-header">
                        <div className="plan-icon">
                           <LuReceiptText size={24} color="#FFF" />
                        </div>
                        <div className="plan-details">
                            <h2 className="plan-name">{subscription?.name}</h2>
                            <p className="plan-billing-period">{billingPeriod}</p>
                            <p className="plan-renewal-text">
                                Your subscription will auto renew on {renewalDate}.
                            </p>
                        </div>
                    </div>
                    {/* <button className="adjust-plan-button">Buy tokens</button> */}
                </div>
            </div>

            <div className="subscription-section">
                <h3 className="section-title">Invoices</h3>
                <div className="invoices-table">
                    <div className="invoices-table-header">
                        <div className="invoice-col-date">Date</div>
                        <div className="invoice-col-total">Total</div>
                        <div className="invoice-col-status">Status</div>
                        <div className="invoice-col-actions">Actions</div>
                    </div>
                    {invoices?.map((invoice) => (
                        <div key={invoice.id} className="invoice-row">
                            <div className="invoice-col-date">
                                {dayjs(invoice.created_at).format('MMM D, YYYY')}
                            </div>
                            <div className="invoice-col-total">
                                ${(invoice.amount_paid_in_cents / 100).toFixed(2)}
                            </div>
                            <div className="invoice-col-status">
                                {invoice.status.charAt(0).toUpperCase() + invoice.status.slice(1)}
                            </div>
                            <div className="invoice-col-actions">
                                <button 
                                    className="view-invoice-button"
                                    onClick={() => window.electron?.openExternal(invoice.url)}
                                >
                                    View
                                </button>
                            </div>
                        </div>
                    ))}
                </div>
            </div>

            <div className="subscription-section">
                <h3 className="section-title">Cancellation</h3>
                <div className="subscription-section-content">
                    <span className="cancel-plan-text">Cancel plan</span>
                    <SaysoButton
                        label={'Cancel'}
                        onClick={handleCancelSubscription}
                        disabled={isPendingGetStripeCancellationPageUrl}
                        loading={isPendingGetStripeCancellationPageUrl}
                        variant="error"
                    />
                </div>
            </div>
        </div>
    );
}