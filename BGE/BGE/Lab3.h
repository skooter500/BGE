#pragma once
#include "Game.h"
#include "GameComponent.h"

using namespace std;

namespace BGE
{
	class Lab3 :
		public Game
	{
	public:
		Lab3(void);

		shared_ptr<GameComponent> ship1;
		shared_ptr<GameComponent> ship2;
		float elapsed;
		bool Initialise();
		void Update();
	};
}

