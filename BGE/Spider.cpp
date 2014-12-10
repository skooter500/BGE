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
#include "Spider.h"

using namespace BGE;

Spider::Spider(void)
{
	float movementDuration = 0;
}

Spider::~Spider(void)
{
}

bool Spider::Initialise()
{
	physicsFactory->CreateGroundPhysics();

	physicsFactory->CreateCameraPhysics();
	dynamicsWorld->setGravity(btVector3(0, -9.8, 0));

	//used for controlling opening and closing legs
	counter = 0;
	openingDuration = 8;
	legDirection = 1; //negative closes leg, positive opens
	speed = 1.5;

	body = createSpider(glm::vec3(0, 10, 0));

	if (!Game::Initialise()) {
		return false;
	}

	camera->transform->position = glm::vec3(0, 50, 100);

	return true;
}


void Spider::Update()
{
	/*
	movementDuration += Time::deltaTime;
	btVector3 walk_direction = GLToBtVector(body->transform->look);
	float speed = 4500;
	if ( movementDuration < 10)
	body->rigidBody->applyCentralForce(btVector3(speed, 0, speed) * walk_direction);
	else if (movementDuration < 20)
	body->rigidBody->applyCentralForce(btVector3(-speed, 0, -speed) * walk_direction);
	else
	movementDuration = 0;
	*/
	animateLegs(Time::deltaTime);
	Game::Update();
}

shared_ptr<PhysicsController> Spider::createSpider(glm::vec3 position)
{

	float thorax_radius = 4.0f;
	shared_ptr<PhysicsController> body_part_thorax = physicsFactory->CreateSphere(thorax_radius, position, glm::quat());
	btScalar mass = 300;
	btVector3 inertia(0, 0, 0);
	body_part_thorax->rigidBody->setMassProps(mass, inertia);
	body_part_thorax->rigidBody->updateInertiaTensor();
	reColour(body_part_thorax, 174, 121, 68);

	//allow only rotation around y axis (prevent it from falling over)
	//body_part_thorax->rigidBody->setAngularFactor(btVector3(0, 1, 0));

	//divided by number of legs
	num_legs = 12;
	float theta = (glm::pi<float>() * 2) / num_legs;

	//legs will move in two groups. Each pair (going from front to back) move in opposing motions.
	bool legGroup = true;
	bool frontGroup;

	for (int i = 1; i < num_legs; i++)
	{
		/*The movement of the front legs differs from that of the rear legs.
		The front legs should elevate as they open and descend as they close to create a pulling motion
		The movement of the rear legs should open as they descend and raise as they open to create a pushing motion
		so we need to seperate them into two groups*/

		switch (i)
		{
		case FRONT_RIGHT:
		case FRONT_LEFT:
		case MID_FRONT_LEFT:
		case MID_FRONT_RIGHT:
			frontGroup = true;
			break;
		case HIND_LEFT:
		case HIND_RIGHT:
		case MID_HIND_LEFT:
		case MID_HIND_RIGHT:
			frontGroup = false;
			break;
		}

		if (i % 3 != 0)
		{
			glm::vec3 coxa_position;
			coxa_position = glm::vec3(position.x + (glm::sin(theta * i) * (thorax_radius)), position.y, position.z + (glm::cos(theta * i) * (thorax_radius)));

			glm::vec3 leg_direction = position - coxa_position;
			leg_direction = glm::normalize(leg_direction);
			shared_ptr<PhysicsController> leg = createLeg(coxa_position, leg_direction, glm::angleAxis(90.0f, glm::vec3(0, 1, 0)), legGroup, frontGroup);

			btTransform localA, localB;
			localA.setIdentity();
			localB.setIdentity();
			localA.setOrigin(btVector3(coxa_position.x, 0, coxa_position.z));
			localA.setRotation(GLToBtQuat(glm::angleAxis(180.0f, glm::vec3(leg_direction.x, 0, leg_direction.z))));
			localB.setRotation(GLToBtQuat(glm::angleAxis(180.0f, glm::vec3(leg_direction.x, 1, leg_direction.z))));
			localB.setOrigin(btVector3(0, 3.5, 0));

			btConeTwistConstraint* coxa_leg = new btConeTwistConstraint(*leg->rigidBody, *body_part_thorax->rigidBody, localB, localA);
			coxa_leg->setLimit(glm::radians(30.0f), glm::radians(10.0f), glm::radians(10.0f)); //cone twist limits only allowing small movements
			coxa_leg->setOverrideNumSolverIterations(150);

			dynamicsWorld->addConstraint(coxa_leg, true);
			legGroup = (legGroup) ? false : true;	//switch leg group (for alternating movement)
		}

	}

	return body_part_thorax;
}

shared_ptr<PhysicsController> Spider::createLeg(glm::vec3 position, glm::vec3 direction, glm::quat orientation, bool group, bool frontGroup)
{
	glm::quat q = orientation * glm::angleAxis(90.0f, direction);

	float leg_offset = 6.50f / 6.0f;
	float muscle_offset = 1;
	btScalar mass = 30;
	btVector3 inertia(0, 0, 0);

	//upper leg 
	shared_ptr<PhysicsController> upper_leg = physicsFactory->CreateCapsule(0.5, 1.5, glm::vec3(position.x, position.y, position.z), q);
	upper_leg->shape->calculateLocalInertia(mass, inertia);
	upper_leg->rigidBody->setMassProps(mass, inertia);
	upper_leg->rigidBody->updateInertiaTensor();
	reColour(upper_leg, 128, 90, 51);

	/*The upper leg will be the object that will have upward force applied to it in order to lift and lower the leg
	assigned to group based on parameter passed in*/
	(frontGroup) ? front_legs.push_back(upper_leg) : hind_legs.push_back(upper_leg);

	//muscle to connect upper leg and middle leg
	shared_ptr<PhysicsController> upper_leg_muscle = physicsFactory->CreateCylinder(0.25, 0.1, position - (direction * leg_offset) + glm::vec3(0, -1, 0), q);
	reColour(upper_leg_muscle, 0, 0, 0);
	btHingeConstraint* upper_leg_muscle_hinge;
	upper_leg_muscle_hinge = new btHingeConstraint(*upper_leg_muscle->rigidBody, *upper_leg->rigidBody,
		btVector3(0, 0, 0), btVector3(direction.x * muscle_offset, -leg_offset, direction.z * muscle_offset),
		btVector3(direction.x, 0, direction.z), btVector3(-direction.z, 0, direction.x));	//perpendicular vector (swap points and negate one)


	upper_leg_muscle_hinge->setLimit(btScalar(0), btScalar(glm::half_pi<float>()));
	upper_leg_muscle_hinge->setOverrideNumSolverIterations(50);
	dynamicsWorld->addConstraint(upper_leg_muscle_hinge, true);	//passing in true to disable collision between bodies of hinge


	//middle leg 
	shared_ptr<PhysicsController> mid_leg = physicsFactory->CreateCapsule(0.5, 1.5, glm::vec3(position.x, position.y, position.z) - (direction * (leg_offset * 4)), q);
	mid_leg->shape->calculateLocalInertia(mass, inertia);
	mid_leg->rigidBody->setMassProps(mass, inertia);
	mid_leg->rigidBody->updateInertiaTensor();
	reColour(mid_leg, 77, 54, 32);

	//hinge joint between upper leg part and mid leg
	btHingeConstraint* upper_mid_leg_hinge;
	upper_mid_leg_hinge = new btHingeConstraint(*mid_leg->rigidBody, *upper_leg->rigidBody,
		btVector3(0, 3.25, 0), btVector3(0, -3.25, 0),
		btVector3(direction.x, 0, direction.z), btVector3(-direction.z, 0, direction.x));
	upper_mid_leg_hinge->setLimit(btScalar(0), btScalar(glm::pi<float>()));

	upper_mid_leg_hinge->setOverrideNumSolverIterations(50);
	dynamicsWorld->addConstraint(upper_mid_leg_hinge, true);

	//muscle to connect middle leg to upper
	shared_ptr<PhysicsController> mid_upper_leg_muscle = physicsFactory->CreateCylinder(0.25, 0.1, position - (direction * (leg_offset * 3)) + glm::vec3(0, -1, 0), q);
	reColour(mid_upper_leg_muscle, 0, 0, 0);
	btHingeConstraint* mid_upper_leg_muscle_hinge;

	mid_upper_leg_muscle_hinge = new btHingeConstraint(*mid_leg->rigidBody, *mid_upper_leg_muscle->rigidBody,
		btVector3(muscle_offset * direction.z, leg_offset, direction.x * -muscle_offset), btVector3(0, 0, 0),
		btVector3(direction.x, 0, direction.z), btVector3(-direction.z, 0, direction.x));
	mid_upper_leg_muscle_hinge->setLimit(btScalar(0), btScalar(glm::half_pi<float>()));
	mid_upper_leg_muscle_hinge->setOverrideNumSolverIterations(20);
	dynamicsWorld->addConstraint(mid_upper_leg_muscle_hinge, true);

	//muscle to connect middle leg to lower leg
	shared_ptr<PhysicsController> mid_lower_leg_muscle = physicsFactory->CreateCylinder(0.25, 0.1, position - (direction * (leg_offset * 5)) + glm::vec3(0, -1, 0), q);
	reColour(mid_lower_leg_muscle, 0, 0, 0);
	btHingeConstraint* mid_lower_leg_muscle_hinge;
	mid_lower_leg_muscle_hinge = new btHingeConstraint(*mid_lower_leg_muscle->rigidBody, *mid_leg->rigidBody,
		btVector3(0, 0, 0), btVector3(muscle_offset * direction.z, -leg_offset, -direction.x * muscle_offset),
		btVector3(-direction.z, 0, direction.x), btVector3(direction.x, 0, direction.z));
	mid_lower_leg_muscle_hinge->setLimit(btScalar(0), btScalar(glm::half_pi<float>()));
	mid_lower_leg_muscle_hinge->setOverrideNumSolverIterations(20);
	dynamicsWorld->addConstraint(mid_lower_leg_muscle_hinge, true);

	//lower leg
	shared_ptr<PhysicsController> lower_leg = physicsFactory->CreateCapsule(0.5, 1.5, glm::vec3(position.x, position.y, position.z) - (direction * (leg_offset * 8)), q);
	lower_leg->shape->calculateLocalInertia(mass, inertia);
	lower_leg->rigidBody->setMassProps(mass, inertia);
	lower_leg->rigidBody->updateInertiaTensor();
	//reduce leg bouncing when it drops to ground
	lower_leg->rigidBody->setRestitution(0.01f);
	lower_leg->rigidBody->setFriction(2500.0f);
	reColour(lower_leg, 33, 23, 14);

	//hinge joint between mid leg part and lower
	btHingeConstraint* mid_lower_leg_hinge;
	mid_lower_leg_hinge = new btHingeConstraint(*lower_leg->rigidBody, *mid_leg->rigidBody,
		btVector3(0, 3.25, 0), btVector3(0, -3.25, 0),
		btVector3(direction.x, 0, direction.z), btVector3(direction.x, 0, direction.z));
	mid_lower_leg_hinge->setLimit(btScalar(0), btScalar(glm::pi<float>()));
	mid_lower_leg_hinge->setOverrideNumSolverIterations(50);
	dynamicsWorld->addConstraint(mid_lower_leg_hinge, true);

	//muscle to connect lower leg to middle leg
	shared_ptr<PhysicsController> lower_leg_muscle = physicsFactory->CreateCylinder(0.25, 0.1, position - (direction * (leg_offset * 7)) + glm::vec3(0, -1, 0), q);
	reColour(lower_leg_muscle, 0, 0, 0);
	btHingeConstraint* lower_leg_muscle_hinge;
	lower_leg_muscle_hinge = new btHingeConstraint(*lower_leg->rigidBody, *lower_leg_muscle->rigidBody,
		btVector3(muscle_offset * direction.z, leg_offset, direction.x * -muscle_offset), btVector3(0, 0, 0),
		btVector3(direction.x, 0, direction.z), btVector3(-direction.z, 0, direction.x));

	lower_leg_muscle_hinge->setLimit(btScalar(0), btScalar(glm::half_pi<float>()));
	lower_leg_muscle_hinge->setOverrideNumSolverIterations(20);
	dynamicsWorld->addConstraint(lower_leg_muscle_hinge, true);


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
	dynamicsWorld->addConstraint(upper_mid_leg_slider, true);
	dynamicsWorld->addConstraint(mid_lower_leg_slider, true);

	//controls for motion limits of the sliders
	upper_mid_leg_slider->setOverrideNumSolverIterations(50);
	upper_mid_leg_slider->setPoweredLinMotor(true);
	upper_mid_leg_slider->setMaxLinMotorForce(8);
	upper_mid_leg_slider->setLowerLinLimit(2.0f);
	upper_mid_leg_slider->setUpperLinLimit(4.0f);

	mid_lower_leg_slider->setOverrideNumSolverIterations(50);
	mid_lower_leg_slider->setPoweredLinMotor(true);
	mid_lower_leg_slider->setMaxLinMotorForce(6);
	mid_lower_leg_slider->setLowerLinLimit(2.0f);
	mid_lower_leg_slider->setUpperLinLimit(4.0f);

	if (group)
	{
		slider_group_1.push_back(mid_lower_leg_slider);
		slider_group_1.push_back(upper_mid_leg_slider);
	}
	else
	{
		slider_group_2.push_back(mid_lower_leg_slider);
		slider_group_2.push_back(upper_mid_leg_slider);
	}

	return upper_leg;

}

//Method to recolor game components taking values (0-255) for red, green, blue
void Spider::reColour(shared_ptr<GameComponent> o, float r, float g, float b)
{
	o->transform->diffuse.r = ((1.0f / 255) * r);
	o->transform->diffuse.g = ((1.0f / 255) * g);
	o->transform->diffuse.b = ((1.0f / 255) * b);
}

void Spider::animateLegs(float timeDelta)
{
	for (it_group_1 = slider_group_1.begin(), it_group_2 = slider_group_2.begin();
		it_group_1 != slider_group_1.end() || it_group_2 != slider_group_2.end();
		++it_group_1, ++it_group_2)
	{
		counter += timeDelta;
		if (counter > openingDuration)
		{
			if (legDirection < 1)
			{
				legDirection = 1;
			}
			else
			{
				legDirection = -1;
			}
			counter = 0;
		}

		(*it_group_1)->setTargetLinMotorVelocity(legDirection * speed);
		(*it_group_2)->setTargetLinMotorVelocity(legDirection * speed * -1);
	}


	for (it_front_legs = front_legs.begin(), it_hind_legs = hind_legs.begin();
		it_front_legs != front_legs.end(), it_hind_legs != hind_legs.end();
		++it_front_legs, ++it_hind_legs)
	{
		(*it_front_legs)->rigidBody->applyForce(btVector3(0, 4000 * legDirection, 0), btVector3(0, 0, 0));
		(*it_hind_legs)->rigidBody->applyForce(btVector3(0, 4000 * -legDirection, 0), btVector3(0, 0, 0));
	}
}


void Spider::Cleanup()
{
	Game::Cleanup();
}
