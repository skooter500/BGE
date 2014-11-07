#pragma once
#include "Game.h"
#include "PhysicsController.h"
#include "PhysicsFactory.h"
#include <btBulletDynamicsCommon.h>
#include "Sphere.h"

namespace BGE
{
	class Assignment :
		public Game
	{
	private:


	public:
		Assignment(void);
		~Assignment(void);
		bool Initialise();
		void Update(float timeDelta);
		void Cleanup();
		void CreateWall();
		shared_ptr<GameComponent> sphere;
		shared_ptr<GameComponent> box;

	};
}