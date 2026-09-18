
#include "GameEngine/GameEngine.h"

#pragma once

namespace SmashBros
{
	class Game : public GameEngine::Application
	{
		private:
			bool firstUpdate;
			bool drawnOnce;
			float padCursorX, padCursorY;
			bool padCursorVisible, padPointerDown, padWaitRelease;
			unsigned int padCursorTime;
			String padScreen;
		public:
			Game();
			virtual ~Game();

			void Initialize();
			void LoadContent();
			void UnloadContent();
			void Update(long gameTime);
			void Draw(Graphics2D& g, long gameTime);
	};
}
