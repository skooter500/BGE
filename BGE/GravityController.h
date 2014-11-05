#pragma once
#include "PhysicsFactory.h"

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