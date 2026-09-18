# Modern displays and controllers

Display fixes: handle SDL_WINDOWEVENT subevents correctly; query current window and renderer output dimensions each event cycle; apply drawable/window DPI scaling; use a full drawable viewport; update input scaling before processing pointer events. Desktop windows are resizable and high-DPI aware. Android handles resizing, rotation, and attached-input changes without recreating the SDL activity and supports either landscape orientation. The 900 x 600 game canvas fits proportionally inside the available window: wide phones and different tablet shapes retain letterboxing rather than stretching characters or cropping controls.

Controllers use SDL's GameController API and a bundled SDL_GameControllerDB snapshot downloaded 2026-09-17. Source: https://github.com/mdqinc/SDL_GameControllerDB ; license accompanies the database in Assets. SDL 2.0.14 introduced DualSense / Xbox Series HIDAPI support; use a recent SDL2 runtime when packaging. Existing repository SDL sources already contain original Kishi and PS5 mappings; the external database expands model coverage. Bluetooth and USB follow the same mapped input path. OS pairing and driver support are required. No controller hardware has been tested here.

Up to four attached mapped controllers occupy stable player slots P1-P4 while connected. Configure those slots as human players in character select. A replacement controller takes the lowest free slot. Unplugging releases held controls. Input is cleared when the window loses focus. Keyboard and gamepad gameplay input are combined, so releasing one source does not release an action still held on the other.

| Control | Gameplay | Menus |
| --- | --- | --- |
| Left stick / D-pad | Move / aim attacks | Move visible cursor |
| A / Cross | Standard attack | Select; hold while moving to drag tokens |
| B / Circle | Special attack | Back |
| X / Square or Y / Triangle | Jump | — |
| Shoulder buttons / triggers | Grab | — |
| Start / Options / Menu | Pause / resume | — |

The controller cursor uses existing mouse or touch paths, covering menu buttons, character tokens, stage selection, rules, settings, and result-screen buttons. It does not add a separate directional-focus system. A shared cursor accepts input from any attached controller; gameplay remains independent per player. Native OS dialogs and keyboard-binding capture still use their original system/keyboard input.

Validation: gamepad-test.cpp runs without hardware using mocked SDL devices, checking four independent slots, fifth-device overflow, repeated polls/refcounts, deadzone, button edges, focus loss, unplug/replacement releases, jump, triggers, pause and cleanup. Compile with `clang++ -std=c++17 -I External/SDL/include tools/modern-input/gamepad-test.cpp -o gamepad-test.exe` (prefix commands with rtk proxy in this workspace). Changed C++ translation units pass syntax checking. Full platform builds and physical phone/tablet/controller tests remain outstanding.

Manual checks for a packaged build: resize repeatedly; move between different-DPI monitors; rotate a phone; check touch alignment at all four canvas corners; pair or plug Kishi, DualSense and Xbox controllers; drag each character token; choose a stage; test every character's movement/attacks/jump/grab; pause/resume; disconnect while moving; reconnect; confirm P2-P4 do not control P1. Include gamecontrollerdb.txt and its license when copying Assets, plus the existing HD asset companions.
