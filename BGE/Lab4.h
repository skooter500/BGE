#pragma once
#include "Game.h"
#include "GameComponent.h"
#include "FountainEffect.h"

#define NUM_FOUNTAINS 20
#define FOUNTAIN_RADIUS 40.0f
#define FOUNTAIN_HEIGHT 10.0f

using namespace std;

namespace BGE
{
	class Lab4 :
		public Game
	{
	public:
		Lab4(void);

		shared_ptr<GameComponent> ship1;
		shared_ptr<GameComponent> ship2;
		float elapsed;
		bool Initialise();
		void Update();
		vector<shared_ptr<FountainEffect>> fountains;
		float fountainTheta;
	};
}

