"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const platformModules = {
    darwin: './MacPermissionsProvider',
    win32: './WindowsPermissionsProvider',
};
const modulePath = platformModules[process.platform];
if (!modulePath) {
    throw new Error(`[Permissions] Unsupported platform: ${process.platform}`);
}
const provider = require(modulePath).default;
exports.default = provider;
//# sourceMappingURL=index.js.map