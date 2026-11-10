// How the mic stall watchdog notices a wake whose `resume` / `unlock-screen` event never arrived.
// Pure, no electron, so test/micStallIdle.test.mts can replay a real sleep timeline.

/**
 * Reports input that happened after the last look at the OS idle counter. Idle time only ever goes
 * down when there is new input, so that drop is the signal. A plain "idle < N seconds" check can't
 * be used: seconds after a suspend it is still satisfied by the input from just before the Mac
 * slept (Sentry SAYSO-APP-CM, 11-09: restarts burned 15s into a sleep, watchdog gave up asleep).
 */
export function createNewInputDetector(getIdleSeconds: () => number) {
  let lastIdleS = 0;
  return {
    /** Call when the watchdog pauses: only input after this point counts. */
    baseline(): void {
      lastIdleS = getIdleSeconds();
    },
    sawNewInput(): boolean {
      const idleS = getIdleSeconds();
      const sawInput = idleS < lastIdleS;
      lastIdleS = idleS;
      return sawInput;
    },
  };
}
