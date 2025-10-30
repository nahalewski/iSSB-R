# Paper Mario Samurai Final Smash - Sprite Extraction Guide

## Required Sprite Files

To complete the samurai Final Smash implementation, you need to extract the following sprites from the sprite sheet and place them in this folder:

### 1. Final Smash Begin (`finalsmash_samurai_begin.png`)
- Extract: Samurai transformation/beginning pose
- Shows Paper Mario transforming into the samurai costume
- Use frames from the beginning of the samurai costume sequence

### 2. Final Smash Hold Sequence (`finalsmash_samurai_hold0.png` through `finalsmash_samurai_hold8.png`)
- Extract: 9 fan-wielding action poses from rows 13-14 (columns 4-9)
- These are the dynamic attack poses with the red fan
- Frame sequence should show:
  - 0: Initial fan pose
  - 1-3: Fan raising/preparation
  - 4-6: Fan swinging/mid-attack
  - 7-8: Fan finishing/recovery

### 3. Final Smash End (`finalsmash_samurai_end.png`)
- Extract: Return to normal form pose
- Shows Paper Mario returning from samurai back to normal
- Use frames from the end of the samurai sequence

## Sprite Sheet Location

The samurai costume sprites are located in:
- **Rows 13-14** from the top of the sprite sheet
- **Columns 1-9** contain various poses
- Focus on **columns 4-9** for the fan-wielding action poses

## Sprite Extraction Tips

1. Use an image editor (GIMP, Photoshop, or online tools) to extract individual sprites
2. Each sprite should be cropped to show only Paper Mario (remove background/padding)
3. Maintain consistent sprite dimensions
4. Ensure sprites face left (the code will mirror for right-facing)
5. Save as PNG format with transparency

## File Naming Convention

- `finalsmash_samurai_begin.png` - Beginning transformation
- `finalsmash_samurai_hold0.png` - Fan pose frame 0
- `finalsmash_samurai_hold1.png` - Fan pose frame 1
- ... (continue through hold8.png)
- `finalsmash_samurai_end.png` - Ending transformation

## Implementation Notes

The code expects:
- 9 fan-wielding frames (hold0 through hold8)
- Each frame should show a dynamic fan attack pose
- The animation will play as a sequence during the Final Smash attack
- Additional origami fan projectiles will spawn during the attack

## Testing

After extracting and placing the sprites:
1. Build the project
2. Test Paper Mario's Final Smash in-game
3. Verify the samurai transformation and fan attack animations play correctly

