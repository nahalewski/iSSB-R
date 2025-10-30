
#include "View.h"
#include "Application.h"
#include "Output/Console.h"
#include <math.h>
#include <cmath>

namespace GameEngine
{
	float View::x = 0;
	float View::y = 0;
	
	float View::Zoom = 1.0;

	int View::oldWidth = 480;
	int View::oldHeight = 320;
	
	int View::windowWidth = 480;
	int View::windowHeight = 320;
	
	int View::scaleWidth = 480;
	int View::scaleHeight = 320;
	
	float View::multScale = 1;
	float View::scaleX = 1;
	float View::scaleY = 1;
	float View::letterBoxW = 0;
	float View::letterBoxH = 0;

	void View::setSize(int w, int h)
	{
		windowWidth = w;
		windowHeight = h;
		if(Application::window != NULL)
		{
			SDL_SetWindowSize(Application::window, w, h);
		}
	}
	
	void View::setScaleSize(int w, int h)
	{
		scaleWidth=w;
		scaleHeight=h;
	}
	
	int View::getWidth()
	{
		return windowWidth;
	}
	
	int View::getHeight()
	{
		return windowHeight;
	}
	
	int View::getScalingWidth()
	{
		return scaleWidth;
	}
	
	int View::getScalingHeight()
	{
		return scaleHeight;
	}
	
	void View::Update(Graphics2D&g)
	{
		multScale = 1;
		scaleX = 1;
		scaleY = 1;
		
		if(Application::scalescreen)
		{
			// Calculate separate scale factors for X and Y to stretch and fill the screen
			scaleX = (float)windowWidth/(float)scaleWidth;
			scaleY = (float)windowHeight/(float)scaleHeight;
			// Use the average for multScale (for backward compatibility)
			multScale = (scaleX + scaleY) / 2.0f;
		}
		
		// Use non-uniform scaling to fill the screen without letterboxing
		g.scale(Zoom*scaleX, Zoom*scaleY);
		
		float difX;
		float difY;
		if(Application::scalescreen)
		{
			// No letterboxing needed since we're stretching to fill
			difX = 0;
			difY = 0;
			letterBoxW = 0;
			letterBoxH = 0;
		}
		else
		{
			difX = (float)(windowWidth - (windowWidth*Zoom))/(float)(2*Zoom);
			difY = (float)(windowHeight - (windowHeight*Zoom))/(float)(2*Zoom);
			letterBoxW = 0;
			letterBoxH = 0;
		}
		g.translate(difX,difY);
	}
	
	void View::Draw(Graphics2D&g)
	{
		g.setScale(1,1);
		g.setTranslation(0,0);
		
		// No letterboxing to draw since we're stretching to fill the screen
		if(Application::showfps)
		{
			g.setColor(Color::BLACK);
			g.setFont(g.defaultFont);
			if(Application::scalescreen)
			{
				g.drawString((String)Application::realFPS + (String)" fps", (30*multScale), (50*multScale));
			}
			else
			{
				g.drawString((String)Application::realFPS + (String)" fps", 30, 50);
			}
		}
	}
}