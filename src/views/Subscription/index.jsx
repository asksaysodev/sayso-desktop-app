import { useState } from "react";
import ViewLayout from "@/components/layouts/ViewLayout";
import "./styles.css";
import BillingTabSelector from "./components/BillingTabSelector";
import PricingComponent from "./components/PricingComponent";

export default function Subscription() {
	// monthly or annual
	const [selectedBillingTab, setSelectedBillingTab] = useState('monthly');

	return (
		<ViewLayout title="Subscription" scrollable>
			<BillingTabSelector
				selectedBillingTab={selectedBillingTab}
				setSelectedBillingTab={setSelectedBillingTab}
			/>
			<div className="pricing-components-grid">
				<PricingComponent />
				<PricingComponent popular />
				<PricingComponent />
			</div>
		</ViewLayout>
  );
}