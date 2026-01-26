export interface GetAccountSubscriptionResponse {
    subscription: {
      plan: string;
      name: string;
      description: string;
      interval: string;
      price: {
        priceInCents: number;
        currency: string;
      };
      includedMinutes: {
        plan: number;
        trial: number;
      };
      status: string;
      billing: {
        cycle: string;
        period: {
          start: string | null;
          end: string | null;
        };
        lastInvoicePaidAt: string | null;
      };
      cancelledAt: string | null;
    };
    invoices: Array<{
      account_id: string;
      id: string | null;
      description: string | null;
      created_at: string | null;
      period_start: string | null;
      period_end: string | null;
      currency: string;
      amount_due_in_cents: number | null;
      amount_paid_in_cents: number | null;
      status: string | null;
      url: string | null;
      pdf_url: string | null;
    }>;
}

export interface GetStripeCancellationPageUrlResponse {
    data: {
        url: string;
    };
}

export interface GetStripeCheckoutPageUrlResponse {
    data: {
        url: string;
    };
}

export interface GetPricingPlansResponse extends Array<PricingPlan> {}

export interface PricingPlan {
  id: string;
  name: string;
  description: string;
  features: unknown;
  includedMinutes: number;
  type: string;
  purchasable: boolean;
  contactLink: string | null;
  pricingOptions: PricingOption[];
  popular: boolean;
  hasTrial: boolean;
  trialDays: number | null;
  trialIncludedMinutes: number | null;
}

export interface PricingOption {
  stripePriceId: string;
  priceInCents: number;
  currency: string;
  interval: string; // 'month' or 'year' i think $FixTS
  includedMinutesPerMonth: number;
}