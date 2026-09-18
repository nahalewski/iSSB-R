# Modern character selection

Six HD roster tiles and six HD player portraits replace the original selection-screen pixel art. Match sprites and animation frame counts are unchanged. Brawl and Training use wider player panels, larger selection targets, readable names, new tokens, and an updated ready banner. Controller instructions move to the footer on these screens.

The portrait illustrations are existing promotional artwork from the official [Super Smash Bros. Ultimate](https://www.smashbros.com/) and [Bleach: Brave Souls](https://www.bleach-bravesouls.com/character/ichigo.html) websites. Exact source URLs are recorded in `portraits/sources.json`; artwork remains the property of its respective rights holders. These images are not AI-generated. Built-in image generation was attempted but rejected by the service.

Run `Fetch-Portraits.ps1` to fetch the source images, then `Build-Assets.ps1` to compose runtime assets. Each PNG has a density-3 `.hd` companion preserving its logical dimensions and alpha collision mask. Roster cards are 408×450 pixels (136×150 logical); player portraits are 480×360 (160×120 logical). Source images retain their original resolution in `portraits/`.

`Preview.ps1` builds `preview.png`, a static composition preview rather than a device screenshot. Runtime rules arrows, token positions, selection state and font rendering may differ.

Validation: Android `assembleDebug --offline` passed; `git diff --check` passed.
