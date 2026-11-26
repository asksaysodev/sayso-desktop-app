import { test } from 'node:test';
import assert from 'node:assert/strict';
import { cornerResizeBounds } from '../electron/utils/cornerResize.ts';

const start = { x: 100, y: 100, width: 520, height: 500 };
const min = { width: 360, height: 300 };
const workArea = { x: 0, y: 25, width: 1440, height: 875 };
const grab = { x: 610, y: 590 };

test('dragging right/down grows the window, top-left stays put (SAYSO-646)', () => {
    assert.deepEqual(
        cornerResizeBounds(start, grab, { x: 650, y: 640 }, min, workArea),
        { x: 100, y: 100, width: 560, height: 550 },
    );
});

test('dragging left/up shrinks the window', () => {
    assert.deepEqual(
        cornerResizeBounds(start, grab, { x: 560, y: 540 }, min, workArea),
        { x: 100, y: 100, width: 470, height: 450 },
    );
});

test('never goes under the minimum size', () => {
    assert.deepEqual(
        cornerResizeBounds(start, grab, { x: 0, y: 0 }, min, workArea),
        { x: 100, y: 100, width: 360, height: 300 },
    );
});

test('never pushes the right/bottom edge past the work area', () => {
    assert.deepEqual(
        cornerResizeBounds(start, grab, { x: 5000, y: 5000 }, min, workArea),
        { x: 100, y: 100, width: 1340, height: 800 },
    );
});

test('minimum wins over the work area when the window is near the edge', () => {
    const nearEdge = { x: 1200, y: 700, width: 360, height: 300 };
    assert.deepEqual(
        cornerResizeBounds(nearEdge, grab, { x: 700, y: 700 }, min, workArea),
        { x: 1200, y: 700, width: 360, height: 300 },
    );
});

test('fractional cursor positions round to whole pixels', () => {
    const r = cornerResizeBounds(start, grab, { x: 620.6, y: 600.4 }, min, workArea);
    assert.equal(r.width, 531);
    assert.equal(r.height, 510);
});
