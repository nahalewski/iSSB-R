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

	@Override
	public void onInputDeviceAdded(int deviceId) {
		InputDevice device = inputManager.getInputDevice(deviceId);
		if (device != null && isGamepadDevice(device)) {
			String deviceName = device.getName();
			android.util.Log.d(TAG, "Controller connected: " + deviceName + " (ID: " + deviceId + ")");
			
			runOnUiThread(() -> {
				Toast.makeText(this, "Controller connected: " + deviceName, Toast.LENGTH_SHORT).show();
			});
		}
	}

	@Override
	public void onInputDeviceRemoved(int deviceId) {
		InputDevice device = inputManager.getInputDevice(deviceId);
		if (device != null && isGamepadDevice(device)) {
			String deviceName = device.getName();
			android.util.Log.d(TAG, "Controller disconnected: " + deviceName + " (ID: " + deviceId + ")");
			
			runOnUiThread(() -> {
				Toast.makeText(this, "Controller disconnected: " + deviceName, Toast.LENGTH_SHORT).show();
			});
		}
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
}
