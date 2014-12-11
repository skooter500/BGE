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
#include "LazerBeam.h"
#include "PhysicsFactory.h"
#include "Box.h"
#include "Sphere.h"
#include "Cylinder.h"

using namespace BGE;

Assignment::Assignment(void)
{
}


Assignment::~Assignment(void)
{
}

shared_ptr<PhysicsController> tripod;
shared_ptr<LazerBeam> lazer = make_shared<LazerBeam>();

bool Assignment::Initialise(void)
{
	physicsFactory->CreateGroundPhysics();
	physicsFactory->CreateCameraPhysics();

	setGravity(glm::vec3(0, -7, 0));

	tripod = CreateTripod(glm::vec3(0, 0, 0));
	
	if (!Game::Initialise())
		return false;

	return true;
}

void Assignment::Update(void)
{
	if (keyState[SDL_SCANCODE_L])
	{
		lazer->transform->position = tripod->transform->position;
		lazer->transform->look = tripod->transform->look;
		Attach(lazer);
	}

	Game::Update();
}

shared_ptr<PhysicsController> Assignment::CreateTripod(glm::vec3 position)
{
	// Create constraints
	btHingeConstraint* left_knee_left_lower_leg;
	btHingeConstraint* right_knee_right_lower_leg;
	btHingeConstraint* back_knee_back_lower_leg;

	btConeTwistConstraint* shoulder_left_upper_leg;
	btConeTwistConstraint* shoulder_right_upper_leg;
	btHingeConstraint* shoulder_back_upper_leg;

	btFixedConstraint* body_left_shoulder;
	btFixedConstraint* body_right_shoulder;
	btFixedConstraint* body_back_shoulder;

	btFixedConstraint* back_knee_back_upper_leg;
	btFixedConstraint* right_knee_right_upper_leg;
	btFixedConstraint* left_knee_left_upper_leg;

	btFixedConstraint* left_lower_leg_left_foot;
	btFixedConstraint* right_lower_leg_right_foot;
	btFixedConstraint* back_lower_leg_back_foot;

	// Create rigid bodies
	// Body 
	shared_ptr<PhysicsController> bodypart_body = physicsFactory->CreateSphere(8.5f, glm::vec3(position.x, position.y + 52, position.z + 2), glm::quat());

	// Left Leg
	shared_ptr<PhysicsController> bodypart_left_upper_leg = physicsFactory->CreateCylinder(1.4, 11,
		glm::vec3(position.x - 15, position.y + 62, position.z + 1),
		glm::angleAxis(67.5f, glm::vec3(0, 0, 1)));

	shared_ptr<PhysicsController> bodypart_left_lower_leg = physicsFactory->CreateCylinder(1.4, 50,
		glm::vec3(position.x - 20, position.y + 33, position.z + 1),
		glm::quat());

	shared_ptr<PhysicsController> bodypart_left_foot = physicsFactory->CreateBox(5, 5, 5,
		glm::vec3(position.x - 20, position.y + 5, position.z + 1),
		glm::quat());

	shared_ptr <PhysicsController> bodypart_left_shoulder = physicsFactory->CreateSphere(1.7f, glm::vec3(position.x - 8, position.y + 59, position.z + 1),
		glm::quat());

	shared_ptr <PhysicsController> bodypart_left_knee = physicsFactory->CreateSphere(1.5f, glm::vec3(position.x - 20, position.y + 60, position.z + 1),
		glm::quat());

	// Right Leg
	shared_ptr<PhysicsController> bodypart_right_upper_leg = physicsFactory->CreateCylinder(1.4, 11,
		glm::vec3(position.x + 15, position.y + 62, position.z + 1),
		glm::angleAxis(67.5f, glm::vec3(0, 0, -1)));

	shared_ptr<PhysicsController> bodypart_right_lower_leg = physicsFactory->CreateCylinder(1.4, 50,
		glm::vec3(position.x + 20, position.y + 33, position.z + 1),
		glm::quat());

	shared_ptr<PhysicsController> bodypart_right_foot = physicsFactory->CreateBox(5, 5, 5,
		glm::vec3(position.x + 20, position.y + 5, position.z + 1),
		glm::quat());

	shared_ptr<PhysicsController> bodypart_right_shoulder = physicsFactory->CreateSphere(1.7f, glm::vec3(position.x + 8, position.y + 59, position.z + 1),
		glm::quat());

	shared_ptr<PhysicsController> bodypart_right_knee = physicsFactory->CreateSphere(1.5f, glm::vec3(position.x + 20, position.y + 60, position.z + 1),
		glm::quat());

	// Back leg
	shared_ptr<PhysicsController> bodypart_back_lower_leg = physicsFactory->CreateCylinder(1.4, 50,
		glm::vec3(position.x, position.y + 33, position.z - 19),
		glm::quat());

	shared_ptr<PhysicsController> bodypart_back_upper_leg = physicsFactory->CreateCylinder(1.4, 11,
		glm::vec3(position.x, position.y + 62, position.z - 13),
		glm::angleAxis(67.5f, glm::vec3(-1, 0, 0)));

	shared_ptr<PhysicsController> bodypart_back_foot = physicsFactory->CreateBox(5, 5, 5,
		glm::vec3(position.x, position.y + 5, position.z - 19),
		glm::quat());

	shared_ptr<PhysicsController> bodypart_back_shoulder = physicsFactory->CreateSphere(1.7f, glm::vec3(position.x, position.y + 59, position.z - 6),
		glm::quat());

	shared_ptr<PhysicsController> bodypart_back_knee = physicsFactory->CreateSphere(1.5f, glm::vec3(position.x, position.y + 60, position.z - 19),
		glm::quat());

	// Create joints
	btTransform localA, localB;

	// Body to back shoulder
	localA.setIdentity();
	localB.setIdentity();
	localA.setOrigin(btVector3(0, 4, -6.5));
	localB.setOrigin(btVector3(0, 0, 3));
	body_back_shoulder = new btFixedConstraint(*bodypart_body->rigidBody, *bodypart_back_shoulder->rigidBody,
		localA, localB);
	dynamicsWorld->addConstraint(body_back_shoulder);

	// Upper back leg to back shoulder
	shoulder_back_upper_leg = new btHingeConstraint(*bodypart_back_shoulder->rigidBody, *bodypart_back_upper_leg->rigidBody,
		btVector3(btScalar(0), btScalar(1), btScalar(0)),
		btVector3(btScalar(0), btScalar(-8), btScalar(0)),
		btVector3(1, 0, 0), btVector3(1, 0, 0));
	dynamicsWorld->addConstraint(shoulder_back_upper_leg);

	// Back knee to upper back leg
	localA.setIdentity();
	localB.setIdentity();
	localA.setOrigin(btVector3(0, 3, -5.5));
	localB.setOrigin(btVector3(0, 0, 0.1));
	back_knee_back_upper_leg = new btFixedConstraint(*bodypart_back_upper_leg->rigidBody, *bodypart_back_knee->rigidBody,
		localA, localB);
	dynamicsWorld->addConstraint(back_knee_back_upper_leg);

	// Back knee to lower back leg
	back_knee_back_lower_leg = new btHingeConstraint(*bodypart_back_knee->rigidBody, *bodypart_back_lower_leg->rigidBody,
		btVector3(btScalar(0), btScalar(1.), btScalar(0)),
		btVector3(btScalar(0), btScalar(28.), btScalar(0)),
		btVector3(1, 0, 0), btVector3(1, 0, 0));
	dynamicsWorld->addConstraint(back_knee_back_lower_leg);

	// Lower back leg to back foot
	localA.setIdentity();
	localB.setIdentity();
	localA.setOrigin(btVector3(0, 1, 0));
	localB.setOrigin(btVector3(0, 30, 0));
	localB.setRotation(GLToBtQuat(glm::angleAxis(-30.0f, glm::vec3(0, 1, 0))));
	back_lower_leg_back_foot = new btFixedConstraint(*bodypart_back_lower_leg->rigidBody, *bodypart_back_foot->rigidBody,
		localA, localB);
	dynamicsWorld->addConstraint(back_lower_leg_back_foot);


	// Left Shoulder to Body
	localA.setIdentity();
	localB.setIdentity();
	localA.setOrigin(btVector3(-5, 6.5, 0));
	localB.setOrigin(btVector3(3, 0, 0));

	body_left_shoulder = new btFixedConstraint(*bodypart_body->rigidBody, *bodypart_left_shoulder->rigidBody,
		localA, localB);
	dynamicsWorld->addConstraint(body_left_shoulder);

	// Left Shoulder to Left Upper Leg
	localA.setIdentity();
	localB.setIdentity();

	localA.setOrigin(btVector3(btScalar(0.), btScalar(1.), btScalar(0.)));
	localB.setOrigin(btVector3(btScalar(0.), btScalar(-7.), btScalar(0.)));

	//localA.getBasis().setEulerZYX(0, 0, 0);
	//localB.getBasis().setEulerZYX(0, 0, glm::half_pi<float>());

	shoulder_left_upper_leg = new btConeTwistConstraint(*bodypart_left_shoulder->rigidBody, *bodypart_left_upper_leg->rigidBody,
		localA, localB);

	dynamicsWorld->addConstraint(shoulder_left_upper_leg);

	// Upper Left Leg to Left Knee
	localA.setIdentity();
	localB.setIdentity();
	localA.setOrigin(btVector3(-5.5, 3, 0));
	localB.setOrigin(btVector3(0.1, 0, 0));
	left_knee_left_upper_leg = new btFixedConstraint(*bodypart_left_upper_leg->rigidBody, *bodypart_left_knee->rigidBody,
		localA, localB);
	dynamicsWorld->addConstraint(left_knee_left_upper_leg);

	// Left Knee to Left Lower Leg
	left_knee_left_lower_leg = new btHingeConstraint(*bodypart_left_knee->rigidBody, *bodypart_left_lower_leg->rigidBody,
		btVector3(btScalar(0), btScalar(1.), btScalar(0)),
		btVector3(btScalar(0), btScalar(28.), btScalar(0)),
		btVector3(0, 0, 1), btVector3(0, 0, 1));
	dynamicsWorld->addConstraint(left_knee_left_lower_leg);

	// Lower left leg to left foot
	localA.setIdentity();
	localB.setIdentity();
	localA.setOrigin(btVector3(0, 1, 0));
	localB.setOrigin(btVector3(0, 30, 0));

	localB.setRotation(GLToBtQuat(glm::angleAxis(-30.0f, glm::vec3(0, 1, 0))));
	left_lower_leg_left_foot = new btFixedConstraint(*bodypart_left_lower_leg->rigidBody, *bodypart_left_foot->rigidBody,
		localA, localB);
	dynamicsWorld->addConstraint(left_lower_leg_left_foot);


	// Right shoulder to body
	localA.setIdentity();
	localB.setIdentity();
	localA.setOrigin(btVector3(5, 6.5, 0));
	localB.setOrigin(btVector3(-3, 0, 0));

	body_right_shoulder = new btFixedConstraint(*bodypart_body->rigidBody, *bodypart_right_shoulder->rigidBody,
		localA, localB);
	dynamicsWorld->addConstraint(body_right_shoulder);


	// Right Shoulder to Upper Right Leg
	localA.setIdentity();
	localB.setIdentity();

	localA.setOrigin(btVector3(btScalar(0.), btScalar(1.), btScalar(0.)));
	localB.setOrigin(btVector3(btScalar(0.), btScalar(-7.), btScalar(0.)));

	//localA.getBasis().setEulerZYX(0, 0, 0);
	//localB.getBasis().setEulerZYX(0, 0, glm::half_pi<float>());

	shoulder_right_upper_leg = new btConeTwistConstraint(*bodypart_right_shoulder->rigidBody, *bodypart_right_upper_leg->rigidBody,
		localA, localB);
	dynamicsWorld->addConstraint(shoulder_right_upper_leg);

	// Right Upper Leg to Right Knee
	localA.setIdentity();
	localB.setIdentity();
	localA.setOrigin(btVector3(5.5, 3, 0));
	localB.setOrigin(btVector3(-0.1, 0, 0));

	right_knee_right_upper_leg = new btFixedConstraint(*bodypart_right_upper_leg->rigidBody, *bodypart_right_knee->rigidBody,
		localA, localB);
	dynamicsWorld->addConstraint(right_knee_right_upper_leg);

	// Right Knee to Right Lower Leg
	right_knee_right_lower_leg = new btHingeConstraint(*bodypart_right_knee->rigidBody, *bodypart_right_lower_leg->rigidBody,
		btVector3(btScalar(0), btScalar(1.), btScalar(0)),
		btVector3(btScalar(0), btScalar(28.), btScalar(0)),
		btVector3(0, 0, 1), btVector3(0, 0, 1));
	dynamicsWorld->addConstraint(right_knee_right_lower_leg);

	// Right lower leg to right foot
	localA.setIdentity();
	localB.setIdentity();
	localA.setOrigin(btVector3(0, 1, 0));
	localB.setOrigin(btVector3(0, 30, 0));
	localB.setRotation(GLToBtQuat(glm::angleAxis(-30.0f, glm::vec3(0, 1, 0))));
	right_lower_leg_right_foot = new btFixedConstraint(*bodypart_right_lower_leg->rigidBody, *bodypart_right_foot->rigidBody,
		localA, localB);
	dynamicsWorld->addConstraint(right_lower_leg_right_foot);

	// Set joint limits
	right_knee_right_lower_leg->setLimit(btScalar(1), btScalar(1));
	left_knee_left_lower_leg->setLimit(btScalar(1), btScalar(1));

	// Set Mass
	bodypart_back_foot->rigidBody->setMassProps(btScalar(2), btVector3(0, 0, 0));
	bodypart_left_foot->rigidBody->setMassProps(btScalar(2), btVector3(0, 0, 0));
	bodypart_right_foot->rigidBody->setMassProps(btScalar(2), btVector3(0, 0, 0));

	bodypart_body->rigidBody->setMassProps(btScalar(5), btVector3(0, 0, 0));

	bodypart_right_upper_leg->rigidBody->setMassProps(btScalar(1), btVector3(0, 0, 0));
	bodypart_left_upper_leg->rigidBody->setMassProps(btScalar(1), btVector3(0, 0, 0));
	bodypart_back_upper_leg->rigidBody->setMassProps(btScalar(1), btVector3(0, 0, 0));

	// Set Colours
	bodypart_body->transform->diffuse = glm::vec3(0, 1, 0);

	bodypart_back_foot->transform->diffuse = glm::vec3(0, 1, 0);
	bodypart_back_knee->transform->diffuse = glm::vec3(0, 1, 0);
	bodypart_back_lower_leg->transform->diffuse = glm::vec3(0, 1, 0);
	bodypart_back_shoulder->transform->diffuse = glm::vec3(0, 1, 0);
	bodypart_back_upper_leg->transform->diffuse = glm::vec3(0, 1, 0);

	bodypart_left_foot->transform->diffuse = glm::vec3(0, 1, 0);
	bodypart_left_knee->transform->diffuse = glm::vec3(0, 1, 0);
	bodypart_left_lower_leg->transform->diffuse = glm::vec3(0, 1, 0);
	bodypart_left_shoulder->transform->diffuse = glm::vec3(0, 1, 0);
	bodypart_left_upper_leg->transform->diffuse = glm::vec3(0, 1, 0);

	bodypart_right_foot->transform->diffuse = glm::vec3(0, 1, 0);
	bodypart_right_knee->transform->diffuse = glm::vec3(0, 1, 0);
	bodypart_right_lower_leg->transform->diffuse = glm::vec3(0, 1, 0);
	bodypart_right_shoulder->transform->diffuse = glm::vec3(0, 1, 0);
	bodypart_right_upper_leg->transform->diffuse = glm::vec3(0, 1, 0);

	return bodypart_body;
}