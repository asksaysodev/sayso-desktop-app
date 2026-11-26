// Decides what fills LoginLayout's single headline slot. Banners take the title's place
// instead of stacking under it, so the layout is the same height whatever is shown
// (SAYSO-628: a stacked notice overflowed the fixed 380×560 splash on Windows).
// Import-free so test/ can load it directly.

export type LoginBannerVariant = 'error' | 'notice';

export type LoginHeadline =
    | { kind: 'banner'; variant: LoginBannerVariant; text: string }
    | { kind: 'title'; text: string }
    | null;

export function resolveLoginHeadline({ error, notice, title }: {
    error?: string | null;
    notice?: string | null;
    title?: string;
}): LoginHeadline {
    if (error) return { kind: 'banner', variant: 'error', text: error };
    if (notice) return { kind: 'banner', variant: 'notice', text: notice };
    if (title) return { kind: 'title', text: title };
    return null;
}
