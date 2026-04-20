import { useState, useEffect } from 'react';
import { AccessibilityFontSizeType } from "@/appSettingsWindow/types";

function getDocumentFontSize(): AccessibilityFontSizeType {
    return (document.documentElement.dataset.fontSize ?? 's') as AccessibilityFontSizeType;
}

/**
 * Returns the active font size key ('s' | 'm' | 'l') for the coach window.
 *
 * Reactive — re-renders consumers when the font size changes.
 *
 * Watches the `data-font-size` attribute on `<html>` via MutationObserver.
 * The attribute is set synchronously from the URL param before React renders
 * (no jump), and kept in sync by the global `font-size-changed` IPC listener
 * in index.tsx. This hook observes that DOM change rather than subscribing to
 * IPC directly, avoiding listener conflicts with the global handler.
 *
 * Defaults to 's' if no attribute is present.
 */
export default function useFontSize() {
    const [fontSize, setFontSize] = useState<AccessibilityFontSizeType>(getDocumentFontSize);

    useEffect(() => {
        const observer = new MutationObserver(() => {
            setFontSize(getDocumentFontSize());
        });
        observer.observe(document.documentElement, {
            attributes: true,
            attributeFilter: ['data-font-size'],
        });
        return () => observer.disconnect();
    }, []);

    return fontSize;
}
