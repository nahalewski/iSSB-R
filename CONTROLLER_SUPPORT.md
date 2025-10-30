# Controller Support

iSSB has comprehensive support for modern gaming controllers on Android.

## Supported Controllers

### ✅ Officially Tested & Supported

1. **Nintendo Pro Controller**
   - Vendor ID: `0x057E`
   - Product IDs: `0x2009` (USB), `0x2017` (Bluetooth)
   - Full button mapping support
   - Gyro/motion controls: Not used

2. **PlayStation 5 DualSense**
   - Vendor ID: `0x054C`
   - Product ID: `0x0CE6`
   - Full button mapping support
   - Adaptive triggers: Not used
   - Haptic feedback: Standard rumble support

3. **PlayStation 4 DualShock 4**
   - Vendor ID: `0x054C`
   - Product IDs: `0x05C4` (Gen 1), `0x09CC` (Gen 2)
   - Full button mapping support
   - Touchpad: Not used for input
   - Light bar: Not controlled by game

4. **Xbox Series X/S Controller**
   - Vendor ID: `0x045E`
   - Product IDs: `0x0B12` (USB), `0x0B13` (Bluetooth)
   - Full button mapping support
   - Share button: Supported

5. **Xbox One Controller**
   - Vendor ID: `0x045E`
   - Product IDs: `0x02DD` (Wireless), `0x02D1` (Wired), `0x02EA` (Elite)
   - Full button mapping support

6. **Xbox 360 Controller**
   - Vendor ID: `0x045E`
   - Product ID: `0x028E`
   - Full button mapping support
   - Works via USB OTG cable

## Button Mappings

All controllers use standard SDL2 gamepad button mappings:

### Combat Controls
- **A Button** (Xbox) / **Cross** (PS) / **B** (Nintendo): Jump
- **B Button** (Xbox) / **Circle** (PS) / **A** (Nintendo): Attack
- **X Button** (Xbox) / **Square** (PS) / **Y** (Nintendo): Special Attack
- **Y Button** (Xbox) / **Triangle** (PS) / **X** (Nintendo): Shield/Block

### Movement
- **Left Stick**: Character movement
- **D-Pad**: Alternative movement/menu navigation
- **Right Stick**: Camera control (if enabled)

### System Controls
- **Start/Options**: Pause menu
- **Back/Share/Minus**: Return to menu
- **Bumpers (L1/R1)**: Grab/Shield
- **Triggers (L2/R2)**: Dodge/Roll

## Connection Methods

### Bluetooth
1. Put controller in pairing mode
2. Open Android Bluetooth settings
3. Select controller from available devices
4. Controller will auto-connect in-game

### USB OTG
1. Connect controller via USB-C to USB-A adapter
2. Plug into Android device
3. Controller is instantly recognized

## Controller Detection

When a controller is connected, you'll see a toast notification:
- ✓ **Nintendo Pro Controller connected**
- ✓ **PS5 DualSense Controller connected**
- ✓ **PS4 DualShock 4 Controller connected**
- ✓ **Xbox Series X/S Controller connected**
- ✓ **Xbox One Controller connected**
- ✓ **Xbox 360 Controller connected**

When disconnected:
- ✗ **Controller disconnected**

## Troubleshooting

### Controller not detected
1. Ensure controller is charged
2. Try reconnecting Bluetooth
3. Restart the app
4. Check Android gamepad settings

### Buttons not responding
1. Check that controller firmware is updated
2. Verify controller works in other Android apps
3. Try USB connection instead of Bluetooth
4. Restart both controller and device

### Lag/Input delay
1. Use USB connection for lowest latency
2. Ensure Bluetooth is not congested
3. Keep controller close to device (Bluetooth)
4. Close background apps

## Multi-Controller Support

- Up to **4 controllers** can be connected simultaneously
- Each controller is assigned to a player automatically
- Controllers are detected in connection order
- Player 1: First connected controller
- Player 2: Second connected controller
- etc.

## Technical Details

### SDL2 GameController API
iSSB uses SDL2's GameController API which provides:
- Automatic button remapping across controller types
- Standardized axis values for analog sticks
- Trigger pressure sensitivity
- Rumble/haptic feedback support

### Android Input System
Controllers are detected via:
- `InputDevice.SOURCE_GAMEPAD`
- `InputDevice.SOURCE_JOYSTICK`  
- `InputDevice.SOURCE_DPAD`

Vendor/Product ID matching ensures proper controller identification.

## Future Enhancements

Planned features:
- [ ] Custom button remapping UI
- [ ] Controller-specific haptic patterns
- [ ] Gyro controls for aiming
- [ ] Per-player controller assignment
- [ ] Controller battery status display
- [ ] Advanced rumble effects

## Notes

- SDL2 handles all controller input processing
- No root access required
- Works with wired and wireless controllers
- Generic HID controllers may work but are untested
- Third-party controllers should work if they emulate standard controllers

