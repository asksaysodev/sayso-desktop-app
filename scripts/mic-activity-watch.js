#!/usr/bin/env node
// Logs mic-activity events as JSON lines, for the SAYSO-528 browser matrix.
//
// The addon has to be built for plain Node first (not Electron's ABI):
//   cd electron/mic-activity && npx node-gyp rebuild
// then: node scripts/mic-activity-watch.js
// Rebuild for Electron (npm run rebuild-mic-activity) before running the app.
const micActivity = require('../electron/mic-activity');

const startedAt = Date.now();
console.log(JSON.stringify({ mode: micActivity.mode(), watcherPid: process.pid }));

micActivity.start((event) => {
  console.log(JSON.stringify({ ...event, sinceStartMs: event.ts - startedAt }));
});

const shutdown = () => {
  micActivity.stop();
  process.exit(0);
};
process.on('SIGINT', shutdown);
process.on('SIGTERM', shutdown);
