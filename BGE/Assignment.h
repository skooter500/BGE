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
		std::list<btConeTwistConstraint*> coxa_group_1;
		std::list<btConeTwistConstraint*>::iterator it;
		std::list<btConeTwistConstraint*> coxa_group_2;
		std::list<btConeTwistConstraint*>::iterator it_2;
		std::list<btSliderConstraint*> slider_group_1;
		std::list<btSliderConstraint*>::iterator it_group_1;
		std::list<btSliderConstraint*> slider_group_2;
		std::list<btSliderConstraint*>::iterator it_group_2;
		float openingDuration;
		int num_legs;
		float speed;
		float legDirection;
		float counter;
		float leg_offset;
		float muscle_offset;

	public:
		Assignment(void);
		~Assignment(void);
		bool Initialise();
		void Update(float timeDelta);
		void Cleanup();
		void CreateSpider(glm::vec3);
		shared_ptr<PhysicsController> Assignment::createLeg(glm::vec3, glm::vec3, glm::quat, bool);
		shared_ptr<GameComponent> sphere;
		shared_ptr<GameComponent> box;

	};
}