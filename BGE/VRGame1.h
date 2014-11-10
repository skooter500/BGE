#pragma once
#include "Game.h"
#include "PhysicsController.h"
#include "PhysicsFactory.h"
#include <btBulletDynamicsCommon.h>
#include <fmod.hpp>
#include "Person.h"

namespace BGE
{

	class VRGame1 :
		public Game
	{
	private:
		btBroadphaseInterface* broadphase;

		// Set up the collision configuration and dispatcher
		btDefaultCollisionConfiguration * collisionConfiguration;
		btCollisionDispatcher * dispatcher;

		// The actual physics solver
		btSequentialImpulseConstraintSolver * solver;

	public:
		VRGame1(void);
		~VRGame1(void);
		bool Initialise();
		void Update();
		void Cleanup();

		void FireProjectile(glm::vec3 pos, glm::vec3 look);
		//void GravityGun(SDL_Joystick * joy, int axis, PhysicsController * & leftHandPickedUp, Hand hand);
		void ResetScene();
		
		// The world.
		//void VRGame1::GravityGun(PhysicsController * & pickedUp, KinectHand * hand);
		
		// Now stuff we need to track
		shared_ptr<Person> person;
		PhysicsController * leftHandPickedUp;
		PhysicsController * rightHandPickedUp;

		float fireRate;
	};
}
