import { test } from 'node:test';
import assert from 'node:assert/strict';
import { resolveLoginHeadline } from '../src/components/layouts/loginHeadline.ts';

test('notice takes the title slot instead of stacking under it (SAYSO-628)', () => {
    assert.deepEqual(
        resolveLoginHeadline({ notice: 'Your session has expired. Please log in again.', title: 'Welcome' }),
        { kind: 'banner', variant: 'notice', text: 'Your session has expired. Please log in again.' },
    );
});

test('error wins over notice and title', () => {
    assert.deepEqual(
        resolveLoginHeadline({ error: 'Invalid login credentials', notice: 'expired', title: 'Welcome' }),
        { kind: 'banner', variant: 'error', text: 'Invalid login credentials' },
    );
});

test('title alone renders the title', () => {
    assert.deepEqual(resolveLoginHeadline({ title: 'Welcome' }), { kind: 'title', text: 'Welcome' });
});

test('nothing renders nothing', () => {
    assert.equal(resolveLoginHeadline({ error: null, notice: null }), null);
});
