package com.brokenphysics.issb;

import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.webkit.JavascriptInterface;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.view.WindowCompat;
import androidx.core.view.WindowInsetsControllerCompat;

/**
 * MenuActivity handles all menu screens using WebView for modern UI
 * This allows us to use HTML/CSS for beautiful, responsive menus
 * while keeping SDL for the actual game rendering
 */
public class MenuActivity extends AppCompatActivity {
    private WebView webView;
    private String currentMenu = "main_menu";
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        // Enable fullscreen
        setupFullscreen();
        
        // Create and configure WebView
        webView = new WebView(this);
        setContentView(webView);
        
        // Configure WebView settings
        WebSettings webSettings = webView.getSettings();
        webSettings.setJavaScriptEnabled(true);
        webSettings.setDomStorageEnabled(true);
        webSettings.setAllowFileAccess(true);
        webSettings.setAllowContentAccess(true);
        
        // Add JavaScript interface for menu interactions
        webView.addJavascriptInterface(new MenuBridge(), "MenuBridge");
        
        // Set WebView client to handle navigation
        webView.setWebViewClient(new WebViewClient() {
            @Override
            public boolean shouldOverrideUrlLoading(WebView view, String url) {
                // Handle internal navigation
                if (url.startsWith("menu://")) {
                    handleMenuNavigation(url);
                    return true;
                }
                return false;
            }
        });
        
        // Load the initial menu
        String menuToLoad = getIntent().getStringExtra("menu");
        if (menuToLoad != null) {
            currentMenu = menuToLoad;
        }
        loadMenu(currentMenu);
    }
    
    private void setupFullscreen() {
        WindowCompat.setDecorFitsSystemWindows(getWindow(), false);
        
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            WindowInsetsControllerCompat controller = 
                WindowCompat.getInsetsController(getWindow(), getWindow().getDecorView());
            if (controller != null) {
                controller.hide(androidx.core.view.WindowInsetsCompat.Type.systemBars());
                controller.setSystemBarsBehavior(
                    WindowInsetsControllerCompat.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE
                );
            }
        }
    }
    
    private void loadMenu(String menuName) {
        String htmlFile = "file:///android_asset/menus/" + menuName + ".html";
        webView.loadUrl(htmlFile);
    }
    
    private void handleMenuNavigation(String url) {
        // Parse menu:// URLs
        String action = url.substring(7); // Remove "menu://"
        
        switch (action) {
            case "start_game":
                startGame();
                break;
            case "character_select":
                loadMenu("character_select");
                break;
            case "settings":
                loadMenu("settings");
                break;
            case "multiplayer":
                loadMenu("multiplayer");
                break;
            case "back":
                onBackPressed();
                break;
            default:
                if (action.startsWith("navigate_")) {
                    String menu = action.substring(9);
                    loadMenu(menu);
                }
                break;
        }
    }
    
    private void startGame() {
        // Get character and stage selections
        android.content.SharedPreferences prefs = getSharedPreferences("iSSB_GameData", MODE_PRIVATE);
        int p1 = prefs.getInt("p1_character", -1);
        int p2 = prefs.getInt("p2_character", -1);
        int stage = prefs.getInt("selected_stage", 0);
        
        android.util.Log.d("MenuActivity", "Starting game - P1:" + p1 + " P2:" + p2 + " Stage:" + stage);
        
        // Set flag to skip C++ menus and go straight to game
        android.content.SharedPreferences.Editor editor = prefs.edit();
        editor.putBoolean("skip_cpp_menus", true);
        editor.apply();
        
        // Launch SDL game activity with NEW_TASK flag to ensure fresh start
        Intent intent = new Intent(this, MainActivity.class);
        intent.putExtra("launch_game", true);
        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TOP);
        
        startActivity(intent);
        // Finish this activity so back button doesn't return here mid-game
        finish();
    }
    
    @Override
    public void onBackPressed() {
        if (webView.canGoBack()) {
            webView.goBack();
        } else {
            super.onBackPressed();
        }
    }
    
    /**
     * JavaScript interface bridge for menu interactions
     */
    public class MenuBridge {
        private int p1CharacterId = -1;
        private int p2CharacterId = -1;
        
        @JavascriptInterface
        public void navigateTo(String menu) {
            runOnUiThread(() -> loadMenu(menu));
        }
        
                @JavascriptInterface
                public void startGame() {
                    runOnUiThread(() -> {
                        // Set flag to skip C++ menus
                        android.content.SharedPreferences prefs = getSharedPreferences("iSSB_GameData", MODE_PRIVATE);
                        android.content.SharedPreferences.Editor editor = prefs.edit();
                        editor.putBoolean("skip_cpp_menus", true);
                        editor.apply();
                        
                        MenuActivity.this.startGame();
                    });
                }
        
        @JavascriptInterface
        public void selectCharacter(int playerNum, int characterId) {
            // Store character selection for when game starts
            if (playerNum == 1) {
                p1CharacterId = characterId;
            } else if (playerNum == 2) {
                p2CharacterId = characterId;
            }
            android.util.Log.d("MenuBridge", "Player " + playerNum + " selected character " + characterId);
            
            // Store in SharedPreferences for game to read
            android.content.SharedPreferences prefs = getSharedPreferences("iSSB_GameData", MODE_PRIVATE);
            android.content.SharedPreferences.Editor editor = prefs.edit();
            editor.putInt("p" + playerNum + "_character", characterId);
            editor.apply();
        }
        
        @JavascriptInterface
        public void selectStage(int stageId) {
            // Store stage selection
            android.util.Log.d("MenuBridge", "Selected stage " + stageId);
            
            // Store in SharedPreferences for game to read
            android.content.SharedPreferences prefs = getSharedPreferences("iSSB_GameData", MODE_PRIVATE);
            android.content.SharedPreferences.Editor editor = prefs.edit();
            editor.putInt("selected_stage", stageId);
            editor.apply();
        }
        
                @JavascriptInterface
                public void updateSetting(String key, String value) {
                    // Update game settings in SharedPreferences
                    android.util.Log.d("MenuBridge", "Setting " + key + " = " + value);
                    
                    android.content.SharedPreferences prefs = getSharedPreferences("iSSB_Settings", MODE_PRIVATE);
                    android.content.SharedPreferences.Editor editor = prefs.edit();
                    editor.putString(key, value);
                    editor.apply();
                    
                    // Settings will be loaded when the game starts
                }
        
        @JavascriptInterface
        public String getCharacterList() {
            // Return JSON list of available characters
            // 7 unlocked characters + 5 locked slots = 12 total roster spots
            return "[" +
                "{\"id\":1,\"name\":\"Mario\",\"icon\":\"mario.png\",\"locked\":false}," +
                "{\"id\":2,\"name\":\"Ichigo\",\"icon\":\"ichigo.png\",\"locked\":false}," +
                "{\"id\":3,\"name\":\"Sonic\",\"icon\":\"sonic.png\",\"locked\":false}," +
                "{\"id\":4,\"name\":\"Fox\",\"icon\":\"fox.png\",\"locked\":false}," +
                "{\"id\":5,\"name\":\"Pikachu\",\"icon\":\"pikachu.png\",\"locked\":false}," +
                "{\"id\":6,\"name\":\"Link\",\"icon\":\"link.png\",\"locked\":false}," +
                "{\"id\":7,\"name\":\"Paper Mario\",\"icon\":\"papermario.png\",\"locked\":false}," +
                "{\"id\":8,\"name\":\"???\",\"icon\":\"locked.png\",\"locked\":true}," +
                "{\"id\":9,\"name\":\"???\",\"icon\":\"locked.png\",\"locked\":true}," +
                "{\"id\":10,\"name\":\"???\",\"icon\":\"locked.png\",\"locked\":true}," +
                "{\"id\":11,\"name\":\"???\",\"icon\":\"locked.png\",\"locked\":true}," +
                "{\"id\":12,\"name\":\"???\",\"icon\":\"locked.png\",\"locked\":true}" +
            "]";
        }
        
        @JavascriptInterface
        public String getSettings() {
            // Return current settings as JSON
            android.content.SharedPreferences prefs = getSharedPreferences("iSSB_Settings", MODE_PRIVATE);
            return "{" +
                "\"gameMode\":\"" + prefs.getString("gameMode", "time") + "\"," +
                "\"itemFrequency\":\"" + prefs.getString("itemFrequency", "2") + "\"," +
                "\"stockCount\":\"" + prefs.getString("stockCount", "3") + "\"," +
                "\"stageHazards\":\"" + prefs.getString("stageHazards", "true") + "\"," +
                "\"finalSmashMeter\":\"" + prefs.getString("finalSmashMeter", "false") + "\"," +
                "\"touchControls\":\"" + prefs.getString("touchControls", "false") + "\"," +
                "\"menuMusic\":\"" + prefs.getString("menuMusic", "true") + "\"," +
                "\"musicVolume\":\"" + prefs.getString("musicVolume", "70") + "\"," +
                "\"soundEffects\":\"" + prefs.getString("soundEffects", "true") + "\"," +
                "\"ingameMusic\":\"" + prefs.getString("ingameMusic", "true") + "\"" +
            "}";
        }
    }
}

