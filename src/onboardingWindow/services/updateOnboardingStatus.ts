import apiClient from '@/config/axios';

export default async function updateOnboardingStatus(status: 'complete' | 'dismissed'): Promise<void> {
    await apiClient.put('/accounts/update-account', { updateData: { onboarding_status: status } });
}
