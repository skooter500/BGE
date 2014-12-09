#pragma once
#include "Game.h"
#include "Box.h"
#include "Sphere.h"
#include "GravityController.h"
#include "Game.h"
#include "GameComponent.h"

namespace BGE
{
	class GravityGame :
		public Game
	{
	public:
		GravityGame();
		~GravityGame();

		bool Initialise();

		void Update();

	};
}
