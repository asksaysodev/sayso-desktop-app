// Run: npm test
// Lives outside electron/ so tsconfig.electron.json never compiles it into the app.
import { test } from 'node:test';
import assert from 'node:assert/strict';
import {
  classifyUpdaterError,
  isTransientUpstreamStatus,
  updaterErrorMessage,
} from '../electron/utils/transientErrors.ts';

const FRIENDLY = 'Couldn’t reach the update server. Please try again shortly.';

test('529 is a transient upstream status', () => {
  assert.equal(isTransientUpstreamStatus(529), true);
});

test('electron-updater HttpError with statusCode 529 is transient', () => {
  const err = Object.assign(new Error('HttpError: 529'), { statusCode: 529 });
  assert.equal(classifyUpdaterError(err), 'transient');
  assert.equal(updaterErrorMessage(err), FRIENDLY);
});

test('wrapped GitHubProvider 529 (SAYSO-APP-CJ shape) is transient', () => {
  const err = Object.assign(
    new Error(
      'Unable to find latest version on GitHub (https://github.com/asksaysodev/sayso-desktop-app/releases/latest), ' +
        'please ensure a production release exists: HttpError: 529 \n"method: GET url: ...\n\nData:\n "\n' +
        '    at createHttpError (...)\n<?xml version="1.0"?><feed>...</feed>',
    ),
    { code: 'ERR_UPDATER_LATEST_VERSION_NOT_FOUND' },
  );
  assert.equal(classifyUpdaterError(err), 'transient');
  assert.equal(updaterErrorMessage(err), FRIENDLY);
});

test('wrapped GitHubProvider HTTP/2 refused stream (SAYSO-APP-CJ shape) is transient', () => {
  const err = Object.assign(
    new Error(
      'Cannot parse releases feed: Error: Unable to find latest version on GitHub ' +
        '(https://github.com/asksaysodev/sayso-desktop-app/releases/latest), ' +
        'please ensure a production release exists: Error: net::ERR_HTTP2_SERVER_REFUSED_STREAM\n' +
        '    at SimpleURLLoaderWrapper.<anonymous> (...),\nXML:\n<?xml version="1.0"?><feed>...</feed>',
    ),
    { code: 'ERR_UPDATER_INVALID_RELEASE_FEED' },
  );
  assert.equal(classifyUpdaterError(err), 'transient');
  assert.equal(updaterErrorMessage(err), FRIENDLY);
});

test('a real 404 (no production release) stays fatal', () => {
  const err = Object.assign(
    new Error('Unable to find latest version on GitHub, please ensure a production release exists: HttpError: 404'),
    { code: 'ERR_UPDATER_LATEST_VERSION_NOT_FOUND' },
  );
  assert.equal(classifyUpdaterError(err), 'fatal');
});
