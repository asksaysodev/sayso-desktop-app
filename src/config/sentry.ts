import packageJson from '../../package.json';
import type { Breadcrumb, ErrorEvent } from '@sentry/electron/renderer';
import { redactSentryEvent, redactSentryBreadcrumb } from '@/utils/redact';

export const sentryConfig = {
  dsn: "https://2c444147dfb523db2091f3240ca27396@o4510697565585408.ingest.us.sentry.io/4510697705046016",
  // Match the main process's release (sayso-app@<version>). Without this the renderer defaults to
  // a git-SHA release, so the renderer+main events of one incident can't be correlated (SAYSO-355).
  // The main-process counterpart is electron/sentry.config.ts — keep the two literals identical.
  // Neither may derive this from package.json `name`: build.staging.js overrides it per channel.
  release: `sayso-app@${packageJson.version}`,
  beforeSend: (event: ErrorEvent) => redactSentryEvent(event),
  beforeBreadcrumb: (breadcrumb: Breadcrumb) => redactSentryBreadcrumb(breadcrumb),
}