# HD loading and level selection

`Build-Assets.ps1` builds the startup/match loading graphic at 2700x1800 and six 780x540 level cards using the generated stage artwork from `tools/battle-hd`. Every graphic has density-3 logical-size metadata. These are native UI compositions of the previously generated artwork, not newly generated illustrations.

Startup loading fills the actual window, with a bounded progress bar. The stage-selection grid has three columns and two rows, with larger touch targets, readable names, a red hover border and controller hints in the footer. Existing stage identifiers and selection actions are retained. The loading overlay is opaque so the roster does not show through it.

All menus map the existing 900x600 canvas independently onto the window width and height. This removes letterboxing without cropping buttons, but widens artwork on ultrawide screens. Mouse, current/previous touch points and controller pointer injection use matching per-axis transforms. Battles disable this mode and retain their expanded, uniform-scale camera; leaving a battle restores menu fill mode. Startup and match loading also fill both axes.

Device verification: installed on the Razer Edge 5G and Pixel 9 Pro Fold. A Razer built-in-display screenshot confirmed a 2400x1080 menu with no black side bars, followed by a full-width running battle with Kishi touch controls hidden. Device activity changed during testing, so exhaustive individual-menu navigation and controller-pointer edge testing remain manual checks.

Rebuild with `Build-Assets.ps1`; create the static layout preview with `Preview.ps1`. The preview is not a device screenshot. Validation: Android native/Java `assembleDebug --offline` and `git diff --check` pass.
