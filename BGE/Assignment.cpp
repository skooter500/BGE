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
	dynamicsWorld->setGravity( btVector3(0, -9.8, 0));
	counter = 0;
	openingDuration = 50;
	legDirection = -1; //negative closes leg, positive opens
	speed = 2;

	CreateSpider(glm::vec3(0, 10, 0));

	if (!Game::Initialise()) {
		return false;
	}

	camera->transform->position = glm::vec3(0, 50, 100);

	return true;
}

void Assignment::Update(float timeDelta)
{
	for (it = sliders.begin(); it != sliders.end(); ++it)
	{
		counter += timeDelta;

		if (counter > openingDuration)
		{
			if (legDirection < 1)
				legDirection = 1;
			else
				legDirection = -1;
			counter = 0;
		}

		(*it)->setTargetLinMotorVelocity(legDirection * speed);
	}
	
	Game::Update(timeDelta);
}

void Assignment::CreateSpider(glm::vec3 position)
{
	/*************************************************************
	control leg movement (alternating pattern, cone hinges connecting body and legs

	*/

	
	/*
	Spider consists of a spherical thorax and capsule abdomen. 8 legs attached to the thorax, with the following
	rigid bodies and joint constraints

	coxa (sphere) -> upper_leg (capsule) via cone twist constraint
	upper_leg (capsule) -> upper_mid_leg (capsule) via hinge joint constraint
	upper_mid_leg (capsule) -> lower_mid_leg (capsule) via hinge joint constraint
	lower_mid_leg (capsule) -> lower_leg (capsule) via hinge joint constraint

	hinge joint from thorax to Abdomen to allow slight movement of abdomen from
	left to right (when viewed top down)

	*/

	float thorax_radius = 8.0f;
	shared_ptr<PhysicsController> body_part_thorax = physicsFactory->CreateSphere (thorax_radius, position, glm::quat());
	//divided by number of legs
	float num_legs = 10;
	float theta = (glm::pi<float>() * 2) / num_legs;

	//angle that the coxa is to attach to the sphere in a loop
	for (int i = 0; i < num_legs; i++)
	{
		if (i % (int)(num_legs/2) != 0)//leave gap at front and back (head and abdomen)
		{
			glm::vec3 coxa_position;
			coxa_position = glm::vec3(position.x + (glm::sin(theta * i) * (thorax_radius + 6.5 )), position.y, position.z + (-glm::cos(theta * i) * (thorax_radius + 6.5 )));

			glm::vec3 leg_direction = position - coxa_position;
			leg_direction = glm::normalize(leg_direction);
			shared_ptr<PhysicsController> leg = createLeg(coxa_position, leg_direction, glm::angleAxis(90.0f, glm::vec3(0,1,0)));
			
			btHingeConstraint* coxa_leg_hinge;
			coxa_leg_hinge = new btHingeConstraint(*body_part_thorax->rigidBody, *leg->rigidBody, btVector3(coxa_position.x, 0, coxa_position.z), btVector3(0, 0, 0),
													GLToBtVector(leg_direction), btVector3(0,1,0), true);
			coxa_leg_hinge->setLimit(btScalar(0), btScalar(0));
			dynamicsWorld->addConstraint(coxa_leg_hinge);
			
		}
	}
	
}

shared_ptr<PhysicsController> Assignment::createLeg(glm::vec3 position, glm::vec3 direction, glm::quat orientation)
{
	glm::quat q = orientation * glm::angleAxis(90.0f, direction);

	float leg_offset = 13.0f / 6.0f;
	float muscle_offset = 2;

	//upper leg 
	shared_ptr<PhysicsController> upper_leg = physicsFactory->CreateCapsule(1, 3, glm::vec3(position.x, position.y, position.z), q);
	upper_leg->rigidBody->setRestitution(0);
	//muscle to connect upper leg and middle leg
	shared_ptr<PhysicsController> upper_leg_muscle = physicsFactory->CreateCylinder(0.5, 0.2, position - (direction * leg_offset ) + glm::vec3(0,-2,0), q);
	btHingeConstraint* upper_leg_muscle_hinge;
	upper_leg_muscle_hinge = new btHingeConstraint(*upper_leg_muscle->rigidBody, *upper_leg->rigidBody, 
													btVector3(0, 0, 0), btVector3(direction.x * muscle_offset, -leg_offset, direction.z * muscle_offset),
													btVector3(direction.x, 0, direction.z), btVector3(-direction.z, 0, direction.x));	//perpendicular vector (swap points and negate one)

	upper_leg_muscle_hinge->setLimit(btScalar(0), btScalar(glm::half_pi<float>()), btScalar(0.1), btScalar(0.2), btScalar(0.1f));
	dynamicsWorld->addConstraint(upper_leg_muscle_hinge);

	//middle leg 
	shared_ptr<PhysicsController> mid_leg = physicsFactory->CreateCapsule(1, 3, glm::vec3(position.x, position.y, position.z) - (direction * ( leg_offset * 4) ), q);

	mid_leg->rigidBody->setDamping(btScalar(1), btScalar(1));
	//hinge joint between upper leg part and mid leg
	btHingeConstraint* upper_mid_leg_hinge;
	upper_mid_leg_hinge = new btHingeConstraint(*mid_leg->rigidBody, *upper_leg->rigidBody,
												btVector3(0, 6.5, 0), btVector3(0, -6.5, 0),
												btVector3(direction.x, 0, direction.z), btVector3(-direction.z, 0, direction.x));
	upper_mid_leg_hinge->setLimit(btScalar(0), btScalar(glm::pi<float>()));
	dynamicsWorld->addConstraint(upper_mid_leg_hinge);

	//muscle to connect middle leg to upper
	shared_ptr<PhysicsController> mid_upper_leg_muscle = physicsFactory->CreateCylinder(0.5, 0.2, position - (direction * (leg_offset * 3)) + glm::vec3(0, -2, 0), q);
	btHingeConstraint* mid_upper_leg_muscle_hinge;

	mid_upper_leg_muscle_hinge = new btHingeConstraint(*mid_leg->rigidBody, *mid_upper_leg_muscle->rigidBody,
														btVector3(muscle_offset * direction.z, leg_offset, direction.x * -muscle_offset), btVector3(0, 0, 0),
														btVector3(direction.x, 0, direction.z), btVector3(-direction.z, 0, direction.x));
	mid_upper_leg_muscle_hinge->setLimit(btScalar(0), btScalar(glm::half_pi<float>()));
	dynamicsWorld->addConstraint(mid_upper_leg_muscle_hinge);

	//muscle to connect middle leg to lower leg
	shared_ptr<PhysicsController> mid_lower_leg_muscle = physicsFactory->CreateCylinder(0.5, 0.2, position - (direction * (leg_offset * 5) ) + glm::vec3(0, -2, 0), q);
	btHingeConstraint* mid_lower_leg_muscle_hinge;
	mid_lower_leg_muscle_hinge = new btHingeConstraint(*mid_lower_leg_muscle->rigidBody, *mid_leg->rigidBody,
														btVector3(0, 0, 0), btVector3(direction.z *  muscle_offset, -leg_offset, -direction.x * muscle_offset),
														btVector3(-direction.z, 0, direction.x), btVector3(direction.x, 0, direction.z));
	mid_lower_leg_muscle_hinge->setLimit(btScalar(0), btScalar(glm::half_pi<float>()));
	dynamicsWorld->addConstraint(mid_lower_leg_muscle_hinge);

	//lower leg
	shared_ptr<PhysicsController> lower_leg = physicsFactory->CreateCapsule(1, 3, glm::vec3(position.x, position.y, position.z) - (direction * (leg_offset * 8)), q);
	lower_leg->rigidBody->setDamping(btScalar(1), btScalar(1));
	//hinge joint between mid leg part and lower
	btHingeConstraint* mid_lower_leg_hinge;
	mid_lower_leg_hinge = new btHingeConstraint(*lower_leg->rigidBody, *mid_leg->rigidBody,
												btVector3(0, 6.5, 0), btVector3(0, -6.5, 0),
												btVector3(direction.x, 0, direction.z), btVector3(direction.x, 0, direction.z));
	mid_lower_leg_hinge->setLimit(btScalar(0), btScalar(glm::pi<float>()));
	dynamicsWorld->addConstraint(mid_lower_leg_hinge);

	//muscle to connect lower leg to middle leg
	shared_ptr<PhysicsController> lower_leg_muscle = physicsFactory->CreateCylinder(0.5, 0.2, position - (direction * (leg_offset * 7)) + glm::vec3(0, -2, 0), q);
	btHingeConstraint* lower_leg_muscle_hinge;
	lower_leg_muscle_hinge = new btHingeConstraint(*lower_leg->rigidBody, *lower_leg_muscle->rigidBody, 
													btVector3(muscle_offset * direction.z, leg_offset, direction.x * -muscle_offset), btVector3(0, 0, 0),
													btVector3(direction.x, 0, direction.z), btVector3(-direction.z, 0, direction.x));

	lower_leg_muscle_hinge->setLimit(btScalar(0), btScalar(glm::half_pi<float>()));
	dynamicsWorld->addConstraint(lower_leg_muscle_hinge);


	//slider joint between muscle pairs fixed to legs
	btTransform muscleTransform1, muscleTransform2;
	muscleTransform1.setIdentity();
	muscleTransform2.setIdentity();
	muscleTransform1.setRotation(GLToBtQuat(glm::angleAxis(270.0f, glm::vec3(0, 0, 1))));
	muscleTransform2.setRotation(GLToBtQuat(glm::angleAxis(270.0f, glm::vec3(0, 0, 1))));

	btSliderConstraint* upper_mid_leg_slider;
	btSliderConstraint* mid_lower_leg_slider;
	upper_mid_leg_slider = new btSliderConstraint(*upper_leg_muscle->rigidBody, *mid_upper_leg_muscle->rigidBody, muscleTransform1, muscleTransform2, true);
	mid_lower_leg_slider = new btSliderConstraint(*mid_lower_leg_muscle->rigidBody, *lower_leg_muscle->rigidBody, muscleTransform1, muscleTransform2, true);
	dynamicsWorld->addConstraint(upper_mid_leg_slider);
	dynamicsWorld->addConstraint(mid_lower_leg_slider);

	upper_mid_leg_slider->setLowerLinLimit(3.0f);
	upper_mid_leg_slider->setUpperLinLimit(15.0f);
	upper_mid_leg_slider->setPoweredLinMotor(true);
	upper_mid_leg_slider->setMaxLinMotorForce(10);
	upper_mid_leg_slider->setDampingDirLin(0.005F);
	upper_mid_leg_slider->setRestitutionLimLin(1.1F);

	mid_lower_leg_slider->setLowerLinLimit(3.0f);
	mid_lower_leg_slider->setUpperLinLimit(15.0f);
	mid_lower_leg_slider->setPoweredLinMotor(true);
	mid_lower_leg_slider->setMaxLinMotorForce(10);
	mid_lower_leg_slider->setDampingDirLin(0.005F);
	mid_lower_leg_slider->setRestitutionLimLin(1.1F);

	sliders.push_back(mid_lower_leg_slider);
	sliders.push_back(upper_mid_leg_slider);

	return upper_leg;
	
}

void Assignment::Cleanup()
{
	Game::Cleanup();
}
