#pragma once
#include <memory>
#include "PhysicsController.h"
#include "GameComponent.h"

using namespace std;

namespace BGE
{
	class PhysicsFactory
	{
	private:
		
	public:
		PhysicsFactory(btDiscreteDynamicsWorld * dynamicsWorld);
		~PhysicsFactory(void);

		shared_ptr<PhysicsController> CreateBox(float width, float height, float depth, glm::vec3 pos, glm::quat quat, bool kinematic = false, bool attachToGame = true);
		shared_ptr<PhysicsController> CreateSphere(float radius, glm::vec3 pos, glm::quat quat, bool kinematic = false, bool attachToGame = true);
		shared_ptr<PhysicsController> CreateCylinder(float radius, float height, glm::vec3 pos, glm::quat quat, bool kinematic = false, bool attachToGame = true);
		shared_ptr<PhysicsController> CreateCapsule(float radius, float height, glm::vec3 pos, glm::quat quat);
		shared_ptr<PhysicsController> CreateCapsuleRagdoll(glm::vec3 position);
		shared_ptr<PhysicsController> CreateVehicle(glm::vec3 pos);
		shared_ptr<PhysicsController> CreateCameraPhysics();
		shared_ptr<PhysicsController> CreateGroundPhysics();
		shared_ptr<PhysicsController> CreateFromModel(string name, glm::vec3 pos, glm::quat quat, glm::vec3 scale = glm::vec3(1));
		
		void CreateWall(glm::vec3 startAt, float width, float height, float blockWidth = 5, float blockHeight = 5, float blockDepth = 5);
		shared_ptr<PhysicsController> CreateRandomObject(glm::vec3 point, glm::quat q, glm::vec3 scale = glm::vec3(1));
		btDiscreteDynamicsWorld * dynamicsWorld;
	};
}
