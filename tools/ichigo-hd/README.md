# Ichigo HD sprites

54 character animation sheets were redrawn with the built-in image-generation tool and installed at 4x texture resolution in `Assets/Images/Game/Characters/Ichigo/`.

The 51 gameplay sheets contain 217 frames. The three results-screen sheets contain 10 frames. All 227 frame slots, animation names, frame rates, and the original 60 PNG filenames in the character folder are retained. Standalone projectiles, the stock icon, symbol, and menu portraits were not part of the character-animation redraw.

Each replacement PNG has a `.png.hd` companion. These are metadata, not extra sprites. The ASCII header is `ISSBHD1 density logicalWidth logicalHeight`, followed by a newline and one byte (0 or 1) per original collision-mask pixel. Ship these companions with the PNGs and rebuild the game with the included engine changes. `BufferedImage` exposes original logical dimensions and collision masks; `Graphics2D` multiplies only texture source coordinates by density. SDL file access supports packaged assets as well as desktop paths.

`generation-log.json` records the final prompts and local generated-source locations. The installed PNGs are the game-ready assets; they do not depend on those external source locations at runtime. `Pack-Sprite.ps1` separates generated rows at transparent gaps, resizes each figure to its original frame bounds, and packs it into the original grid at 4x resolution. Some poses are artistic reinterpretations rather than exact tracings.

Run from the repository root:

```powershell
rtk proxy powershell -NoProfile -File tools/ichigo-hd/Verify-Sprites.ps1
```

Validation passed for 54/54 sheets and 227 frames: expected dimensions, nonempty frame slots, unchanged PNG filename count, and collision-mask bytes matching the original Git assets. See `validation.json` for the per-sheet report. C++ syntax checks passed for the modified renderer and image loader. The game has not been built or play-tested; animation smoothness and target-device texture limits still need an in-game check.
