package com.brokenphysics.issb;

import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.hardware.input.InputManager;
import android.view.InputDevice;
import android.view.View;
import android.widget.Toast;
import androidx.core.view.WindowCompat;
import androidx.core.view.WindowInsetsControllerCompat;
import org.libsdl.app.SDLActivity;

public class MainActivity extends SDLActivity implements InputManager.InputDeviceListener {
	private static final String TAG = "iSSB";
	private InputManager inputManager;
	private Handler mainHandler;
	
	/**
	 * A native method that is implemented by the 'native-lib' native library,
	 * which is packaged with this application.
	 */
	public native String stringFromJNI();

	protected String[] getLibraries() {
		return new String[] {
				"hidapi",
				"SDL2",
				"SDL2_image",
				"SDL2_mixer",
				//"SDL2_net",
				"SDL2_ttf",
				"iSSBLib"
		};
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		mainHandler = new Handler(Looper.getMainLooper());
		super.onCreate(savedInstanceState);
		
		// Enable edge-to-edge display and modern fullscreen
		// Must be called after super.onCreate() so window is available
		// Use multiple posts to ensure it runs after SDLActivity finishes its setup
		mainHandler.post(() -> {
			setupModernFullscreen();
		});
		mainHandler.postDelayed(() -> {
			setupModernFullscreen();
		}, 200);
		mainHandler.postDelayed(() -> {
			setupModernFullscreen();
		}, 500);
		
		// Set up gamepad input device monitoring
		setupGamepadMonitoring();
	}

	@Override
	protected void onResume() {
		super.onResume();
		// Re-apply fullscreen after resume to ensure it stays active
		mainHandler.postDelayed(() -> {
			setupModernFullscreen();
		}, 100);
		mainHandler.postDelayed(() -> {
			setupModernFullscreen();
		}, 300);
	}

	@Override
	public void onWindowFocusChanged(boolean hasFocus) {
		super.onWindowFocusChanged(hasFocus);
		
		if (hasFocus) {
			// Re-apply immersive fullscreen when window regains focus
			mainHandler.postDelayed(() -> {
				setupModernFullscreen();
			}, 50);
			mainHandler.postDelayed(() -> {
				setupModernFullscreen();
			}, 200);
		}
	}
	
	@Override
	public void onSystemUiVisibilityChange(int visibility) {
		super.onSystemUiVisibilityChange(visibility);
		
		// If system UI becomes visible, re-hide it immediately
		if ((visibility & View.SYSTEM_UI_FLAG_FULLSCREEN) == 0 || 
		    (visibility & View.SYSTEM_UI_FLAG_HIDE_NAVIGATION) == 0) {
			mainHandler.postDelayed(() -> {
				setupModernFullscreen();
			}, 100);
		}
	}

	/**
	 * Set up modern fullscreen using WindowCompat and WindowInsetsControllerCompat
	 */
	private void setupModernFullscreen() {
		if (getWindow() == null) return;
		
		// Enable edge-to-edge display
		WindowCompat.setDecorFitsSystemWindows(getWindow(), false);
		
		// Remove window flags that might interfere
		getWindow().clearFlags(android.view.WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN);
		getWindow().addFlags(android.view.WindowManager.LayoutParams.FLAG_FULLSCREEN);
		
		View decorView = getWindow().getDecorView();
		
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
			// Android 11+ (API 30+)
			WindowInsetsControllerCompat windowInsetsController = 
				WindowCompat.getInsetsController(getWindow(), decorView);
			
			if (windowInsetsController != null) {
				// Hide system bars and enable immersive sticky mode
				windowInsetsController.hide(
					androidx.core.view.WindowInsetsCompat.Type.systemBars()
				);
				windowInsetsController.setSystemBarsBehavior(
					WindowInsetsControllerCompat.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE
				);
			}
		} else if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
			// Android 4.4+ (API 19+) - fallback for older devices
			applyImmersiveFullscreen();
		}
		
		// Force fullscreen mode active in SDLActivity
		org.libsdl.app.SDLActivity.mFullscreenModeActive = true;
	}

	/**
	 * Apply immersive fullscreen flags for Android 10 and below
	 */
	private void applyImmersiveFullscreen() {
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
			int flags = android.view.View.SYSTEM_UI_FLAG_FULLSCREEN
					| android.view.View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
					| android.view.View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
					| android.view.View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
					| android.view.View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
					| android.view.View.SYSTEM_UI_FLAG_LAYOUT_STABLE;
			
			getWindow().getDecorView().setSystemUiVisibility(flags);
		}
	}

	/**
	 * Set up gamepad input device monitoring
	 */
	private void setupGamepadMonitoring() {
		inputManager = (InputManager) getSystemService(INPUT_SERVICE);
		if (inputManager != null) {
			inputManager.registerInputDeviceListener(this, null);
			
			// Check for already connected controllers
			int[] deviceIds = inputManager.getInputDeviceIds();
			for (int deviceId : deviceIds) {
				InputDevice device = inputManager.getInputDevice(deviceId);
				if (device != null && isGamepadDevice(device)) {
					onInputDeviceAdded(deviceId);
				}
			}
		}
	}

	/**
	 * Check if an InputDevice is a gamepad/controller
	 */
	private boolean isGamepadDevice(InputDevice device) {
		int sources = device.getSources();
		return (sources & InputDevice.SOURCE_GAMEPAD) == InputDevice.SOURCE_GAMEPAD
				|| (sources & InputDevice.SOURCE_JOYSTICK) == InputDevice.SOURCE_JOYSTICK
				|| (sources & InputDevice.SOURCE_DPAD) == InputDevice.SOURCE_DPAD;
	}

	/**
	 * Detect specific controller type and return friendly name
	 */
	private String getControllerType(InputDevice device) {
		String deviceName = device.getName().toLowerCase();
		int vendorId = device.getVendorId();
		int productId = device.getProductId();
		
		// Nintendo Pro Controller
		if (deviceName.contains("pro controller") || 
		    (vendorId == 0x057E && (productId == 0x2009 || productId == 0x2017))) {
			return "Nintendo Pro Controller";
		}
		
		// PS5 DualSense
		if (deviceName.contains("dualsense") || 
		    deviceName.contains("ps5") ||
		    (vendorId == 0x054C && productId == 0x0CE6)) {
			return "PS5 DualSense Controller";
		}
		
		// PS4 DualShock 4
		if (deviceName.contains("dualshock") || 
		    deviceName.contains("ps4") ||
		    deviceName.contains("wireless controller") ||
		    (vendorId == 0x054C && (productId == 0x05C4 || productId == 0x09CC))) {
			return "PS4 DualShock 4 Controller";
		}
		
		// Xbox Controllers (One, Series X/S, 360)
		if (deviceName.contains("xbox") || 
		    deviceName.contains("x-box") ||
		    (vendorId == 0x045E && (productId == 0x02D1 || productId == 0x02DD || 
		                             productId == 0x0B12 || productId == 0x0B13 ||
		                             productId == 0x028E || productId == 0x02EA))) {
			if (deviceName.contains("series") || productId == 0x0B12 || productId == 0x0B13) {
				return "Xbox Series X/S Controller";
			} else if (deviceName.contains("one") || productId == 0x02DD || productId == 0x02D1) {
				return "Xbox One Controller";
			} else {
				return "Xbox 360 Controller";
			}
		}
		
		// Generic fallback
		return device.getName();
	}

	@Override
	public void onInputDeviceAdded(int deviceId) {
		InputDevice device = inputManager.getInputDevice(deviceId);
		if (device != null && isGamepadDevice(device)) {
			String controllerType = getControllerType(device);
			android.util.Log.d(TAG, "Controller connected: " + controllerType + 
			                   " (Vendor: 0x" + Integer.toHexString(device.getVendorId()) + 
			                   ", Product: 0x" + Integer.toHexString(device.getProductId()) + ")");
			
			runOnUiThread(() -> {
				Toast.makeText(this, "✓ " + controllerType + " connected", Toast.LENGTH_LONG).show();
			});
		}
	}

	@Override
	public void onInputDeviceRemoved(int deviceId) {
		// Note: device will be null here since it's already removed, so we can't get the type
		android.util.Log.d(TAG, "Controller disconnected (ID: " + deviceId + ")");
		
		runOnUiThread(() -> {
			Toast.makeText(this, "✗ Controller disconnected", Toast.LENGTH_SHORT).show();
		});
	}

	@Override
	public void onInputDeviceChanged(int deviceId) {
		// Called when a device's configuration changes
		// We can ignore this for now, but log it for debugging
		InputDevice device = inputManager.getInputDevice(deviceId);
		if (device != null && isGamepadDevice(device)) {
			android.util.Log.d(TAG, "Controller changed: " + device.getName() + " (ID: " + deviceId + ")");
		}
	}

    @Override
    protected void onDestroy() {
        if (inputManager != null) {
            inputManager.unregisterInputDeviceListener(this);
        }
        super.onDestroy();
    }
    
    /**
     * JNI methods callable from C++ to get menu data
     */
    public static int getP1Character() {
        try {
            android.content.Context context = org.libsdl.app.SDLActivity.getContext();
            if (context == null) return -1;
            
            android.content.SharedPreferences prefs = context.getSharedPreferences("iSSB_GameData", MODE_PRIVATE);
            return prefs.getInt("p1_character", -1);
        } catch (Exception e) {
            android.util.Log.e(TAG, "Failed to get P1 character: " + e.getMessage());
            return -1;
        }
    }
    
    public static int getP2Character() {
        try {
            android.content.Context context = org.libsdl.app.SDLActivity.getContext();
            if (context == null) return -1;
            
            android.content.SharedPreferences prefs = context.getSharedPreferences("iSSB_GameData", MODE_PRIVATE);
            return prefs.getInt("p2_character", -1);
        } catch (Exception e) {
            android.util.Log.e(TAG, "Failed to get P2 character: " + e.getMessage());
            return -1;
        }
    }
    
    public static boolean shouldSkipMenus() {
        try {
            android.content.Context context = org.libsdl.app.SDLActivity.getContext();
            if (context == null) {
                android.util.Log.e(TAG, "shouldSkipMenus: context is null");
                return false;
            }
            
            android.content.SharedPreferences prefs = context.getSharedPreferences("iSSB_GameData", MODE_PRIVATE);
            boolean skip = prefs.getBoolean("skip_cpp_menus", false);
            
            android.util.Log.d(TAG, "shouldSkipMenus: " + skip);
            
            // DON'T clear the flag yet - let the game clear it after successful load
            
            return skip;
        } catch (Exception e) {
            android.util.Log.e(TAG, "Failed to check skip menus: " + e.getMessage());
            return false;
        }
    }
    
    public static void clearCharacterSelections() {
        try {
            android.content.Context context = org.libsdl.app.SDLActivity.getContext();
            if (context == null) return;
            
            android.content.SharedPreferences prefs = context.getSharedPreferences("iSSB_GameData", MODE_PRIVATE);
            android.content.SharedPreferences.Editor editor = prefs.edit();
            // Clear the skip menus flag so we don't auto-launch again
            editor.putBoolean("skip_cpp_menus", false);
            // Don't clear character selections - keep them for replay
            editor.apply();
            
            android.util.Log.d(TAG, "Cleared skip_cpp_menus flag");
        } catch (Exception e) {
            android.util.Log.e(TAG, "Failed to clear selections: " + e.getMessage());
        }
    }
    
    public static boolean getTouchControlsSetting() {
        try {
            android.content.Context context = org.libsdl.app.SDLActivity.getContext();
            if (context == null) return false;
            
            android.content.SharedPreferences prefs = context.getSharedPreferences("iSSB_Settings", MODE_PRIVATE);
            boolean touchEnabled = prefs.getString("touchControls", "false").equals("true");
            android.util.Log.d(TAG, "Touch controls setting: " + touchEnabled);
            return touchEnabled;
        } catch (Exception e) {
            android.util.Log.e(TAG, "Failed to get touch controls setting: " + e.getMessage());
            return false; // Default OFF
        }
    }
    
    public static int getSelectedStage() {
        try {
            android.content.Context context = org.libsdl.app.SDLActivity.getContext();
            if (context == null) return 0; // Default to first stage
            
            android.content.SharedPreferences prefs = context.getSharedPreferences("iSSB_GameData", MODE_PRIVATE);
            int stage = prefs.getInt("selected_stage", 0);
            android.util.Log.d(TAG, "Selected stage: " + stage);
            return stage;
        } catch (Exception e) {
            android.util.Log.e(TAG, "Failed to get selected stage: " + e.getMessage());
            return 0; // Default to first stage
        }
    }
    
    public static void returnToCharacterSelect() {
        try {
            // Get the current activity instance
            final MainActivity activity = (MainActivity) mSingleton;
            if (activity != null) {
                activity.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        android.util.Log.d(TAG, "Returning to character select menu");
                        
                        // Launch MenuActivity with character select
                        android.content.Intent intent = new android.content.Intent(activity, MenuActivity.class);
                        intent.putExtra("menu", "character_select");
                        intent.addFlags(android.content.Intent.FLAG_ACTIVITY_CLEAR_TOP);
                        activity.startActivity(intent);
                        
                        // Finish the game activity
                        activity.finish();
                    }
                });
            }
        } catch (Exception e) {
            android.util.Log.e(TAG, "Failed to return to character select: " + e.getMessage());
        }
    }
}
