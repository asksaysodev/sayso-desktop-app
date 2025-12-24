const BillingTabOptions = [
	{ value: 'month', label: 'Monthly billing' },
	{ value: 'year', label: 'Annual billing' },
]

export default function BillingTabSelector({ selectedBillingTab, setSelectedBillingTab }) {
	return (
		<div className="billing-tab-selector-container">
			{BillingTabOptions.map(({ value, label }) => {
				const isActive = selectedBillingTab === value;
				return (
					<button 
						key={value} 
						onClick={() => setSelectedBillingTab(value)} 
						className={`billing-tab-option ${isActive ? 'active' : ''}`}
					>
						<p>{label}</p>
					</button>
				)
			})}
		</div>
	)
}