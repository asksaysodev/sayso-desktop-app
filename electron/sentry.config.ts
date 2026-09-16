const pkg = require('../package.json') as { build_env?: string; version: string };

export default {
  dsn: "https://2c444147dfb523db2091f3240ca27396@o4510697565585408.ingest.us.sentry.io/4510697705046016",
  environment: pkg.build_env || 'production',
  // Pinned, not derived. @sentry/electron defaults release to `${app.name}@${version}`, read at
  // init — and main.ts calls Sentry.init *before* the IS_STAGING app.setName(), so it would pick up
  // the packaged package.json `name`, which build.staging.js sets to sayso-app-staging. Must stay
  // identical to src/config/sentry.ts or main+renderer events of one incident split across two
  // releases (SAYSO-355). Channel separation is `environment`'s job, not `release`'s.
  release: `sayso-app@${pkg.version}`,
};
