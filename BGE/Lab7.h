#pragma once
#include "Game.h"
#include "GameComponent.h"
#include "FountainEffect.h"

using namespace std;

namespace BGE
{
	class Lab7 :
		public Game
	{
	public:
		Lab7(void);
		~Lab7(void);

		shared_ptr<GameComponent> ship1;
		float elapsed;
		bool Initialise();
		void Update();

		glm::vec3 force;
		float mass;
	};
}

