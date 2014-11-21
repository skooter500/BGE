#include "PhysicsGame1.h"
#include "PhysicsController.h"
#include "PhysicsCamera.h"
#include "Box.h"
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
	dynamicsWorld->setGravity( btVector3(0, 0, 0));

	
	float width = 20.0f, height = 10.0f, depth = 2.0f;
	int numBlocks = 32, level = height / 2, rowSize = 6;

	for (int i = 1; i < numBlocks + 1; i++)
	{
		if (i % (rowSize + 1) == 0)
		{
			level += height;
			i = 1;
			numBlocks -= rowSize;
		}

		physicsFactory->CreateBox(width, height, depth, glm::vec3(width*i, level, -width), glm::quat());
	}

	physicsFactory->CreateSpider(glm::vec3(0, 10, 0));

	if (!Game::Initialise()) {
		return false;
	}

	camera->transform->position = glm::vec3(0, 10, 20);

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
