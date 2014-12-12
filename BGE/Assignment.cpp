#include "Assignment.h"
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

Assignment::Assignment()
{
}


Assignment::~Assignment()
{
}

float MaxMotor=30.0f;
float MaxMotor2 = 3.0f;
bool IsEnabled = true;
float LowLimit = -1.5f;
float UpLimit = 1.5f;
float low, up;
bool Assignment::Initialise()
{
	physicsFactory->CreateGroundPhysics();
	physicsFactory->CreateCameraPhysics();
	setGravity(glm::vec3(0, -7, 0));

	//Create a stampede of Horses!
	/*for (int i = 0; i < 5; i++)
	{
		CreateHorse(30*i, 10);
	}
	for (int i = 0; i < 5; i++)
	{
		CreateWheeledHorse(40*i,-30);
	}*/

	CreateHorse(-20, 10);
	CreateWheeledHorse(20, -30);
	
	
	return Game::Initialise();
}
float timer=0;
void BGE::Assignment::Update()
{
	PrintFloat("Speed ", MaxMotor2);
	if (keyState[SDL_SCANCODE_UP])
	{
		up = 0;
		low = 1;
		MaxMotor2 += 1.0f;
	}
	if (keyState[SDL_SCANCODE_DOWN])
	{
		low = 0;
		up = 0;
		
	}
	
	Game::Update();
}

void BGE::Assignment::Cleanup()
{
	Game::Cleanup();
}

void BGE::Assignment::CreateHorse(float x, float z)
{
	glm::vec3 Brown(1, 0.5f, 0);
	glm::vec3 Yellow(2.5f, 2.5f, 0);
	glm::quat q = glm::angleAxis(90.0f, glm::vec3(1, 0, 0));
	glm::quat q2 = glm::angleAxis(30.0f, glm::vec3(1, 0, 0));

	//Create Walking Horse
	//Create Body
	shared_ptr<PhysicsController> body = physicsFactory->CreateCapsule(4, 5, glm::vec3(x, 15, z), q);
	body->transform->diffuse = Brown;

	//Create Head
	shared_ptr<PhysicsController> head = physicsFactory->CreateCapsule(2.5f, 2.5f, glm::vec3(x, 15, z), q);
	shared_ptr<PhysicsController> neck = physicsFactory->CreateCylinder(2, 4, glm::vec3(x, 15, z), q);
	head->transform->diffuse = Brown;
	neck->transform->diffuse = Brown;
	btTransform t1, t2, t3, t4;
	t1.setIdentity();
	t2.setIdentity();
	t1.setOrigin(btVector3(2, 2, 0));
	t2.setRotation(GLToBtQuat(glm::angleAxis(-100.0f, glm::vec3(2, 0, 0)))); // Not sure but this needs to be a minus angle
	t2.setOrigin(btVector3(2, 4, 0));
	btFixedConstraint * fixed = new btFixedConstraint(*head->rigidBody, *neck->rigidBody, t1, t2);
	dynamicsWorld->addConstraint(fixed);
	t3.setIdentity();
	t4.setIdentity();
	t3.setOrigin(btVector3(1, -6, 0));
	t4.setRotation(GLToBtQuat(glm::angleAxis(-10.0f, glm::vec3(5, 2, 0)))); // Not sure but this needs to be a minus angle
	t4.setOrigin(btVector3(1, 6, 0));
	btFixedConstraint * fixed2 = new btFixedConstraint(*neck->rigidBody, *body->rigidBody, t3, t4);
	dynamicsWorld->addConstraint(fixed2);

	shared_ptr<PhysicsController> joint = physicsFactory->CreateCylinder(1, 1, glm::vec3(x, 20, z), glm::quat());
	btHingeConstraint *shoulder = new btHingeConstraint(*joint->rigidBody, *body->rigidBody, btVector3(0, 1, 0), btVector3(6, 7, 0), btVector3(0, 1, 0), btVector3(7, 0, 0), false);
	shoulder->enableMotor(IsEnabled);
	shoulder->setMaxMotorImpulse(-MaxMotor);
	shoulder->setLimit(LowLimit, UpLimit);
	dynamicsWorld->addConstraint(shoulder);
	shared_ptr<PhysicsController> leg1 = physicsFactory->CreateCapsule(1.5f, 4, glm::vec3(x+5, 15, z), glm::quat());
	btHingeConstraint *shoulderleg = new btHingeConstraint(*leg1->rigidBody, *joint->rigidBody, btVector3(0, 7, 0), btVector3(1, 1, 0), btVector3(0, 3, 0), btVector3(1, 0, 0), false);
	dynamicsWorld->addConstraint(shoulderleg);
	joint->transform->diffuse = Brown;
	leg1->transform->diffuse = Brown;

	shared_ptr<PhysicsController> joint2 = physicsFactory->CreateCylinder(1, 1, glm::vec3(x, 20, z), glm::quat());
	btHingeConstraint *shoulder2 = new btHingeConstraint(*joint2->rigidBody, *body->rigidBody, btVector3(0, -1, 0), btVector3(-6, 7, 0), btVector3(0, 1, 0), btVector3(7, 0, 0), false);
	shoulder2->enableMotor(IsEnabled);
	shoulder2->setMaxMotorImpulse(-MaxMotor);
	shoulder2->setLimit(LowLimit, UpLimit);
	dynamicsWorld->addConstraint(shoulder2);
	shared_ptr<PhysicsController> leg2 = physicsFactory->CreateCapsule(1.5f, 4, glm::vec3(x+5, 15, z), glm::quat());
	btHingeConstraint *shoulderleg2 = new btHingeConstraint(*leg2->rigidBody, *joint2->rigidBody, btVector3(0, 7, 0), btVector3(1, -1, 0), btVector3(0, 3, 0), btVector3(1, 0, 0), false);
	dynamicsWorld->addConstraint(shoulderleg2);
	joint2->transform->diffuse = Brown;
	leg2->transform->diffuse = Brown;

	shared_ptr<PhysicsController> joint3 = physicsFactory->CreateCylinder(1, 1, glm::vec3(x, 20, z), glm::quat());
	btHingeConstraint *shoulder3 = new btHingeConstraint(*joint3->rigidBody, *body->rigidBody, btVector3(0, -1, 0), btVector3(-6, -7, 0), btVector3(0, 1, 0), btVector3(7, 0, 0), false);
	shoulder3->enableMotor(IsEnabled);
	shoulder3->setMaxMotorImpulse(-MaxMotor);
	shoulder3->setLimit(LowLimit, UpLimit);
	dynamicsWorld->addConstraint(shoulder3);
	shared_ptr<PhysicsController> leg3 = physicsFactory->CreateCapsule(1.5f, 4, glm::vec3(x+5, 15, z), glm::quat());
	btHingeConstraint *shoulderleg3 = new btHingeConstraint(*leg3->rigidBody, *joint3->rigidBody, btVector3(0, 7, 0), btVector3(1, -1, 0), btVector3(0, 3, 0), btVector3(1, 0, 0), false);
	dynamicsWorld->addConstraint(shoulderleg3);
	joint3->transform->diffuse = Brown;
	leg3->transform->diffuse = Brown;

	shared_ptr<PhysicsController> joint4 = physicsFactory->CreateCylinder(1, 1, glm::vec3(x, 20, z), glm::quat());
	btHingeConstraint *shoulder4 = new btHingeConstraint(*joint4->rigidBody, *body->rigidBody, btVector3(0, 1, 0), btVector3(6, -7, 0), btVector3(0, 1, 0), btVector3(7, 0, 0), false);
	shoulder4->enableMotor(IsEnabled);
	shoulder4->setMaxMotorImpulse(-MaxMotor);
	shoulder4->setLimit(LowLimit, UpLimit);
	dynamicsWorld->addConstraint(shoulder4);
	shared_ptr<PhysicsController> leg4 = physicsFactory->CreateCapsule(1.5f, 4, glm::vec3(x+5, 15, z), glm::quat());
	btHingeConstraint *shoulderleg4 = new btHingeConstraint(*leg4->rigidBody, *joint4->rigidBody, btVector3(0, 7, 0), btVector3(1, 1, 0), btVector3(0, 3, 0), btVector3(1, 0, 0), false);
	dynamicsWorld->addConstraint(shoulderleg4);
	joint4->transform->diffuse = Brown;
	leg4->transform->diffuse = Brown;


	//Create Tail
	shared_ptr<PhysicsController> tail = physicsFactory->CreateCapsule(1, 2, glm::vec3(x+5, 8, z-5), glm::quat());
	btHingeConstraint * hinget = new btHingeConstraint(*tail->rigidBody, *body->rigidBody, btVector3(0, 3, 0), btVector3(0, -12, 0), btVector3(1, 1, 1), btVector3(0, 1, 0), false);
	tail->transform->diffuse = Yellow;
	dynamicsWorld->addConstraint(hinget);
}
void BGE::Assignment::CreateWheeledHorse(float x, float z)
{
	glm::vec3 Brown(1, 0.5f, 0);
	glm::vec3 Yellow(2.5f, 2.5f, 0);
	glm::quat q = glm::angleAxis(90.0f, glm::vec3(1, 0, 0));
	glm::quat q2 = glm::angleAxis(30.0f, glm::vec3(1, 0, 0));
	//Create Wheeled Horse
	//Create Body
	shared_ptr<PhysicsController> body2 = physicsFactory->CreateCapsule(4, 5, glm::vec3(x, 15, z), q);
	body2->transform->diffuse = Brown;

	//Create Head
	shared_ptr<PhysicsController> head2 = physicsFactory->CreateCapsule(2.5f, 2.5f, glm::vec3(x, 20, z), q);
	shared_ptr<PhysicsController> neck2 = physicsFactory->CreateCylinder(2, 4, glm::vec3(x, 20, z), q);
	head2->transform->diffuse = Brown;
	neck2->transform->diffuse = Brown;
	btTransform t12, t22, t32, t42;
	t12.setIdentity();
	t22.setIdentity();
	t12.setOrigin(btVector3(2, 2, 0));
	t22.setRotation(GLToBtQuat(glm::angleAxis(-100.0f, glm::vec3(2, 0, 0))));
	t22.setOrigin(btVector3(2, 4, 0));
	btFixedConstraint * fixedw = new btFixedConstraint(*head2->rigidBody, *neck2->rigidBody, t12, t22);
	dynamicsWorld->addConstraint(fixedw);
	t32.setIdentity();
	t42.setIdentity();
	t32.setOrigin(btVector3(1, -6, 0));
	t42.setRotation(GLToBtQuat(glm::angleAxis(-10.0f, glm::vec3(5, 2, 0))));
	t42.setOrigin(btVector3(1, 6, 0));
	btFixedConstraint * fixed2w = new btFixedConstraint(*neck2->rigidBody, *body2->rigidBody, t32, t42);
	dynamicsWorld->addConstraint(fixed2w);



	//Create Legs
	shared_ptr<PhysicsController> leg12 = physicsFactory->CreateCapsule(1, 4, glm::vec3(x+5, 15, z), glm::quat());
	shared_ptr<PhysicsController> leg22 = physicsFactory->CreateCapsule(1, 4, glm::vec3(x-5, 15, z), glm::quat());
	shared_ptr<PhysicsController> leg32 = physicsFactory->CreateCapsule(1, 4, glm::vec3(x+5, 15, z), glm::quat());
	shared_ptr<PhysicsController> leg42 = physicsFactory->CreateCapsule(1, 4, glm::vec3(x+5, 15, z), glm::quat());
	leg12->transform->diffuse = Brown;
	leg22->transform->diffuse = Brown;
	leg32->transform->diffuse = Brown;
	leg42->transform->diffuse = Brown;

	//Attach to Body
	btHingeConstraint * hingew = new btHingeConstraint(*leg12->rigidBody, *body2->rigidBody, btVector3(0, 6, 0), btVector3(5, 9, 0), btVector3(0, 1, 0), btVector3(-1, 0, -4), false);
	dynamicsWorld->addConstraint(hingew);
	btHingeConstraint * hinge2w = new btHingeConstraint(*leg22->rigidBody, *body2->rigidBody, btVector3(0, 6, 0), btVector3(-5, 9, 0), btVector3(0, 1, 0), btVector3(1, 0, -4), false);
	dynamicsWorld->addConstraint(hinge2w);
	btHingeConstraint * hinge3w = new btHingeConstraint(*leg32->rigidBody, *body2->rigidBody, btVector3(0, 6, 0), btVector3(5, -9, 0), btVector3(0, 1, 0), btVector3(-1, 0, -4), false);
	dynamicsWorld->addConstraint(hinge3w);
	btHingeConstraint * hinge4w = new btHingeConstraint(*leg42->rigidBody, *body2->rigidBody, btVector3(0, 6, 0), btVector3(-5, -9, 0), btVector3(0, 1, 0), btVector3(1, 0, -4), false);
	dynamicsWorld->addConstraint(hinge4w);

	//Create Wheels
	shared_ptr<PhysicsController> wheel1 = physicsFactory->CreateCylinder(3, 1, glm::vec3(x+5, 5, z), glm::quat());
	wheel1->transform->diffuse = Yellow;
	shared_ptr<PhysicsController> wheel2 = physicsFactory->CreateCylinder(3, 1, glm::vec3(x-5, 5, z), glm::quat());
	wheel2->transform->diffuse = Yellow;
	shared_ptr<PhysicsController> wheel3 = physicsFactory->CreateCylinder(3, 1, glm::vec3(x+5, 5, z), glm::quat());
	wheel3->transform->diffuse = Yellow;
	shared_ptr<PhysicsController> wheel4 = physicsFactory->CreateCylinder(3, 1, glm::vec3(x-5, 5, z), glm::quat());
	wheel4->transform->diffuse = Yellow;

	//Attach to legs

	btHingeConstraint * hinge5w = new btHingeConstraint(*wheel1->rigidBody, *leg12->rigidBody, btVector3(0, 1, 0), btVector3(0, -8, 0), btVector3(0, 1, 0), btVector3(1, 0, 7), false);
	hinge5w->enableMotor(IsEnabled);
	hinge5w->setMaxMotorImpulse(MaxMotor2);
	//hinge5w->setLimit(low, up);
	dynamicsWorld->addConstraint(hinge5w);

	btHingeConstraint * hinge6w = new btHingeConstraint(*wheel2->rigidBody, *leg22->rigidBody, btVector3(0, 1, 0), btVector3(0, -8, 0), btVector3(0, 1, 0), btVector3(1, 0, -7), false);
	hinge6w->enableMotor(IsEnabled);
	hinge6w->setMaxMotorImpulse(-MaxMotor2);
	//hinge6w->setLimit(low, up);
	dynamicsWorld->addConstraint(hinge6w);
	btHingeConstraint * hinge7w = new btHingeConstraint(*wheel3->rigidBody, *leg32->rigidBody, btVector3(0, 1, 0), btVector3(0, -8, 0), btVector3(0, 1, 0), btVector3(1, 0, 7), false);
	hinge7w->enableMotor(IsEnabled);
	hinge7w->setMaxMotorImpulse(MaxMotor2);
	//hinge7w->setLimit(low, up);
	dynamicsWorld->addConstraint(hinge7w);
	btHingeConstraint * hinge8w = new btHingeConstraint(*wheel4->rigidBody, *leg42->rigidBody, btVector3(0, 1, 0), btVector3(0, -8, 0), btVector3(0, 1, 0), btVector3(1, 0, -7), false);
	hinge8w->enableMotor(IsEnabled);
	hinge8w->setMaxMotorImpulse(-MaxMotor2);
	//hinge8w->setLimit(low, up);
	dynamicsWorld->addConstraint(hinge8w);



	//Create Tail
	shared_ptr<PhysicsController> tail2 = physicsFactory->CreateCapsule(1, 2, glm::vec3(x+5, 5, z-5), glm::quat());
	btHingeConstraint * hingetw = new btHingeConstraint(*tail2->rigidBody, *body2->rigidBody, btVector3(0, 3, 0), btVector3(0, -12, 0), btVector3(1, 1, 1), btVector3(0, 1, 0), false);
	tail2->transform->diffuse = Yellow;
	dynamicsWorld->addConstraint(hingetw);

	//Create Brace
	shared_ptr<PhysicsController> brace1 = physicsFactory->CreateCapsule(1, 4, glm::vec3(x+5, 5, z), q);
	shared_ptr<PhysicsController> brace2 = physicsFactory->CreateCapsule(1, 4, glm::vec3(x-5, 5, z), q);
	brace1->transform->diffuse = Brown;
	brace2->transform->diffuse = Brown;

	//Attach to legs
	btHingeConstraint * hinge92 = new btHingeConstraint(*brace1->rigidBody, *leg12->rigidBody, btVector3(0, 8, 0), btVector3(-1, 0, 0), btVector3(0, 1, 0), btVector3(1, 0, 0), false);
	dynamicsWorld->addConstraint(hinge92);
	btHingeConstraint * hinge102 = new btHingeConstraint(*brace2->rigidBody, *leg22->rigidBody, btVector3(0, 8, 0), btVector3(-1, 0, 0), btVector3(0, 1, 0), btVector3(1, 0, 0), false);
	dynamicsWorld->addConstraint(hinge102);
	btHingeConstraint * hinge112 = new btHingeConstraint(*brace1->rigidBody, *leg32->rigidBody, btVector3(0, -8, 0), btVector3(1, 0, 0), btVector3(0, 1, 0), btVector3(1, 0, 0), false);
	dynamicsWorld->addConstraint(hinge112);
	btHingeConstraint * hinge122 = new btHingeConstraint(*brace2->rigidBody, *leg42->rigidBody, btVector3(0, -8, 0), btVector3(1, 0, 0), btVector3(0, 1, 0), btVector3(1, 0, 0), false);
	dynamicsWorld->addConstraint(hinge122);

}