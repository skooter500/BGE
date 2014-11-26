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

#include "Script.h"

using namespace BGE;

PhysicsGame1::PhysicsGame1(void)
{
}

PhysicsGame1::~PhysicsGame1(void)
{
}

shared_ptr<PhysicsController> cyl;
std::shared_ptr<GameComponent> station;
shared_ptr<PhysicsController> box1;

bool PhysicsGame1::Initialise() 
{	
	physicsFactory->CreateGroundPhysics();
	//physicsFactory->CreateCameraPhysics();	

	setGravity(glm::vec3(0, -0, 0));

	shared_ptr<Box> obj1 = make_shared<Box>(5, 5, 5);
	obj1->EnableScripting();
	obj1->Attach(make_shared<Script>("roll.lua", obj1.get()));
	obj1->Attach(make_shared<Script>("init.lua", obj1.get()));
	obj1->Attach(make_shared<Script>("template.lua", obj1.get()));
	Attach(obj1);

	this->EnableScripting();
	Attach(make_shared<Script>("template.lua", this));
	//Attach(make_shared<Script>("roll.lua", this));

	if (!Game::Initialise()) {
		return false;
	}


	
	return true;
}

void BGE::PhysicsGame1::Update()
{
	//cyl->rigidBody->applyTorque(GLToBtVector(glm::vec3(0.0f,0.0f,1.0f)));
	Game::Update();
}

void BGE::PhysicsGame1::Cleanup()
{
	Game::Cleanup();
}
