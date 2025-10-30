#pragma once

namespace SmashBros
{
	namespace AndroidBridge
	{
		// Get character selections from WebView menus
		int getP1Character();
		int getP2Character();
		
		// Check if we should skip C++ menus
		bool shouldSkipMenus();
		
		// Clear character selections
		void clearCharacterSelections();
		
		// Set touch controls enabled/disabled
		void setTouchControlsEnabled(bool enabled);
		
		// Get touch controls setting from preferences
		bool getTouchControlsSetting();
		
		// Get selected stage from WebView
		int getSelectedStage();

		// Settings from WebView
		int getGameMode();
		int getItemFrequency();
		int getStockCount();
		bool getStageHazards();
		bool getFinalSmashMeter();
		
		// Audio settings from WebView
		bool getMenuMusicSetting();
		bool getMenuSoundFxSetting();
		bool getInGameMusicSetting();
		
		// Return to WebView character select menu
		void returnToCharacterSelect();
	}
}

