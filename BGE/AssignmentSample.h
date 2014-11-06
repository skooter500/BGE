#pragma once
#include "Game.h"
#include "PhysicsController.h"
#include "PhysicsFactory.h"
#include <btBulletDynamicsCommon.h>

namespace BGE
{
	class AssignmentSample :
		public Game
	{
	private:

	public:
		AssignmentSample(void);
		~AssignmentSample(void);
		bool Initialise();
		void Update();
		void Cleanup();
	};
}