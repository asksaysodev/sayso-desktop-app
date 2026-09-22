; Custom NSIS include, picked up by electron-builder via build.nsis.include.
; Path is relative to directories.buildResources ("assets").
;
; Why this file exists: Launch at Login is implemented with Electron's
; app.setLoginItemSettings(), which on Windows writes two HKCU registry values —
; the Run entry itself and the Task Manager approval flag. electron-builder's
; uninstaller removes only its own Uninstall\ and Software\{GUID} keys and never
; touches CurrentVersion\Run, so uninstalling with the toggle on used to leave an
; orphan startup entry pointing at a deleted exe, listed in Task Manager forever.

!macro customUnInstall
  ; The installer runs the OLD uninstaller silently with --updated on every
  ; update (app-builder-lib/templates/nsis/include/installUtil.nsh). Without this
  ; guard, updating would silently disable Launch at Login for every user who had
  ; it on. Only a real uninstall should clean up.
  ${ifNot} ${isUpdated}
    ; Current value name: Electron names the Run value after the AppUserModelId,
    ; which main.ts pins to build.appId — the same string NSIS gets as APP_ID.
    DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "${APP_ID}"
    DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Explorer\StartupApproved\Run" "${APP_ID}"

    ; Pre-AUMID builds (before SAYSO-453) registered under Electron's derived
    ; default, "electron.app.<app.name>". app.name is "Sayso" on production and
    ; the package name on staging, which is what APP_PACKAGE_NAME holds. Runtime
    ; migration rehomes these on launch, but an uninstall without an intervening
    ; launch would still strand them.
    DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "electron.app.Sayso"
    DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Explorer\StartupApproved\Run" "electron.app.Sayso"
    !ifdef APP_PACKAGE_NAME
      DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "electron.app.${APP_PACKAGE_NAME}"
      DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Explorer\StartupApproved\Run" "electron.app.${APP_PACKAGE_NAME}"
    !endif
  ${endIf}
!macroend
