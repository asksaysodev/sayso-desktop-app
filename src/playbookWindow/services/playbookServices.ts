import apiClient from '@/config/axios';
import {
    GetPlaybooksResponse,
    Playbook,
    UploadPlaybookResponse,
} from '@/playbookWindow/types';

export async function getPlaybooks(): Promise<Playbook[]> {
    const response = await apiClient.get<GetPlaybooksResponse>('/playbooks');
    return response.data.playbooks;
}

export async function uploadPlaybook(file: File, alias: string): Promise<Playbook> {
    const formData = new FormData();
    formData.append('file', file);
    formData.append('alias', alias);
    const response = await apiClient.post<UploadPlaybookResponse>('/playbooks', formData);
    return response.data.playbook;
}

export async function deletePlaybook(id: string): Promise<void> {
    await apiClient.delete(`/playbooks/${id}`);
}

export async function updatePlaybookAlias(id: string, alias: string): Promise<void> {
    await apiClient.patch(`/playbooks/${id}`, { alias });
}

export async function setDefaultPlaybook(id: string): Promise<{ success: true; default_playbook_id: string }> {
    const response = await apiClient.patch('/playbooks/default', { playbook_id: id });
    return response.data;
}

export async function updatePlaybookOrder(order: string[]): Promise<{ success: true; playbook_order: string[] }> {
    const response = await apiClient.patch('/playbooks/order', { order });
    return response.data;
}
