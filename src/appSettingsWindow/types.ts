import { AccountStatus } from "@/types/user";

export type SettingsPanel = 'personal' | 'company' | 'files' | 'connections' | 'security' | 'coach';

export type MemberStatusFilterValue = AccountStatus;
export type MemberStatusFilter = { key: 'status'; value: MemberStatusFilterValue };
export type MemberActiveFilter = MemberStatusFilter;
export enum SettingsPanelEnum {
    PERSONAL = 'personal',
    COMPANY = 'company',
    FILES = 'files',
    CONNECTIONS = 'connections',
    SECURITY = 'security',
    COACH = 'coach'
}

export interface PostBufferTimeResponse {
    success: true;
    insight_buffer_time_ms: number;
}
export type AccessibilityFontSizeType = 's' | 'm' | 'l';
export interface GetCoachSettingsResponse {
    account_id: string;
    insight_buffer_time_ms: number;
    cue_mode: CueMode;
    auto_stop_delay_minutes: number;
    font_size: AccessibilityFontSizeType;
    default_playbook_id: string | null;
    open_last_used: boolean;
}

export interface PostOpenLastUsedResponse {
    success: true;
    openLastUsed: boolean;
}

export type CueMode = 'condensed' | 'full';

export interface PostAutoStopTimeDelayResponse {
    success: boolean;
    autoStopDelay: number
}