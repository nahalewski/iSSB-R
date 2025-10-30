# Paper Mario: The Origami King - Character Integration Guide

## Overview
This guide explains how to integrate the Paper Mario sprite sheets from "Paper Mario: The Origami King" as a playable character or alternate costume in iSSB.

## Sprite Asset Location
**Source:** `c:/Users/Ben/Documents/Cursor Projects/issb revamp/Nintendo Switch - Paper Mario_ The Origami King - Playable Characters - Mario/`

### Available Assets
- **Sprite Sheets:** 4 compiled sheets (`Mario 1.png`, `Mario 2.png`, `Mario 3.png`, `Mario 4.png`)
- **Individual Frames:** Organized in 22 numbered folders (animations 1-22)
- **Total Frames:** ~2000+ individual sprite frames

## Animation Mapping

Based on the existing Mario character animation system and the available Paper Mario sprites, here's the recommended mapping:

### Folder Structure Analysis
```
Individual frames/
├── 1/  (946 frames) - Likely: Idle/Standing animation
├── 2/  (3 frames)   - Likely: Turning animation
├── 3/  (70 frames)  - Likely: Walking/Running
├── 4/  (20 frames)  - Likely: Jump start
├── 5/  (270 frames) - Likely: Jump/Air movement
├── 6/  (14 frames)  - Likely: Landing
├── 7/  (14 frames)  - Likely: Attack A
├── 8/  (16 frames)  - Likely: Attack B
├── 9/  (16 frames)  - Likely: Special move
├── 10/ (334 frames) - Likely: Complex attack sequence
├── 11/ (3 frames)   - Likely: Crouch/Duck
├── 12/ (43 frames)  - Likely: Grab/Throw
├── 13/ (4 frames)   - Likely: Hurt/Damage
├── 14/ (33 frames)  - Likely: Spin attack
├── 15/ (9 frames)   - Likely: Air attack
├── 16/ (2 frames)   - Likely: Hang/Ledge
├── 17/ (6 frames)   - Likely: Dash
├── 18/ (6 frames)   - Likely: Skid/Stop
├── 19/ (2 frames)   - Likely: Fall
├── 20/ (3 frames)   - Likely: Taunt/Victory
├── 21/ (4 frames)   - Likely: Special action
├── 22/ (22 frames)  - Likely: Final Smash/Ultimate
```

### Recommended Animation Mapping to iSSB System

#### Basic Movements
| iSSB Animation | Folder | Frame Count | Duration (ms) | Notes |
|---------------|--------|-------------|---------------|-------|
| `stand` | 1 | Select 9 frames | 10ms | Use representative idle poses |
| `walk` | 3 | 4-6 frames | 10ms | Extract walking cycle |
| `run` | 3 | 8 frames | 15ms | Faster walk frames |
| `jump` | 4 | 6 frames | 10ms | Jump start sequence |
| `jump2` | 5 | 20 frames | 30ms | Mid-air/double jump |
| `land` | 6 | 3 frames | 20ms | Landing impact |
| `fall` | 19 | 2 frames | 30ms | Falling pose |
| `skid` | 18 | 1 frame | 30ms | Stopping pose |
| `crouch` | 11 | 1 frame | 1ms | Crouching |
| `hang` | 16 | 1 frame | 1ms | Ledge hang |

#### Combat Actions
| iSSB Animation | Folder | Frame Count | Duration (ms) | Notes |
|---------------|--------|-------------|---------------|-------|
| `dash_attack` | 17 | 6 frames | 17ms | Dash attack |
| `standard_attack` | 7 | 6 frames | 24ms | Basic A attack |
| `standard_attack2` | 7 | 7 frames | 26ms | Second hit combo |
| `standard_attack3` | 7 | 6 frames | 18ms | Third hit combo |
| `standard_attack_side` | 8 | 9 frames | 16ms | Side tilt attack |
| `standard_attack_up` | 15 | 6 frames | 18ms | Up tilt |
| `standard_attack_down` | 14 | 12 frames | 25ms | Down tilt |

#### Air Attacks
| iSSB Animation | Folder | Frame Count | Duration (ms) | Notes |
|---------------|--------|-------------|---------------|-------|
| `air_attack` | 15 | 3 frames | 8ms | Neutral air |
| `air_attack_side` | 15 | 6 frames | 12ms | Forward air |
| `air_attack_up` | 15 | 6-8 frames | 45ms | Up air (use spinning frames) |
| `air_attack_down` | 14 | 14 frames | 20ms | Down air (spin attack) |

#### Special Moves
| iSSB Animation | Folder | Frame Count | Duration (ms) | Notes |
|---------------|--------|-------------|---------------|-------|
| `special_attack` | 8 | 4 frames | 14ms | Neutral B (Hammer) |
| `special_attack_side` | 9 | 8 frames | 18ms | Side B |
| `special_attack_up` | 5 | 4 frames | 7ms | Up B (Jump) |
| `special_charge_down` | 12 | 1 frame | 0ms | Down B charge |
| `special_attack_down` | 12 | 1 frame | 0ms | Down B release |

#### Smash Attacks
| iSSB Animation | Folder | Frame Count | Duration (ms) | Notes |
|---------------|--------|-------------|---------------|-------|
| `smash_charge` | 21 | 1 frame | 1ms | Charge pose |
| `smash_attack` | 10 | 4 frames | 10ms | Side smash |
| `smash_attack_up` | 10 | 4 frames | 8ms | Up smash |
| `smash_attack_down` | 14 | 4 frames | 10ms | Down smash (spin) |

#### Grab & Throw
| iSSB Animation | Folder | Frame Count | Duration (ms) | Notes |
|---------------|--------|-------------|---------------|-------|
| `grab` | 12 | 4 frames | 10ms | Grabbing animation |
| `grabbed` | 13 | 1 frame | 30ms | Being grabbed |
| `hold` | 12 | 1 frame | 30ms | Holding opponent |
| `release` | 12 | 4 frames | 12ms | Releasing opponent |
| `toss` | 12 | 4 frames | 14ms | Forward throw |
| `toss_up` | 12 | 5 frames | 17ms | Up throw |
| `toss_down` | 12 | 6 frames | 20ms | Down throw |
| `grab_attack` | 12 | 3 frames | 10ms | Pummel |

#### Hurt Animations
| iSSB Animation | Folder | Frame Count | Duration (ms) | Notes |
|---------------|--------|-------------|---------------|-------|
| `hurt_minor` | 13 | 1 frame | 2ms | Light hit |
| `hurt_fly` | 13 | 1 frame | 2ms | Launching |
| `hurt_flip` | 13 | 4 frames | 10ms | Tumbling |
| `hurt_spin` | 14 | 9 frames | 18ms | Spin damage (two-sided) |
| `hurt_spin_up` | 14 | 9 frames | 18ms | Upward spin (single) |

#### Final Smash
| iSSB Animation | Folder | Frame Count | Duration (ms) | Notes |
|---------------|--------|-------------|---------------|-------|
| `finalsmash_begin` | 22 | 13 frames | 10ms | Final Smash startup |
| `finalsmash_hold` | 22 | 1 frame | 0ms | Holding (loop) |
| `finalsmash_end` | 22 | 18 frames | 15ms | Final Smash ending |

## Implementation Steps

### Step 1: Prepare Sprite Sheets
1. Navigate to the Paper Mario folders
2. For each animation, extract the required frames from the numbered folders
3. Compile frames into horizontal sprite sheets (PNG format)
4. Name files according to iSSB convention (e.g., `stand.png`, `walk.png`, etc.)

### Step 2: Create Character Directory
```bash
mkdir -p "C:/Users/Ben/iSSB/Assets/Images/Game/Characters/PaperMario"
```

### Step 3: Process Sprite Sheets
For each animation:
1. Open the source folder (e.g., `Individual frames/1/`)
2. Select the required number of frames
3. Arrange horizontally in a new image
4. Save as `[animation_name].png`
5. Copy to `Assets/Images/Game/Characters/PaperMario/`

### Step 4: Create Character Class (Option A: New Character)

Create `C:/Users/Ben/iSSB/Source/SmashBros/Game/Characters/PaperMario.h`:

```cpp
#include "../../Player.h"

#pragma once

namespace SmashBros
{
    class PaperMario : public Player
    {
    private:
        // Paper-specific abilities
        byte hammerCharge;
        boolean confettiActive;
        
    public:
        PaperMario(float x1, float y1, byte playerNo, byte team);
        virtual ~PaperMario();
        
        virtual void Load();
        virtual void LoadAttackTypes();
        virtual void Unload();
        
        // Override attacks with paper-themed moves
        virtual void attackB();        // Hammer
        virtual void attackSideB();    // Paper Airplane
        virtual void attackUpB();      // 1000-Fold Arms
        virtual void attackDownB();    // Confetti Burst
        virtual void attackFinalSmash(); // Origami Transformation
    };
}
```

Create `C:/Users/Ben/iSSB/Source/SmashBros/Game/Characters/PaperMario.cpp`:

```cpp
#include "PaperMario.h"
#include "../../Global.h"
#include "../../AttackTemplates.h"

namespace SmashBros
{
    PaperMario::PaperMario(float x1, float y1, byte playerNo, byte team) 
        : Player(x1, y1, playerNo, team)
    {
        hammerCharge = 0;
        confettiActive = false;
        
        walkSpeed = 2.8;  // Slightly slower than Mario
        runSpeed = 3.7;
        fallWalk = 1.8;
        fallRun = 1.8;
        
        weight = 0.08;  // Lighter (paper!)
        
        name = "Paper Mario";
        
        setScale(1.1f);
        setItemOffset(4, 2);
        setHitbox(-10, -9, 20, 28);
        setHangPoint(5, 20);
    }
    
    PaperMario::~PaperMario()
    {
        //
    }
    
    void PaperMario::Load()
    {
        setFolderPath("Images/Game/Characters/PaperMario/");
        
        // Load all animations using the mapping table above
        addTwoSidedAnimation("stand", "stand.png", 10, 9, 1);
        addTwoSidedAnimation("walk", "walk.png", 10, 4, 1);
        addTwoSidedAnimation("run", "run.png", 15, 8, 1);
        // ... continue for all animations from the mapping table
    }
    
    void PaperMario::LoadAttackTypes()
    {
        // Similar to Mario but with paper-themed variations
        addAIAttackType(ATTACK_B, ATTACKTYPE_MELEE, 2);  // Hammer
        // ... etc
    }
    
    void PaperMario::Unload()
    {
        Player::Unload();
    }
    
    void PaperMario::attackB()
    {
        // Hammer attack implementation
        // TODO: Add hammer projectile/melee hitbox
    }
}
```

### Step 5: Alternative - Add as Mario Costume

Instead of a new character, add Paper Mario as an alternate costume:

In `Mario.cpp`, modify the `Load()` function:

```cpp
void Mario::Load()
{
    // Check if Paper Mario skin is selected
    if(getSkinID() == 1)  // Paper Mario skin
    {
        setFolderPath("Images/Game/Characters/PaperMario/");
    }
    else  // Default Mario
    {
        setFolderPath("Images/Game/Characters/Mario/");
    }
    
    // Load animations (same names for both)
    addTwoSidedAnimation("stand", "stand.png", 10, 9, 1);
    // ... etc
}
```

### Step 6: Register Character
Add to character selection system in the appropriate menu file.

## Frame Extraction Tool (Python Script)

Create `extract_paper_mario_frames.py`:

```python
from PIL import Image
import os

def extract_and_compile_animation(source_folder, output_file, frame_count, frame_width=None):
    """
    Extract frames from a folder and compile into a horizontal sprite sheet
    """
    frames = []
    frame_files = sorted([f for f in os.listdir(source_folder) if f.endswith('.png')])[:frame_count]
    
    for frame_file in frame_files:
        frame_path = os.path.join(source_folder, frame_file)
        img = Image.open(frame_path)
        frames.append(img)
    
    # Determine sprite sheet dimensions
    if not frames:
        return
    
    frame_height = max(img.height for img in frames)
    if frame_width is None:
        frame_width = max(img.width for img in frames)
    
    total_width = frame_width * len(frames)
    
    # Create sprite sheet
    sprite_sheet = Image.new('RGBA', (total_width, frame_height))
    
    x_offset = 0
    for img in frames:
        sprite_sheet.paste(img, (x_offset, 0))
        x_offset += frame_width
    
    sprite_sheet.save(output_file)
    print(f"Created {output_file} with {len(frames)} frames")

# Animation mapping
animations = {
    "stand.png": ("Individual frames/1", 9),
    "walk.png": ("Individual frames/3", 4),
    "run.png": ("Individual frames/3", 8),
    "jump.png": ("Individual frames/4", 6),
    # Add all animations from the mapping table
}

base_path = "c:/Users/Ben/Documents/Cursor Projects/issb revamp/Nintendo Switch - Paper Mario_ The Origami King - Playable Characters - Mario/"
output_path = "C:/Users/Ben/iSSB/Assets/Images/Game/Characters/PaperMario/"

os.makedirs(output_path, exist_ok=True)

for output_file, (source_folder, frame_count) in animations.items():
    full_source = os.path.join(base_path, source_folder)
    full_output = os.path.join(output_path, output_file)
    extract_and_compile_animation(full_source, full_output, frame_count)
```

## Next Steps

1. **Test in MenuActivity**: Update character list to include Paper Mario
2. **Create Character Icon**: Extract a representative frame for the character select portrait
3. **Balance Testing**: Adjust weight, speed, and damage values
4. **Special Moves**: Implement paper-themed special attacks (hammer, confetti, etc.)

## Notes

- Paper Mario is visually flatter/2D - this fits the sprite-based engine perfectly
- Consider adding a "paper flutter" effect when damaged (unique to Paper Mario)
- The "Back" folders in the sprite source contain alternate angles - use these for some animations if needed
- Folder 1 has 946 frames - likely includes ALL idle animations and reactions

## Integration Checklist

- [ ] Extract and compile sprite sheets from folders
- [ ] Create PaperMario character class
- [ ] Implement Load() function with all animations
- [ ] Add character to selection menu
- [ ] Create character portrait/icon
- [ ] Test all animations in-game
- [ ] Balance character stats
- [ ] Implement special moves
- [ ] Add sound effects
- [ ] Test multiplayer

