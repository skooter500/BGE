#include "AssignmentSample.h"
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

using namespace BGE;

AssignmentSample::AssignmentSample(void)
{			
}

AssignmentSample::~AssignmentSample(void)
{
}

shared_ptr<PhysicsController> blahh;

bool AssignmentSample::Initialise()
{
	physicsFactory->CreateGroundPhysics();
	physicsFactory->CreateCameraPhysics();

	setGravity(glm::vec3(0, -9, 0));

	// Loop to create a stack of ragdolls
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			blahh = physicsFactory->CreateCapsuleRagdoll(glm::vec3(i, j, 0));
		}
	}

	if (!Game::Initialise())
	{
		return false;
	}

	return true;
}

void BGE::AssignmentSample::Update()
{
	Game::Update();
}

void BGE::AssignmentSample::Cleanup()
{
	Game::Cleanup();
}