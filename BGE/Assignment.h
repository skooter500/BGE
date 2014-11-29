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
		std::list<shared_ptr<PhysicsController>> leg_group_1;
		std::list<shared_ptr<PhysicsController>>::iterator it_leg_1;
		std::list<shared_ptr<PhysicsController>> leg_group_2;
		std::list<shared_ptr<PhysicsController>>::iterator it_leg_2;
		std::list<btSliderConstraint*> slider_group_1;
		std::list<btSliderConstraint*>::iterator it_group_1;
		std::list<btSliderConstraint*> slider_group_2;
		std::list<btSliderConstraint*>::iterator it_group_2;
		shared_ptr<PhysicsController> body;
		void Assignment::reColour(shared_ptr<GameComponent>, float, float, float);
		void animateLegs(float);
		float movementDuration;
		float openingDuration;
		int num_legs;
		float speed;
		float legDirection;
		float counter;
		float leg_offset;
		float muscle_offset;
		float direction_duration;
				

	public:
		Assignment(void);
		~Assignment(void);
		bool Initialise();
		void Update(float timeDelta);
		void Cleanup();
		shared_ptr<PhysicsController> CreateSpider(glm::vec3);
		shared_ptr<PhysicsController> Assignment::createLeg(glm::vec3, glm::vec3, glm::quat, bool);
		shared_ptr<GameComponent> sphere;
		shared_ptr<GameComponent> box;

	};
}