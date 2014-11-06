#include "PhysicsGame1.h"
#include "PhysicsController.h"
#include "Sphere.h"
#include "PhysicsCamera.h"
#include "Box.h"
#include "Cylinder.h"
#include "Steerable3DController.h"
#include "Ground.h"
#include "Content.h"
#include <btBulletDynamicsCommon.h>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <gtx/euler_angles.hpp>
#include <gtx/norm.hpp>
#include "VectorDrawer.h"
#include "Utils.h"
#include "Assignment.h"

using namespace BGE;

Assignment::Assignment(void)
{
}

Assignment::~Assignment(void)
{
}

bool Assignment::Initialise()
{
	physicsFactory->CreateGroundPhysics();
	physicsFactory->CreateCameraPhysics();
	
	shared_ptr<PhysicsController> box1 = physicsFactory->CreateBox(1, 1, 4, glm::vec3(5, 5, 0), glm::quat());
	return true;
}

void Assignment::Update(float timeDelta)
{
	Game::Update(timeDelta);
}

void Assignment::Cleanup()
{
	Game::Cleanup();
}

void Assignment::CreateWall()
{

}
