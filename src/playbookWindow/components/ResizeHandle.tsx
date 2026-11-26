import { useRef, useState, type PointerEvent } from 'react';

/**
 * Bottom-right resize handle for the playbook window (SAYSO-646).
 *
 * The whole square is the grab target. Pointer capture keeps events coming
 * while the cursor runs outside the window during a drag; main reads the
 * cursor and sizes the window, so this only says when the drag starts,
 * moves (at most once per frame) and ends.
 */
export default function ResizeHandle() {
    const [isDragging, setIsDragging] = useState(false);
    const draggingRef = useRef(false);
    const frameRef = useRef<number | null>(null);

    const api = window.electronAPI?.playbook;

    const onPointerDown = (e: PointerEvent<HTMLDivElement>) => {
        if (e.button !== 0 || !api) return;
        e.preventDefault();
        e.currentTarget.setPointerCapture(e.pointerId);
        api.resizeStart();
        draggingRef.current = true;
        setIsDragging(true);
    };

    const onPointerMove = () => {
        if (!draggingRef.current || !api || frameRef.current !== null) return;
        frameRef.current = requestAnimationFrame(() => {
            frameRef.current = null;
            api.resizeMove();
        });
    };

    // pointerup, pointercancel and a lost capture all end the drag.
    const endDrag = () => {
        if (!draggingRef.current) return;
        draggingRef.current = false;
        if (frameRef.current !== null) {
            cancelAnimationFrame(frameRef.current);
            frameRef.current = null;
        }
        // One last move so the window lands exactly where the pointer let go.
        api?.resizeMove();
        api?.resizeEnd();
        setIsDragging(false);
    };

    return (
        <div
            className={`playbook-resize-grip${isDragging ? ' is-dragging' : ''}`}
            onPointerDown={onPointerDown}
            onPointerMove={onPointerMove}
            onPointerUp={endDrag}
            onPointerCancel={endDrag}
            onLostPointerCapture={endDrag}
            aria-hidden="true"
        >
            <svg width="10" height="10" viewBox="0 0 10 10" fill="none">
                <path d="M9 1L1 9M9 5L5 9" stroke="currentColor" strokeWidth="1.5" strokeLinecap="round" />
            </svg>
        </div>
    );
}
