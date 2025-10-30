#!/usr/bin/env python3
"""
Paper Mario Sprite Sheet Extraction Tool
Extracts frames from Paper Mario: The Origami King sprite folders
and compiles them into horizontal sprite sheets for iSSB
"""

from PIL import Image
import os
import sys

# Animation mapping based on PAPER_MARIO_INTEGRATION_GUIDE.md
ANIMATIONS = {
    # Basic Movements
    "stand.png": ("1", 9, 0),  # (folder, frame_count, start_offset)
    "walk.png": ("3", 4, 0),
    "run.png": ("3", 8, 0),
    "jump.png": ("4", 6, 0),
    "jump2.png": ("5", 20, 0),
    "land.png": ("6", 3, 0),
    "fall.png": ("19", 2, 0),
    "skid.png": ("18", 1, 0),
    "crouch.png": ("11", 1, 0),
    "hang.png": ("16", 1, 0),
    
    # Combat Actions
    "dash_attack.png": ("17", 6, 0),
    "standard_attack.png": ("7", 6, 0),
    "standard_attack2.png": ("7", 7, 0),
    "standard_attack3.png": ("7", 6, 0),
    "standard_attack_side.png": ("8", 9, 0),
    "standard_attack_up.png": ("15", 6, 0),
    "standard_attack_down.png": ("14", 12, 0),
    
    # Air Attacks
    "air_attack.png": ("15", 3, 0),
    "air_attack_side.png": ("15", 6, 0),
    "air_attack_up.png": ("15", 8, 0),
    "air_attack_down.png": ("14", 14, 0),
    
    # Special Moves
    "special_attack.png": ("8", 4, 0),
    "special_attack_side.png": ("9", 8, 0),
    "special_attack_up.png": ("5", 4, 0),
    "special_charge_down.png": ("12", 1, 0),
    "special_attack_down.png": ("12", 1, 0),
    
    # Smash Attacks
    "smash_charge.png": ("21", 1, 0),
    "smash_attack.png": ("10", 4, 0),
    "smash_attack_up.png": ("10", 4, 0),
    "smash_attack_down.png": ("14", 4, 0),
    
    # Grab & Throw
    "grab.png": ("12", 4, 0),
    "grabbed.png": ("13", 1, 0),
    "hold.png": ("12", 1, 0),
    "release.png": ("12", 4, 0),
    "toss.png": ("12", 4, 0),
    "toss_up.png": ("12", 5, 0),
    "toss_down.png": ("12", 6, 0),
    "grab_attack.png": ("12", 3, 0),
    "grab_attack_swing.png": ("12", 13, 0),
    
    # Melee Weapon
    "melee_weapon.png": ("12", 4, 0),
    "melee_weapon_up.png": ("12", 4, 0),
    "melee_weapon_down.png": ("12", 4, 0),
    
    # Hurt Animations
    "hurt_minor.png": ("13", 1, 0),
    "hurt_fly.png": ("13", 1, 0),
    "hurt_flip.png": ("13", 4, 0),
    "hurt_spin.png": ("14", 9, 0),
    "hurt_spin_up.png": ("14", 9, 0),
    
    # Final Smash
    "finalsmash_begin.png": ("22", 13, 0),
    "finalsmash_hold.png": ("22", 1, 0),
    "finalsmash_end.png": ("22", 18, 0),
}

def extract_and_compile_animation(source_base, output_file, folder, frame_count, start_offset=0):
    """
    Extract frames from a folder and compile into a horizontal sprite sheet
    """
    source_folder = os.path.join(source_base, "Individual frames", folder)
    
    if not os.path.exists(source_folder):
        print(f"Warning: Source folder not found: {source_folder}")
        return False
    
    # Get all PNG files in the folder
    all_files = sorted([f for f in os.listdir(source_folder) if f.endswith('.png') and not f.startswith('.')])
    
    if len(all_files) == 0:
        print(f"Warning: No PNG files found in {source_folder}")
        return False
    
    # Select the frames we need
    frame_files = all_files[start_offset:start_offset + frame_count]
    
    if len(frame_files) < frame_count:
        print(f"Warning: Only {len(frame_files)} frames available, needed {frame_count} for {output_file}")
        frame_files = all_files[:frame_count]  # Use first N frames instead
    
    frames = []
    for frame_file in frame_files:
        frame_path = os.path.join(source_folder, frame_file)
        try:
            img = Image.open(frame_path).convert('RGBA')
            frames.append(img)
        except Exception as e:
            print(f"Error loading {frame_path}: {e}")
            return False
    
    if not frames:
        print(f"Error: No frames loaded for {output_file}")
        return False
    
    # Determine sprite sheet dimensions
    # Use the maximum dimensions from all frames
    frame_height = max(img.height for img in frames)
    frame_width = max(img.width for img in frames)
    
    total_width = frame_width * len(frames)
    
    # Create sprite sheet with transparency
    sprite_sheet = Image.new('RGBA', (total_width, frame_height), (0, 0, 0, 0))
    
    # Paste frames horizontally, centering each frame
    x_offset = 0
    for img in frames:
        # Center the frame vertically if it's smaller than frame_height
        y_offset = (frame_height - img.height) // 2
        sprite_sheet.paste(img, (x_offset, y_offset), img)
        x_offset += frame_width
    
    # Save the sprite sheet
    sprite_sheet.save(output_file, 'PNG')
    print(f"[OK] Created {os.path.basename(output_file)} ({len(frames)} frames, {total_width}x{frame_height})")
    
    return True

def create_character_portrait(source_base, output_file):
    """
    Create character portrait from idle animation
    """
    source_folder = os.path.join(source_base, "Individual frames", "1")
    
    if not os.path.exists(source_folder):
        print(f"Warning: Cannot create portrait, folder not found: {source_folder}")
        return False
    
    # Get first idle frame
    all_files = sorted([f for f in os.listdir(source_folder) if f.endswith('.png') and not f.startswith('.')])
    
    if len(all_files) == 0:
        return False
    
    try:
        img = Image.open(os.path.join(source_folder, all_files[0])).convert('RGBA')
        
        # Resize to a reasonable portrait size (e.g., 200x200)
        portrait_size = (200, 200)
        
        # Calculate scaling to fit within portrait size while maintaining aspect ratio
        img.thumbnail(portrait_size, Image.Resampling.LANCZOS)
        
        # Create a new image with the portrait size and center the character
        portrait = Image.new('RGBA', portrait_size, (0, 0, 0, 0))
        offset_x = (portrait_size[0] - img.width) // 2
        offset_y = (portrait_size[1] - img.height) // 2
        portrait.paste(img, (offset_x, offset_y), img)
        
        portrait.save(output_file, 'PNG')
        print(f"[OK] Created character portrait: {os.path.basename(output_file)}")
        return True
    except Exception as e:
        print(f"Error creating portrait: {e}")
        return False

def main():
    # Paths
    source_base = "c:/Users/Ben/Documents/Cursor Projects/issb revamp/Nintendo Switch - Paper Mario_ The Origami King - Playable Characters - Mario"
    output_base = "C:/Users/Ben/iSSB/Assets/Images/Game/Characters/PaperMario"
    
    print("Paper Mario Sprite Sheet Extraction Tool")
    print("=" * 60)
    print(f"Source: {source_base}")
    print(f"Output: {output_base}")
    print("=" * 60)
    
    # Check if source exists
    if not os.path.exists(source_base):
        print(f"ERROR: Source directory not found!")
        print(f"Please ensure the Paper Mario sprites are at:")
        print(f"  {source_base}")
        return 1
    
    # Create output directory if it doesn't exist
    os.makedirs(output_base, exist_ok=True)
    
    # Process all animations
    success_count = 0
    fail_count = 0
    
    print(f"\nProcessing {len(ANIMATIONS)} animations...")
    print("-" * 60)
    
    for output_file, (folder, frame_count, start_offset) in ANIMATIONS.items():
        output_path = os.path.join(output_base, output_file)
        
        if extract_and_compile_animation(source_base, output_path, folder, frame_count, start_offset):
            success_count += 1
        else:
            fail_count += 1
    
    print("-" * 60)
    print(f"\nAnimation Processing Complete:")
    print(f"  Success: {success_count}")
    print(f"  Failed: {fail_count}")
    
    # Create character portrait
    print("\nCreating character portrait...")
    portrait_path = os.path.join(output_base, "papermario.png")
    if create_character_portrait(source_base, portrait_path):
        print("[OK] Portrait created successfully")
    else:
        print("[FAIL] Failed to create portrait")
    
    # Also create a menu icon version
    print("\nCreating menu icon...")
    menu_icon_path = os.path.join(output_base, "../../Menu/CharIcons/papermario.png")
    os.makedirs(os.path.dirname(menu_icon_path), exist_ok=True)
    if create_character_portrait(source_base, menu_icon_path):
        print("[OK] Menu icon created successfully")
    
    print("\n" + "=" * 60)
    print("All done! Sprite sheets are ready in:")
    print(f"  {output_base}")
    print("=" * 60)
    
    return 0 if fail_count == 0 else 1

if __name__ == "__main__":
    sys.exit(main())

