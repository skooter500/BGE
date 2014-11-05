#pragma once
#include "PhysicsFactory.h"
#include "Game.h"
#include "GameComponent.h"

namespace BGE
{
	class GravityController : 
			public GameComponent
	{
		public: GravityController();

		glm::vec3 gravity;
				
		void Update(float);
			
	};
}