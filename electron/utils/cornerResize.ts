// Size math for the playbook window's bottom-right resize handle (SAYSO-646).
//
// Kept free of imports so `node --test` can load it directly.

export interface Point { x: number; y: number }
export interface Rect { x: number; y: number; width: number; height: number }

/**
 * Bounds for a window being resized from its bottom-right corner.
 *
 * The top-left corner stays put; the size follows the cursor's travel since
 * the drag started, never going under the minimum size and never pushing the
 * right/bottom edge past the work area. The minimum wins when the window
 * already sits too close to the work area's edge for it.
 */
export function cornerResizeBounds(
    start: Rect,
    cursorStart: Point,
    cursorNow: Point,
    minSize: { width: number; height: number },
    workArea: Rect,
): Rect {
    const maxWidth = workArea.x + workArea.width - start.x;
    const maxHeight = workArea.y + workArea.height - start.y;

    const width = start.width + (cursorNow.x - cursorStart.x);
    const height = start.height + (cursorNow.y - cursorStart.y);

    return {
        x: start.x,
        y: start.y,
        width: Math.round(Math.max(minSize.width, Math.min(width, maxWidth))),
        height: Math.round(Math.max(minSize.height, Math.min(height, maxHeight))),
    };
}
