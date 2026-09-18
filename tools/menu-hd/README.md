# HD menu refresh

The startup version dialog requesting donations is removed from Game.cpp, together with its unused message field. Preference version migration remains intact. The optional donation links within Other settings are unchanged.

The menu uses newly generated artwork inspired by Super Smash Bros. Ultimate: a large red Smash tile (the original Group destination), blue Solo, orange Options, and purple Local Wireless on iOS. Group, Solo, and Options submenus have matching Brawl, Rules, Training, Controls, Sound, Display, and Other tiles. Back is also refreshed. All existing destinations remain; wireless is still iOS-only.

24 button PNGs (12 normal/selected pairs) use 3x logical resolution. The shared bg2 menu background is 1920 x 1280 (4x). Labels are typeset during asset packaging; selected tiles have yellow and white outlines. The .png.hd companions use the engine density support introduced with the Ichigo update and must ship alongside the PNGs. Opaque tile masks match their rectangular hit areas. No Ichigo assets or animation counts were changed in this refresh.

Raw image generation outputs are in sources/. Exact prompts and provenance are in generation-log.json. Build-Menu.ps1 reproduces the installed art using System.Drawing on Windows. Run with `rtk proxy powershell -NoProfile -File tools/menu-hd/Build-Menu.ps1` from the repository root.

Verify-Menu.ps1 checks source menu coordinates against logical asset sizes, paired state dimensions, metadata lengths, and nonoverlapping bounds. It creates desktop composition previews for MainMenu, GroupMenu, SoloMenu, and OptionsMenu. These are asset/layout previews, not runtime screenshots. Bluetooth's bounds are checked but its tile is omitted from desktop previews.

Validation: all 11 menu destinations pass asset/layout checks. Game.cpp, Preferences.cpp, and MenuClasses.cpp pass clang++ C++17 syntax checking against bundled SDL headers (legacy SDL pragma-pack warnings). Git whitespace check passes with core.whitespace=cr-at-eol. A full executable build and interactive play test have not been performed. Rebuild and package Assets, including .hd companions, to use the changes.
