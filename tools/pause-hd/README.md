# HD battle pause menu

Run `Build-Assets.ps1` to create three 1080x216 button textures with density-3 metadata and 360x72 logical hit areas. Native typography and simple geometric icons match the existing HD menus. Resume, End Match and Exit Game retain their existing actions.

The pause panel is centered using the current battle viewport dimensions, with a full-screen translucent backdrop and a PAUSED heading. Buttons are opaque for legibility; their original press feedback remains intact.

Validation: Android debug build and whitespace check passed. Installed on Razer Edge 5G and Pixel 9 Pro Fold. Runtime pause-panel verification requires a paused match on the device.
