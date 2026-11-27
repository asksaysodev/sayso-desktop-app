import { test } from 'node:test';
import assert from 'node:assert/strict';
import { noteCoachOpened, noteCoachClosed, getCoachOpener } from '../electron/utils/coachOpener.ts';

test('nothing recorded reads as other', () => {
  noteCoachClosed();
  assert.equal(getCoachOpener(), 'other');
});

test('the opener holds until the window closes', () => {
  noteCoachOpened('shortcut');
  assert.equal(getCoachOpener(), 'shortcut');
  // A stop + start in the same window, or a focus of the open coach: no new note, same opener.
  assert.equal(getCoachOpener(), 'shortcut');
  noteCoachClosed();
  assert.equal(getCoachOpener(), 'other');
});

test('the prompt promoted by another opener takes that opener, not call_prompt', () => {
  noteCoachClosed();
  noteCoachOpened('tray');
  assert.equal(getCoachOpener(), 'tray');
});

test('a new window after a close gets its own opener', () => {
  noteCoachOpened('call_prompt');
  noteCoachClosed();
  noteCoachOpened('web_app');
  assert.equal(getCoachOpener(), 'web_app');
});
