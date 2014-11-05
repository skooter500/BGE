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
			bool Initialise();
			void Update(float timeDelta);

			shared_ptr<GravityController> sgc, bgc;
			shared_ptr<Sphere> sphere;
			shared_ptr<Box> box;
	};
		
}