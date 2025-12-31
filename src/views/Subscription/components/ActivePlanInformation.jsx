import { useAuth } from "@/context/AuthContext";
import { LuReceiptText } from "react-icons/lu";

export default function ActivePlanInformation() {
    const { globalUser } = useAuth();

    // Mock data - will be replaced with actual data later
    const mockData = {
        planName: "Pro plan",
        billingPeriod: "Monthly",
        renewalDate: "Jan 30, 2026",
        paymentMethod: "Mastercard •••• 3662",
        invoices: [
            { date: "Dec 30, 2025", total: "$20.00", status: "Paid" },
            { date: "Nov 11, 2025", total: "$20.00", status: "Paid" }
        ]
    };

    return (
        <div className="active-plan-information-container">
            <div className="subscription-section">
                <div className="subscription-section-content">
                    <div className="plan-header">
                        <div className="plan-icon">
                           <LuReceiptText size={24} color="#FFF" />
                        </div>
                        <div className="plan-details">
                            <h2 className="plan-name">{mockData.planName}</h2>
                            <p className="plan-billing-period">{mockData.billingPeriod}</p>
                            <p className="plan-renewal-text">
                                Your subscription will auto renew on {mockData.renewalDate}.
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
                    {mockData.invoices.map((invoice, index) => (
                        <div key={index} className="invoice-row">
                            <div className="invoice-col-date">{invoice.date}</div>
                            <div className="invoice-col-total">{invoice.total}</div>
                            <div className="invoice-col-status">{invoice.status}</div>
                            <div className="invoice-col-actions">
                                <button className="view-invoice-button">View</button>
                            </div>
                        </div>
                    ))}
                </div>
            </div>

            <div className="subscription-section">
                <h3 className="section-title">Cancellation</h3>
                <div className="subscription-section-content">
                    <span className="cancel-plan-text">Cancel plan</span>
                    <button className="cancel-button">Cancel</button>
                </div>
            </div>
        </div>
    );
}