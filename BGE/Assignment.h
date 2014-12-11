#pragma once
#include "Game.h"
#include "PhysicsController.h"
#include "PhysicsFactory.h"
#include <btBulletDynamicsCommon.h>

namespace BGE
{

	class Assignment :
		public Game
	{
	public:
		Assignment(void);
		~Assignment(void);
		bool Initialise(void);
		void Update(void);
		shared_ptr<PhysicsController> CreateTripod(glm::vec3);
	};
}
