iSSB
====

A cross-platform "Super Smash Bros. Brawl" fan-game

This project uses git submodules, so in order to download it, you'll have to clone it recursively:

```bash
git clone --recursive https://github.com/lufinkey/iSSB
```

## Screenshots

### Character Select
![Character Select Screen](docs/screenshot_character_select.png)
Modern character selection screen with support for multiple players and gamepad controls.

### Multiplayer
![Multiplayer/Bluetooth Battledome](docs/screenshot_multiplayer.png)
Bluetooth multiplayer lobby system for connecting with friends.

### Settings
![Game Rules & Settings](docs/screenshot_settings.png)
Comprehensive game rules configuration including game modes, item frequency, and stage hazards.

## Development Notes

This project has been in development for **several years**, evolving from a desktop fan-game into a fully-featured Android experience. While AI assistance was utilized during the recent Android modernization and UI overhaul, the core game engine, character mechanics, physics system, and original gameplay implementations represent years of dedicated manual development work.

**Recent Updates:**
- ✅ Modern WebView-based menu system
- ✅ Full gamepad/controller support
- ✅ True fullscreen rendering on Android devices
- ✅ Adaptive UI scaling for all screen sizes
- ✅ Character select, stage select, and settings menus
- ✅ Bluetooth multiplayer support
- ✅ Paper Mario character integration

The game features a custom C++ engine built on SDL2, with hand-crafted character animations, attack systems, and stage mechanics developed over multiple years of iteration.