import { FaMinus, FaCheck } from "react-icons/fa";

export const MOCK_PRICING = {
    "id": "53c0219e-7e3d-4ac0-a2ea-441fceb704a7",
    "name": "growth",
    "description": "real-time coaching for better sales",
    "features": {
        "features": [
            {
                "id": "6b7a9a6d-8f6a-4f0f-9c9b-6d4a2b6e3e01",
                "name": "Dashboard analytics",
                "included": true
            },
            {
                "id": "2e3d7a91-0f5e-4f7e-b8c4-1c6a4e9b8d12",
                "name": "Email support",
                "included": true
            },
            {
                "id": "9a4c2f83-5b6d-4c1e-9d7e-3e2f6a1b8c45",
                "name": "Priority support",
                "included": false
            },
            {
                "id": "4f8e6c12-3a5d-4b9f-8c1e-7a2d9b6e5f78",
                "name": "Live AI coaching",
                "included": true
            },
            {
                "id": "c7e3a4f2-6b9d-4a1e-8f5c-2d6b9e7a1c34",
                "name": "API access",
                "included": false
            },
            {
                "id": "8b5d3a9e-2f6c-4e1b-9a7d-6c4f8e2b1d90",
                "name": "Custom training",
                "included": false
            },
            {
                "id": "1e9b6f4a-7d5c-4a8e-b2f9-3c6d8a5e7b21",
                "name": "Onboarding assistance",
                "included": false
            }
        ]
    },
    "includedMinutes": 1800,
    "type": "subscription",
    "purchasable": true,
    "contactLink": null,
    "pricingOptions": [
        {
            "stripePriceId": "price_1ShrpcED8sIomulPWixPydZH",
            "priceInCents": 7900,
            "currency": "usd",
            "interval": "year",
            "includedMinutesPerMonth": 1800
        },
        {
            "stripePriceId": "price_1Shro6ED8sIomulP6DVc6UER",
            "priceInCents": 85320,
            "currency": "usd",
            "interval": "month",
            "includedMinutesPerMonth": 1800
        }
    ]
}

export default function PricingComponent({ popular = false }) {
    // Using monthly pricing for now
    const monthlyPrice = MOCK_PRICING.pricingOptions.find(opt => opt.interval === 'month');
    const priceInDollars = monthlyPrice ? (monthlyPrice.priceInCents / 100).toFixed(0) : '0';
    
    return (
        <div className="pricing-card">
            {popular && <div className="pricing-card-popular-tag">Most popular</div>}

            <div className="pricing-card-header">
                <h2 className="pricing-plan-name">{MOCK_PRICING.name}</h2>
                <div className="pricing-amount-container">
                    <span className="pricing-currency">$</span>
                    <span className="pricing-amount">{priceInDollars}</span>
                    <span className="pricing-period">per month</span>
                </div>
                <button className={`pricing-cta-button ${popular ? 'pricing-cta-button-popular' : ''}`}>Get started</button>
            </div>

            <div className="pricing-features-section">
                <h3 className="pricing-features-title">FEATURES</h3>
                <p className="pricing-features-subtitle">Everything in our free plan plus....</p>
                
                <ul className="pricing-features-list">
                    {MOCK_PRICING.features.features.map((feature) => (
                        <li key={feature.id} className="pricing-feature-item">
                            <div className={`pricing-feature-icon ${feature.included ? 'included' : 'not-included'}`}>
                                {feature.included
                                    ? <FaCheck color="white" size={10}/>
                                    : <FaMinus color="white" size={10}/>
                                }
                            </div>
                            <span className="pricing-feature-text">{feature.name}</span>
                        </li>
                    ))}
                </ul>
            </div>
        </div>
    )
}