#pragma once
#include "Game.h"
#include "PhysicsController.h"
#include "PhysicsFactory.h"
#include <btBulletDynamicsCommon.h>
#include "Sphere.h"

namespace BGE
{
	class Spider :
		public Game
	{
	private:
		std::list<shared_ptr<PhysicsController>> front_legs;
		std::list<shared_ptr<PhysicsController>>::iterator it_front_legs;
		std::list<shared_ptr<PhysicsController>> hind_legs;
		std::list<shared_ptr<PhysicsController>>::iterator it_hind_legs;
		std::list<btSliderConstraint*> slider_group_1;
		std::list<btSliderConstraint*>::iterator it_group_1;
		std::list<btSliderConstraint*> slider_group_2;
		std::list<btSliderConstraint*>::iterator it_group_2;

		static const int FRONT_RIGHT = 1, MID_FRONT_RIGHT = 2, MID_HIND_RIGHT = 4, HIND_RIGHT = 5,
			HIND_LEFT = 7, MID_HIND_LEFT = 8, MID_FRONT_LEFT = 10, FRONT_LEFT = 11;

		shared_ptr<PhysicsController> body;
		void reColour(shared_ptr<GameComponent>, float, float, float);
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
		Spider(void);
		~Spider(void);
		bool Initialise();
		void Update();
		void Cleanup();
		shared_ptr<PhysicsController> createSpider(glm::vec3);
		shared_ptr<PhysicsController> createLeg(glm::vec3, glm::vec3, glm::quat, bool, bool);
		shared_ptr<GameComponent> sphere;
		shared_ptr<GameComponent> box;

	};
}