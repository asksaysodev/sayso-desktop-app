// Sentry reporting for the Cue streaming layer (SAYSO-348).
//
// Every capture in electron/streaming/ goes through here — nothing in this
// folder should call Sentry directly. Transience is read off
// isTransientNetworkError so the pattern list stays defined exactly once.

const Sentry = require('@sentry/electron/main');
const { isTransientNetworkError } = require('../utils/transientErrors');

/**
 * @typedef {Object} StreamingFailure
 * @property {string} speaker
 * @property {string} endpoint
 * @property {string} code
 */

/**
 * @typedef {Object} StreamingContext
 * @property {string} [speaker]
 * @property {string} [endpoint]
 * @property {string} [sessionId]
 * @property {string} [stage]
 * @property {StreamingFailure[]} [failures]
 * @property {*} [cause]
 */

/**
 * Stable grouping key for a streaming failure.
 *
 * @param {*} err
 * @returns {string}
 */
function streamingErrorCode(err) {
  if (typeof err?.code === 'string' && err.code) {
    return err.code;
  }
  const handshake = /Unexpected server response:\s*(\d{3})/.exec(err?.message ?? '');
  if (handshake) {
    return `HTTP_${handshake[1]}`;
  }
  return 'UNKNOWN';
}

/**
 * @param {*} scope
 * @param {StreamingContext} context
 * @param {string} code
 */
function applyScope(scope, context, code) {
  const { speaker = null, endpoint = null, sessionId = null, stage = 'unknown', failures = null } = context;

  scope.setContext('cue-streaming', { speaker, endpoint, sessionId, stage, code, failures });
  scope.setTag('streaming.stage', stage);
  scope.setTag('streaming.code', code);
  if (speaker) {
    scope.setTag('streaming.speaker', speaker);
  }

  scope.setFingerprint(['cue-streaming', stage, code]);
}

/**
 * Report a streaming exception unless it is environmental noise.
 *
 * @param {*} err
 * @param {StreamingContext} [context]
 * @returns {boolean} true if it reached Sentry.
 */
function reportStreamingError(err, context = {}) {
  if (isTransientNetworkError(err)) {
    return false;
  }

  const code = streamingErrorCode(err);
  Sentry.withScope((scope) => {
    applyScope(scope, context, code);
    Sentry.captureException(err);
  });
  return true;
}

/**
 * Report a streaming condition with no Error of its own — the max-send-failures
 * counters. `context.cause` is the socket's last error; when that is transient
 * the message is a symptom of the same blip and is suppressed with it.
 *
 * @param {string} message
 * @param {StreamingContext} [context]
 * @returns {boolean} true if it reached Sentry.
 */
function reportStreamingMessage(message, context = {}) {
  if (context.cause && isTransientNetworkError(context.cause)) {
    return false;
  }

  const code = streamingErrorCode(context.cause);
  Sentry.withScope((scope) => {
    applyScope(scope, context, code);
    Sentry.captureMessage(message, 'error');
  });
  return true;
}

module.exports = {
  reportStreamingError,
  reportStreamingMessage,
  streamingErrorCode
};
