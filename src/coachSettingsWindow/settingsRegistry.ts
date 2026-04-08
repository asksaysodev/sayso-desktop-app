import { SidebarOptionType } from "./CoachSettingsWindowApp";

export interface SettingsRegistryEntry {
    id: string;
    label: string;
    description: string;
    keywords: string[];
    section: SidebarOptionType;
}

export const SETTINGS_REGISTRY: SettingsRegistryEntry[] = [
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
    // Auto Stop
    {
        id: 'auto-stop-time-delay',
        label: 'Time Delay',
        description: 'How long after the call starts before Sayso automatically stops the session',
        keywords: ['time', 'delay', 'auto', 'stop', 'session', 'end', 'timer', 'pause'],
        section: 'auto-stop',
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
    }
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
