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
	openingDuration = 8;
	legDirection = 1; //negative closes leg, positive opens
	speed = 2;

	CreateSpider(glm::vec3(0, 10, 0));

	if (!Game::Initialise()) {
		return false;
	}

	//camera->transform->position = glm::vec3(0, 10, 20);

	return true;
}

void Assignment::Update(float timeDelta)
{
	/*
	for (it = sliders.begin(); it != sliders.end(); ++it)
	{
		if ((int)openingDuration % 8 == 0)
		{
			legDirection *= -1;
			openingDuration = 8;
		}

		openingDuration -= timeDelta;

		(*it)->setTargetLinMotorVelocity(legDirection * speed);
	}
	*/
	Game::Update(timeDelta);
}

void Assignment::CreateSpider(glm::vec3 position)
{
	/*************************************************************
	TO DO:
	Legs need to point in a specific direction. Create method passing in vector and ammend below to correct hinge vectors
	method to control speed/direction of leg movements to be used in update
	method to control leg movement (alternating pattern, cone hinges connecting body and legs

	*/

	
	/*
	Spider consists of a spherical thorax and capsule abdomen. 8 legs attached to the thorax, with the following
	rigid bodies and joint constraints

	thorax (sphere) -> coxa (sphere) via cone twist constraint
	coxa (sphere) -> upper_leg (capsule) via cone twist constraint
	upper_leg (capsule) -> upper_mid_leg (capsule) via hinge joint constraint
	upper_mid_leg (capsule) -> lower_mid_leg (capsule) via hinge joint constraint
	lower_mid_leg (capsule) -> lower_leg (capsule) via hinge joint constraint
	lower_leg (capsule) -> foot (capsule) via hinge joint constraint

	hinge joint from thorax to Abdomen to allow slight movement of abdomen from
	left to right (when viewed top down)

	*/

	float thorax_radius = 8.0f;
	shared_ptr<PhysicsController> body_part_thorax = physicsFactory->CreateSphere (thorax_radius, position, glm::quat());
	body_part_thorax->rigidBody->setMassProps(0, btVector3());
	//divided by number of legs
	float num_legs = 8;
	float theta = (glm::pi<float>() * 2) / num_legs;

	//angle that the coxa is to attach to the sphere in a loop
	for (int i = 0; i < num_legs; i++)
	{
		if (i < num_legs)//leave gap at front and back (head and abdomen)
		{
			glm::vec3 coxa_position;
			coxa_position = glm::vec3(position.x + (glm::sin(theta * i) * (thorax_radius + 6.5 )), position.y, position.z + (-glm::cos(theta * i) * (thorax_radius + 6.5 )));

			glm::vec3 leg_direction = position - coxa_position;
			leg_direction = glm::normalize(leg_direction);
			shared_ptr<PhysicsController> leg = createLeg(coxa_position, leg_direction, glm::angleAxis(90.0f, glm::vec3(0,1,0)));
			
			/*
			btFixedConstraint* coxa_leg_hinge;
			btTransform localA, localB;
			localA.setIdentity(); localB.setIdentity();
			localA.getBasis().setEulerZYX(0,0,0); localA.getBasis().setEulerZYX(0,0,0);
			localA.setOrigin(GLToBtVector(coxa_position));
			localB.getBasis().setEulerZYX(0,0,0); localA.getBasis().setEulerZYX(0,0,0);
			localB.setOrigin(btVector3(btScalar(0.), btScalar(0), btScalar(0.)));
			coxa_leg_hinge = new btFixedConstraint(*body_part_thorax->rigidBody, *leg->rigidBody, localA, localB);
			dynamicsWorld->addConstraint(coxa_leg_hinge);
			//joint between leg and coxa
			*/
			btHingeConstraint* coxa_leg_hinge;

			coxa_leg_hinge = new btHingeConstraint(*body_part_thorax->rigidBody, *leg->rigidBody, btVector3(coxa_position.x, 0, coxa_position.z), btVector3(0, 0, 0),
			GLToBtVector(leg_direction), btVector3(0, 1, 0));
			coxa_leg_hinge->setLimit(btScalar(0), btScalar(0));
			dynamicsWorld->addConstraint(coxa_leg_hinge);
			
		}
	}
	
}

shared_ptr<PhysicsController> Assignment::createLeg(glm::vec3 position, glm::vec3 direction, glm::quat orientation)
{
	glm::quat q = orientation * glm::angleAxis(90.0f, direction);

	float leg_offset = 14.0f / 4.0f;
	float muscle_offset = 2;

	//upper leg & muscle
	shared_ptr<PhysicsController> upper_leg = physicsFactory->CreateCapsule(1, 3, glm::vec3(position.x, position.y, position.z), q);
	shared_ptr<PhysicsController> upper_leg_muscle = physicsFactory->CreateCylinder(0.5, 0.2, position - (direction * leg_offset ) + glm::vec3(0,-2,0), q);
	btHingeConstraint* upper_leg_muscle_hinge;
	upper_leg_muscle_hinge = new btHingeConstraint(*upper_leg_muscle->rigidBody, *upper_leg->rigidBody, btVector3(-5, 0, 0), btVector3(-2, 2, 0), btVector3(1, 0, 0), btVector3(0, 0, 1));
	upper_leg_muscle_hinge->setLimit(btScalar(0), btScalar(glm::half_pi<float>()));
	//dynamicsWorld->addConstraint(upper_leg_muscle_hinge);

	//middle leg and muscles
	//shared_ptr<PhysicsController> mid_leg = physicsFactory->CreateCapsule(1, 3, glm::vec3(position.x, position.y, position.z) + (direction * ( leg_offset * 4) ), q);
	//shared_ptr<PhysicsController> mid_upper_leg_muscle = physicsFactory->CreateCylinder(0.5, 0.2, position + (direction * (leg_offset * 3)) + glm::vec3(0, -2, 0), q);
	//shared_ptr<PhysicsController> mid_lower_leg_muscle = physicsFactory->CreateCylinder(0.5, 0.2, position + (direction * (leg_offset * 5) ) + glm::vec3(0, -2, 0), q);
	//btHingeConstraint* mid_upper_leg_muscle_hinge;
	//mid_upper_leg_muscle_hinge = new btHingeConstraint(*mid_leg->rigidBody, *mid_upper_leg_muscle->rigidBody, btVector3(-2, -2, 0), btVector3(0, 0, 0), btVector3(0, 0, 1), btVector3(1, 0, 0));
	//mid_upper_leg_muscle_hinge->setLimit(btScalar(0), btScalar(glm::half_pi<float>()));
	//dynamicsWorld->addConstraint(mid_upper_leg_muscle_hinge);
	//btHingeConstraint* mid_lower_leg_muscle_hinge;
	//mid_lower_leg_muscle_hinge = new btHingeConstraint(*mid_lower_leg_muscle->rigidBody, *mid_leg->rigidBody, btVector3(0, 0, 0), btVector3(-2, 2, 0), btVector3(1, 0, 0), btVector3(0, 0, 1));
	//mid_lower_leg_muscle_hinge->setLimit(btScalar(0), btScalar(glm::half_pi<float>()));
	//dynamicsWorld->addConstraint(mid_lower_leg_muscle_hinge);
	
	//hinge joint between upper leg part and mid leg
	//btHingeConstraint* upper_mid_leg_hinge;
//	upper_mid_leg_hinge = new btHingeConstraint(*mid_leg->rigidBody, *upper_leg->rigidBody, btVector3(0, -6.5, 0), btVector3(0, 6.5, 0), btVector3(0, 0, 1), btVector3(0, 0, 1));
	//upper_mid_leg_hinge->setLimit(btScalar(0), btScalar(glm::half_pi<float>()));
	//dynamicsWorld->addConstraint(upper_mid_leg_hinge);
	
	//lower leg and muscle
	//shared_ptr<PhysicsController> lower_leg = physicsFactory->CreateCapsule(1, 3, glm::vec3(position.x, position.y, position.z) + (direction * (leg_offset * 8)), q);
	//shared_ptr<PhysicsController> lower_leg_muscle = physicsFactory->CreateCylinder(0.5, 0.2, position + (direction * (leg_offset * 7)) + glm::vec3(0, -2, 0), q);
	//btHingeConstraint* lower_leg_muscle_hinge;
	//lower_leg_muscle_hinge = new btHingeConstraint(*lower_leg->rigidBody, *lower_leg_muscle->rigidBody, btVector3(-2, -2, 0), btVector3(0, 0, 0), btVector3(0, 0, 1), btVector3(1, 0, 0));
	//lower_leg_muscle_hinge->setLimit(btScalar(0), btScalar(glm::half_pi<float>()));
	//dynamicsWorld->addConstraint(lower_leg_muscle_hinge);

	//hinge joint between mid leg part and lower
	//btHingeConstraint* mid_lower_leg_hinge;
	//mid_lower_leg_hinge = new btHingeConstraint(*lower_leg->rigidBody, *mid_leg->rigidBody, btVector3(0, -6.5, 0), btVector3(0, 6.5, 0), btVector3(0, 0, 1), btVector3(0, 0, 1));
	//mid_lower_leg_hinge->setLimit(btScalar(0), btScalar(glm::pi<float>()));
//	dynamicsWorld->addConstraint(mid_lower_leg_hinge);
	

	//slider joint between muscles fixed to leg
	
	//btTransform muscleTransform1, muscleTransform2;
	//muscleTransform1.setIdentity();
	//muscleTransform2.setIdentity();

	//muscleTransform1.setRotation(GLToBtQuat(glm::angleAxis(90.0f, glm::vec3(0, 0, 1))));

	//btSliderConstraint* upper_mid_leg_slider;
	//btSliderConstraint* mid_lower_leg_slider;
	//upper_mid_leg_slider = new btSliderConstraint(*upper_leg_muscle->rigidBody, *mid_upper_leg_muscle->rigidBody, muscleTransform1, muscleTransform1, true);
	//mid_lower_leg_slider = new btSliderConstraint(*mid_lower_leg_muscle->rigidBody, *lower_leg_muscle->rigidBody, muscleTransform1, muscleTransform1, true);
	//dynamicsWorld->addConstraint(upper_mid_leg_slider);
	//dynamicsWorld->addConstraint(mid_lower_leg_slider);
	//upper_mid_leg_slider->setPoweredLinMotor(true);
	//upper_mid_leg_slider->setLowerLinLimit(2);
	//upper_mid_leg_slider->setUpperLinLimit(15);
	//upper_mid_leg_slider->setMaxLinMotorForce(20);
	//mid_lower_leg_slider->setPoweredLinMotor(true);
	//mid_lower_leg_slider->setMaxLinMotorForce(20);
	//mid_lower_leg_slider->setLowerLinLimit(2);
	//mid_lower_leg_slider->setUpperLinLimit(15);
	//sliders.push_back(mid_lower_leg_slider);
	//sliders.push_back(upper_mid_leg_slider);

	return upper_leg;
	
}

void Assignment::Cleanup()
{
	Game::Cleanup();
}
