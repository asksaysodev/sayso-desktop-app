import TabSelector from '../../../components/TabSelector';
import '../../../components/TabSelector.css';

const BillingTabOptions = [
	{ value: 'month', label: 'Monthly billing' },
	{ value: 'year', label: 'Annual billing' },
]

export default function BillingTabSelector({ selectedBillingTab, setSelectedBillingTab }) {
	return (
		<div className="billing-tab-selector-container">
			<TabSelector 
				tabs={BillingTabOptions}
				selectedValue={selectedBillingTab}
				onChange={setSelectedBillingTab}
			/>
		</div>
	)
}