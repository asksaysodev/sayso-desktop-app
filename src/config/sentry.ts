import packageJson from '../../package.json';

export const sentryConfig = {
  dsn: "https://2c444147dfb523db2091f3240ca27396@o4510697565585408.ingest.us.sentry.io/4510697705046016",
  // Match the main process's release (sayso-app@<version>). Without this the renderer defaults to
  // a git-SHA release, so the renderer+main events of one incident can't be correlated (SAYSO-355).
  release: `sayso-app@${packageJson.version}`,
}