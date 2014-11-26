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
		std::list<btSliderConstraint*> sliders;
		std::list<btSliderConstraint*>::iterator it;
		float openingDuration;
		float speed;
		float legDirection;
		float counter;

	public:
		Assignment(void);
		~Assignment(void);
		bool Initialise();
		void Update(float timeDelta);
		void Cleanup();
		void CreateSpider(glm::vec3);
		shared_ptr<PhysicsController> Assignment::createLeg(glm::vec3, glm::vec3, glm::quat);
		shared_ptr<GameComponent> sphere;
		shared_ptr<GameComponent> box;

	};
}