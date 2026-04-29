import { SidebarOptionType } from "./AppSettingsWindowApp";

export interface SettingsRegistryEntry {
    id: string;
    label: string;
    description: string;
    keywords: string[];
    section: SidebarOptionType;
}

export const SETTINGS_REGISTRY: SettingsRegistryEntry[] = [
    // General
    {
        id: 'general-tab',
        label: 'General',
        description: 'General app settings',
        keywords: ['general', 'tab', 'app', 'settings'],
        section: 'general',
    },
    {
        id: 'launch-at-login',
        label: 'Launch at Login',
        description: 'Automatically start Sayso in the tray when your Mac starts',
        keywords: ['launch', 'login', 'startup', 'autostart', 'auto start', 'start', 'mac', 'tray', 'general'],
        section: 'general',
    },
    {
        id: 'auto-stop-time-delay',
        label: 'Auto Stop',
        description: 'How long after the call starts before Sayso automatically stops the session',
        keywords: ['auto stop', 'auto', 'stop', 'time', 'delay', 'session', 'end', 'timer', 'pause', 'general'],
        section: 'general',
    },
    // Cue
    {
        id: 'cue-mode',
        label: 'Mode',
        description: 'Choose how insights are displayed during your call',
        keywords: ['mode', 'display', 'insights', 'full', 'condensed', 'cue'],
        section: 'cue',
    },
    {
        id: 'cue-buffer-time',
        label: 'Buffer Time',
        description: 'How long to wait after the call starts before Sayso begins showing insights',
        keywords: ['buffer', 'time', 'delay', 'wait', 'start', 'insights', 'cue'],
        section: 'cue',
    },
    // Audio
    // {
    //     id: 'audio-input-device',
    //     label: 'Input Device',
    //     description: 'The microphone used during your call.',
    //     keywords: ['input', 'device', 'microphone', 'mic', 'audio', 'hardware'],
    //     section: 'audio',
    // },
    // {
    //     id: 'audio-input-level',
    //     label: 'Input Level',
    //     description: 'Live input level from your microphone.',
    //     keywords: ['input', 'level', 'volume', 'microphone', 'mic', 'audio', 'meter'],
    //     section: 'audio',
    // },
    // Scripts
    // {
    //     id: 'scripts-active',
    //     label: 'Active',
    //     description: 'Enable or disable script guidance during your call.',
    //     keywords: ['active', 'enable', 'disable', 'script', 'toggle', 'on', 'off'],
    //     section: 'scripts',
    // },
    // {
    //     id: 'scripts-script',
    //     label: 'Script',
    //     description: 'Choose which script to use during your call.',
    //     keywords: ['script', 'select', 'choose', 'call', 'guidance'],
    //     section: 'scripts',
    // },

    // Accessibility
    {
        id: 'accessibility-settings',
        label: 'Accessibility',
        description: 'Choose the font size to use',
        keywords: ['accessibility', 'font', 'size', 'adjust', 'scale', 'small','medium', 'large'],
        section: 'accessibility',
    },
    // Playbooks
    {
        id: 'playbooks-upload',
        label: 'Upload',
        description: 'Upload a new playbook (PDF, DOCX, TXT)',
        keywords: ['playbook', 'playbooks', 'upload', 'add', 'new', 'file', 'drop', 'browse', 'pdf', 'docx', 'txt', 'alias', 'script', 'scripts'],
        section: 'playbooks',
    },
    {
        id: 'playbooks-scripts',
        label: 'Scripts',
        description: 'Browse, refresh, and delete your playbooks',
        keywords: ['playbook', 'playbooks', 'scripts', 'list', 'manage', 'refresh', 'delete', 'remove'],
        section: 'playbooks',
    },
];

export function searchSettings(query: string): SettingsRegistryEntry[] {
    const q = query.toLowerCase().trim();
    if (!q) return [];

    return SETTINGS_REGISTRY.filter(entry =>
        entry.label.toLowerCase().includes(q) ||
        entry.description.toLowerCase().includes(q) ||
        entry.keywords.some(k => k.includes(q))
    );
}
