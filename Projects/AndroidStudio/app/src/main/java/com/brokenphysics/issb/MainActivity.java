package com.brokenphysics.issb;

import org.libsdl.app.SDLActivity;
import android.os.Build;
import android.view.View;
import android.view.WindowInsets;
import android.view.WindowInsetsController;
import android.view.WindowManager;

public class MainActivity extends SDLActivity {
	private void enterImmersiveMode() {
		if (Build.VERSION.SDK_INT >= 28) {
			WindowManager.LayoutParams attrs = getWindow().getAttributes();
			attrs.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
			getWindow().setAttributes(attrs);
		}
		if (Build.VERSION.SDK_INT >= 30) {
			getWindow().setDecorFitsSystemWindows(false);
			WindowInsetsController controller = getWindow().getInsetsController();
			if (controller != null) {
				controller.setSystemBarsBehavior(WindowInsetsController.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE);
				controller.hide(WindowInsets.Type.systemBars());
			}
		} else {
			getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
				| View.SYSTEM_UI_FLAG_FULLSCREEN | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
				| View.SYSTEM_UI_FLAG_LAYOUT_STABLE | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
				| View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION);
		}
	}

	@Override public void onWindowFocusChanged(boolean hasFocus) {
		super.onWindowFocusChanged(hasFocus);
		if (hasFocus) enterImmersiveMode();
	}

	@Override protected void onResume() {
		super.onResume();
		enterImmersiveMode();
	}
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
}
