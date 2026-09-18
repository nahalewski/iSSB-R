
#include "Game.h"
#include "GameEngine/Input/Gamepad.h"
#include "SmashBros/Preferences.h"
#include "SmashBros/Controls.h"
#include "SmashBros/Menus/Menus.h"
#include "SmashBros/Game/GameScreen.h"

namespace SmashBros
{
	Game::Game()
	{
		//Constructor
		View::setSize(900,600);
		padCursorX=450;padCursorY=300;padCursorVisible=false;
		padPointerDown=false;padWaitRelease=true;padCursorTime=0;
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
		scaleToWindow(true,900,600);

		AssetManager::loadImage("Images/icon.png");
		setLoadScreen("Images/loading.png");
		
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
		setWindowTitle("ISSB-R");
	}
	
	void Game::LoadContent()
	{
		//Load Things
		Controls::loadControls();
		Preferences::init();
		Preferences::load();

		Menus::loadAssets();
		Menus::loadMenus();
		ScreenManager::Add(new GameScreen("Game"));
		ScreenManager::GoToScreen("TitleScreen");
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
			}
			firstUpdate = false;
		}
		if(getKeyPressed(Keys::ESCAPE) && !getPrevKeyPressed(Keys::ESCAPE))
		{
			Exit();
		}
		Gamepad& pads=Gamepad::get();
		String screen=ScreenManager::currentName();
		if(!screen.equals(padScreen)) {
			if(padPointerDown)controllerPointer(padCursorX,padCursorY,false);
			padPointerDown=false;padWaitRelease=true;padScreen=screen;
		}
		unsigned int now=SDL_GetTicks();
		float dt=padCursorTime ? (now-padCursorTime)/1000.0f : 0;
		if(dt>0.05f)dt=0.05f;
		padCursorTime=now;
		if(!screen.equals("Game")) {
			int slot=-1;
			for(int i=0;i<4;i++)if(pads.connected(i)) {
				if(slot<0)slot=i;
				if(pads.axis(i,0)!=0 || pads.axis(i,1)!=0 || pads.button(i,SDL_CONTROLLER_BUTTON_A)
					|| pads.button(i,SDL_CONTROLLER_BUTTON_B) || pads.button(i,SDL_CONTROLLER_BUTTON_DPAD_UP)
					|| pads.button(i,SDL_CONTROLLER_BUTTON_DPAD_DOWN) || pads.button(i,SDL_CONTROLLER_BUTTON_DPAD_LEFT)
					|| pads.button(i,SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) {slot=i;break;}
			}
			if(slot>=0) {
				float dx=pads.axis(slot,0),dy=pads.axis(slot,1);
				if(pads.button(slot,SDL_CONTROLLER_BUTTON_DPAD_LEFT))dx=-1;
				if(pads.button(slot,SDL_CONTROLLER_BUTTON_DPAD_RIGHT))dx=1;
				if(pads.button(slot,SDL_CONTROLLER_BUTTON_DPAD_UP))dy=-1;
				if(pads.button(slot,SDL_CONTROLLER_BUTTON_DPAD_DOWN))dy=1;
				padCursorX=std::fmax(5.0f,std::fmin(895.0f,padCursorX+dx*500*dt));
				padCursorY=std::fmax(5.0f,std::fmin(595.0f,padCursorY+dy*500*dt));
				bool confirm=pads.button(slot,SDL_CONTROLLER_BUTTON_A);
				if(!confirm)padWaitRelease=false;
				bool down=confirm&&!padWaitRelease;
				if(dx!=0 || dy!=0 || down || padPointerDown) {
					padCursorVisible=true;
					controllerPointer(padCursorX,padCursorY,down);
				}
				padPointerDown=down;
				if(pads.button(slot,SDL_CONTROLLER_BUTTON_B)&&!pads.button(slot,SDL_CONTROLLER_BUTTON_B,true)&&Menus::button_back) {
					if(padPointerDown)controllerPointer(padCursorX,padCursorY,false);
					padPointerDown=false;padWaitRelease=true;
					Menus::button_back->onRelease();
				}
			} else {
				if(padPointerDown)controllerPointer(padCursorX,padCursorY,false);
				padPointerDown=false;padCursorVisible=false;
			}
		} else padCursorVisible=false;
		ScreenManager::Update(gameTime);
		pads.finishUpdate();
	}
	
	void Game::Draw(Graphics2D& g, long gameTime)
	{
		ScreenManager::Draw(g, gameTime);
		if(padCursorVisible && !ScreenManager::currentName().equals("Game")) {
			g.setColor(Color::BLACK);g.fillRect(padCursorX-8,padCursorY-8,16,16);
			g.setColor(Color::WHITE);g.fillRect(padCursorX-5,padCursorY-5,10,10);
			g.setColor(Color::BLACK);
			g.setFont(AssetManager::getFont("Fonts/arial.ttf",Font::BOLD,13));
			g.drawString("Stick / D-pad: cursor    A / Cross: select or hold to drag    B / Circle: back",145,35);
		}
		drawnOnce = true;
	}
}
