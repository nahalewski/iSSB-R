
#include "StageSelectScreen.h"
#include "Menus.h"
#include "../Global.h"
#include "../Loader.h"

namespace SmashBros
{
	StageSelectScreen::StageIcon::StageIcon(StageSelectScreen*screen, float x1, float y1, int num) : Actor(x1,y1)
	{
		this->num = num;
		this->screen = screen;
	}

	StageSelectScreen::StageIcon::~StageIcon()
	{
		//
	}

	void StageSelectScreen::StageIcon::onMouseEnter()
	{
		setColor(Color::WHITE);
	}

	void StageSelectScreen::StageIcon::onMouseLeave()
	{
		setColor(Color::WHITE);
	}

	void StageSelectScreen::StageIcon::onRelease()
	{
		Global::selectedStage = num;
		MusicManager::stop();
		screen->queueLoad = 1;
		screen->onSelect(num);
	}

	StageSelectScreen::StageSelectScreen(const String&name) : Screen(name)
	{
		stageGrid = new ActorGrid(170,215, 2, 3);
		stageGrid->setSpacing(280, 210);
		stageGrid->setScale(1.0f);
		for(int i=0; i<=Global::totalStages; i++)
		{
			StageIcon*a = new StageIcon(this, 0,0, i);
			String sPath = "";
			
			sPath = StageLoader::getIconPath(i);
			
			a->addAnimation(new Animation("normal",1,sPath));
			a->changeAnimation("normal", FORWARD);
			stageGrid->add(a);
		}
		
		loadScreen = new Actor(450,300);
		loadScreen->addAnimation(new Animation("normal",1,"Images/loading.png"));
		loadScreen->changeAnimation("normal", FORWARD);
		loadScreen->setAlpha(0.0f);
		loadScreen->setScale(1.0f);
		
		queueLoad = 0;
		drawnOnce = false;
	}

	StageSelectScreen::~StageSelectScreen()
	{
		delete stageGrid;
		delete loadScreen;
	}

	void StageSelectScreen::onLoad()
	{
		//Open for implementation
		Menus::playSound("titlescreen click");
        Global::LoadGame();
	}
	
	void StageSelectScreen::onSelect(int stageNo)
	{
		//Open for implementation
	}
	
	void StageSelectScreen::onGoToGame()
	{
		//Open for implementation
		ScreenManager::GoToScreen("Game");
	}
	
#ifndef SMASHBROS_SCRIPT_DISABLE
	void StageSelectScreen::unloadScriptedStages()
	{
		stageGrid->remove(scriptStageIcons, false);
		for(int i=0; i<scriptStageIcons.size(); i++)
		{
			delete scriptStageIcons.get(i);
		}
		scriptStageIcons.clear();
	}
	
	void StageSelectScreen::reloadScriptedStages()
	{
		unloadScriptedStages();
		
		ArrayList<ScriptModule::ScriptEntityInfo*> scriptEntities = StageLoader::getScriptEntities();
		for(int i=0; i<scriptEntities.size(); i++)
		{
			ScriptModule::ScriptEntityInfo* info = scriptEntities.get(i);
			String iconPath = info->getPath() + '/' + info->getIcon();
			bool loadedIcon = AssetManager::loadImage(iconPath);
			if(!loadedIcon)
			{
				iconPath = "Images/Menus/StageSelect/default.png";
				AssetManager::loadImage(iconPath);
			}
			
			StageIcon* stageIcon = new StageIcon(this, 0,0, Global::totalStages+i+1);
			stageIcon->addAnimation(new Animation("normal",1,iconPath));
			stageIcon->changeAnimation("normal", FORWARD);
			scriptStageIcons.add(stageIcon);
			stageGrid->add(stageIcon);
		}
	}
#endif //SMASHBROS_SCRIPT_DISABLE
	
	void StageSelectScreen::LoadContent()
	{
		Screen::LoadContent();
		queueLoad = 0;
		drawnOnce = false;
        Menus::playSound("titlescreen click");
#ifndef SMASHBROS_SCRIPT_DISABLE
		reloadScriptedStages();
#endif //SMASHBROS_SCRIPT_DISABLE
	}
	
	void StageSelectScreen::UnloadContent()
	{
		Screen::UnloadContent();
		queueLoad = 0;
		drawnOnce = false;
#ifndef SMASHBROS_SCRIPT_DISABLE
		unloadScriptedStages();
#endif //SMASHBROS_SCRIPT_DISABLE
	}
	
	void StageSelectScreen::Update(long gameTime)
	{
		if(queueLoad>0 && !drawnOnce)return;
		if(drawnOnce)
		{
			if(queueLoad==1)
			{
				onLoad();
				queueLoad++;
			}
			else if(queueLoad == 2)
			{
				onGoToGame();
				queueLoad = 0;
				drawnOnce = false;
			}
		}
		stageGrid->Update(gameTime);
		Menus::button_back->Update(gameTime);
	}

	void StageSelectScreen::Draw(Graphics2D&g, long gameTime)
	{
		g.setColor(Color(243,244,246));g.fillRect(0,0,900,600);
		g.setColor(Color(22,24,30));g.fillRect(145,14,715,62);
		g.setColor(Color::WHITE);
		g.setFont(AssetManager::getFont("Fonts/arial.ttf",Font::BOLD,28));
		g.drawString("CHOOSE YOUR STAGE",165,56);
		stageGrid->Draw(g, gameTime);
		int hovered=stageGrid->getHoveredIndex();
		if(hovered>=0)
		{
			Actor* tile=stageGrid->get(hovered);
			g.setColor(Color(230,35,45));
			float left=tile->x-tile->width/2-3, top=tile->y-tile->height/2-3;
			g.fillRect(left,top,tile->width+6,4);
			g.fillRect(left,top+tile->height+2,tile->width+6,4);
			g.fillRect(left,top,4,tile->height+6);
			g.fillRect(left+tile->width+2,top,4,tile->height+6);
		}
		g.setColor(Color(55,57,65));
		g.setFont(AssetManager::getFont("Fonts/arial.ttf",Font::PLAIN,17));
		g.drawString("Choose an arena to begin the battle.",40,562);
		Menus::button_back->Draw(g, gameTime);
		if(queueLoad>0)
		{
			loadScreen->Draw(g, gameTime);
			drawnOnce = true;
		}
	}
}
