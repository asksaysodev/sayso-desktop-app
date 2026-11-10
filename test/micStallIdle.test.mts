// Run: npm test
import { test } from 'node:test';
import assert from 'node:assert/strict';
import { createNewInputDetector } from '../electron/audio/micStallIdle.ts';

function detectorWith(idleSeconds: number[]) {
  let i = 0;
  return createNewInputDetector(() => idleSeconds[Math.min(i++, idleSeconds.length - 1)]);
}

test('input from just before the sleep is not a wake (SAYSO-APP-CM, 11-09)', () => {
  // Paused 5s after her last input; checks at +15s and +23s with no new input.
  const detector = detectorWith([5, 20, 28]);
  detector.baseline();
  assert.equal(detector.sawNewInput(), false);
  assert.equal(detector.sawNewInput(), false);
});

test('input right before the sleep is not a wake either', () => {
  const detector = detectorWith([0, 8, 16]);
  detector.baseline();
  assert.equal(detector.sawNewInput(), false);
  assert.equal(detector.sawNewInput(), false);
});

test('an idle counter that stops during real sleep is not a wake', () => {
  const detector = detectorWith([5, 5, 5]);
  detector.baseline();
  assert.equal(detector.sawNewInput(), false);
  assert.equal(detector.sawNewInput(), false);
});

test('input after the pause is a wake, even on an idle-for-minutes machine', () => {
  const detector = detectorWith([300, 2]);
  detector.baseline();
  assert.equal(detector.sawNewInput(), true);
});

test('input between two checks is a wake', () => {
  const detector = detectorWith([0, 8, 16, 3]);
  detector.baseline();
  assert.equal(detector.sawNewInput(), false);
  assert.equal(detector.sawNewInput(), false);
  assert.equal(detector.sawNewInput(), true);
});
