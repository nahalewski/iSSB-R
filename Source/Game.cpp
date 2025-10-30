
#include "Game.h"
#include "SmashBros/Preferences.h"
#include "SmashBros/Controls.h"
#include "SmashBros/Menus/Menus.h"
#include "SmashBros/Game/GameScreen.h"
#include "SmashBros/AndroidBridge.h"

namespace SmashBros
{
	Game::Game()
	{
		//Constructor
		// Don't set a fixed size here - let it use the actual device resolution
		// View::setSize(900,600);
		firstUpdate = true;
		drawnOnce = false;
	}
	
	Game::~Game()
	{
		//
	}
	
	void Game::Initialize()
	{
		/*Font* font = new Font("Fonts/arial.ttf",Font::PLAIN,16);
		SDL_Color color;
		color.r = 255;
		color.g = 255;
		color.b = 255;
		color.a = 255;

		SDL_Color bgcolor;
		bgcolor.r = 0;
		bgcolor.g = 0;
		bgcolor.b = 0;
		bgcolor.a = 0;

		SDL_Surface* surface = TTF_RenderGlyph_Shaded(font->getTTF(), 'H', color, bgcolor);
		Console::WriteLine((String)"H: " + surface->w + ", " + surface->h);
		SDL_FreeSurface(surface);
		surface = TTF_RenderGlyph_Shaded(font->getTTF(), 'y', color, bgcolor);
		Console::WriteLine((String)"y: " + surface->w + ", " + surface->h);
		SDL_FreeSurface(surface);
		surface = TTF_RenderGlyph_Shaded(font->getTTF(), 'r', color, bgcolor);
		Console::WriteLine((String)"r: " + surface->w + ", " + surface->h);
		SDL_FreeSurface(surface);
		surface = TTF_RenderGlyph_Shaded(font->getTTF(), '.', color, bgcolor);
		Console::WriteLine((String)".: " + surface->w + ", " + surface->h);
		SDL_FreeSurface(surface);*/

		//Initialize things
		// Enable scaling with 900x600 logical resolution
		// View will be modified to stretch to fill screen without letterboxing
		scaleToWindow(true, 900, 600);

	AssetManager::loadImage("Images/icon.png");
	// No loading screen needed - we load directly to game or menus
	// setLoadScreen("Images/loading.png");
		
		Global::init();
		
		#ifdef DEBUG
			showRealFPS(true);
		#endif
		if(Preferences::highFPS())
		{
			setFPS(60);
			setUpdatesPerFrame(1);
		}
		else
		{
			setFPS(30);
			setUpdatesPerFrame(2);
		}
		//setWindowTitle("Super Smash Bros.");
	}
	
	void Game::LoadContent()
	{
		//Load Things
		Controls::loadControls();
		Preferences::init();
		Preferences::load();
		// Apply settings chosen in WebView (game mode, stock, etc.)
		Preferences::applyWebSettings();
		
		// Load touch controls setting from WebView preferences
		bool touchEnabled = AndroidBridge::getTouchControlsSetting();
		AndroidBridge::setTouchControlsEnabled(touchEnabled);
		Console::WriteLine((String)"Touch controls: " + (touchEnabled ? "ON" : "OFF"));

		// Add the game screen (always needed)
		ScreenManager::Add(new GameScreen("Game"));
		
		// Check if we should skip C++ menus and go straight to game
		if(AndroidBridge::shouldSkipMenus())
		{
			int p1Char = AndroidBridge::getP1Character();
			int p2Char = AndroidBridge::getP2Character();
			int selectedStage = AndroidBridge::getSelectedStage();
			
			Console::WriteLine((String)"WebView Launch - P1:" + p1Char + " P2:" + p2Char + " Stage:" + selectedStage);
			
			// Clamp character selections to valid range (1 to totalCharacters)
			if(p1Char < 1 || p1Char > Global::totalCharacters) { 
				Console::WriteLine("P1 invalid from WebView - defaulting to 1 (Mario)"); 
				p1Char = 1; 
			}
			if(p2Char < 1 || p2Char > Global::totalCharacters) { 
				Console::WriteLine("P2 invalid from WebView - defaulting to 2"); 
				p2Char = 2; 
			}
			// Clamp stage selection to valid range (0 to totalStages-1)
			if(selectedStage < 0 || selectedStage >= Global::totalStages) { 
				Console::WriteLine("Stage invalid from WebView - defaulting to 0"); 
				selectedStage = 0; 
			}
			
			// Set up game with WebView menu selections
			if(p1Char > 0 && p2Char > 0)
			{
				// Fix: selectedChar array is 0-indexed (P1 at index 0, P2 at index 1)
				Global::selectedChar[0] = p1Char;
				Global::selectedChar[1] = p2Char;
				Global::charAmount = 2;
				Global::selectedStage = selectedStage; // Use stage from WebView
				Global::gameType = Global::TYPE_TRAINING; // Training mode
				
				// Set CPU flags: both players are human in training mode
				Global::CPU[0] = false; // P1 is human
				Global::CPU[1] = false; // P2 is human (training mode)
				
				Console::WriteLine("Starting fight directly - bypassing all C++ menus");
				
				// Load the game: create stage and players before transitioning
				Global::LoadGame();
				
				// Clear the skip flag now that we've successfully loaded
				AndroidBridge::clearCharacterSelections();
				
				// Go DIRECTLY to game screen - NO LOADING SCREEN, NO C++ MENUS
				ScreenManager::GoToScreen("Game");
			}
			else
			{
			// If selections are invalid, send user back to NEW WebView character select
			Console::WriteLine("ERROR: Invalid character selections - returning to WebView Character Select");
			AndroidBridge::returnToCharacterSelect();
			}
		}
		else
		{
			// Only load and show C++ menus if NOT launched from WebView (for debugging)
			Console::WriteLine("No WebView launch - loading C++ menus");
			Menus::loadAssets();
			Menus::loadMenus();
			ScreenManager::GoToScreen("TitleScreen");
		}
		
		if(Preferences::debuglog)
		{
			Console::OutputToFile(true, "iSSB.log");
		}
	}
	
	void Game::UnloadContent()
	{
		//
	}
	
	void Game::Update(long gameTime)
	{
		if(firstUpdate && drawnOnce)
		{
			if(Preferences::newVersion)
			{
				Preferences::newVersion = false;
				Game::showMessage((String)"iSSB "+Preferences::version, Preferences::versionMessage);
			}
			firstUpdate = false;
		}
		if(getKeyPressed(Keys::ESCAPE) && !getPrevKeyPressed(Keys::ESCAPE))
		{
			Exit();
		}
		ScreenManager::Update(gameTime);
	}
	
	void Game::Draw(Graphics2D& g, long gameTime)
	{
		ScreenManager::Draw(g, gameTime);
		drawnOnce = true;
	}
}
