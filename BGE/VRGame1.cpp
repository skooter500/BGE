#include "VRGame1.h"
#include <fmod.hpp>
#include <fmod_errors.h>
#include <sstream>
#include <string>
#include "Content.h"
#include "Utils.h"
#include <sapi.h>
#include <sphelper.h>
#include "Sphere.h"
#include "Params.h"

using namespace BGE;
using namespace std;

VRGame1::VRGame1(void)
{

	person = nullptr;
	
	fireRate = 5.0f;
	leftHandPickedUp= nullptr;
	rightHandPickedUp= nullptr;

	tag = "VR Game";
}

VRGame1::~VRGame1(void)
{
}

void VRGame1::ResetScene()
{
	Game::Instance()->ClearChildrenWithTag("Box");
	Game::Instance()->ClearChildrenWithTag("Model");
	Game::Instance()->ClearChildrenWithTag("Cylinder");
	Game::Instance()->ClearChildrenWithTag("Sphere");
	Game:Instance()->DeletePhysicsConstraints();
	
	physicsFactory->CreateWall(glm::vec3(-20, 0, 10), 5, 5);
	physicsFactory->CreateCapsuleRagdoll(glm::vec3(0, 20, -0));
	physicsFactory->CreateCapsuleRagdoll(glm::vec3(-10, 20, -30));
	physicsFactory->CreateVehicle(glm::vec3(10, 5, -40));

	physicsFactory->CreateVehicle(glm::vec3(-20, 5, -50));
	physicsFactory->CreateVehicle(glm::vec3(0, 5, -60));
}

bool VRGame1::Initialise() 
{
	dynamicsWorld->setGravity(btVector3(0,-20,0));

	physicsFactory->CreateCameraPhysics();
	physicsFactory->CreateGroundPhysics();

	person = make_shared<Person>();
	Attach(person);
	person->headCamera = false;

	ResetScene();

	if (!Game::Initialise()) {
		return false;
	}

	return true;
}

void VRGame1::FireProjectile(glm::vec3 pos, glm::vec3 look)
{
	glm::quat q(RandomFloat(), RandomFloat(), RandomFloat(), RandomFloat());
	glm::normalize(q);
	shared_ptr<PhysicsController> physicsComponent = physicsFactory->CreateSphere(1, pos, q);
	//soundSystem->Vibrate(200, 1.0f);
	float force = 3000.0f;
	physicsComponent->rigidBody->applyCentralForce(GLToBtVector(look) * force);
}

// Note that pickedUp is passed by reference and so can be changed!!
void VRGame1::GravityGun(SDL_Joystick * joy, int axis, PhysicsController * & pickedUp, Hand hand)
{
	int lhp = SDL_JoystickGetAxis(joy, axis);
	if (lhp > 0)
	{
		float dist = 1000.0f;
		if (pickedUp == NULL)
		{
			btVector3 rayFrom = GLToBtVector(hand.pos); // Has to be some distance in front of the camera otherwise it will collide with the camera all the time
			btVector3 rayTo = GLToBtVector(hand.pos + (hand.look * dist));

			btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom, rayTo);
			dynamicsWorld->rayTest(rayFrom, rayTo, rayCallback);

			if (rayCallback.hasHit())
			{
				pickedUp = reinterpret_cast<PhysicsController*>(rayCallback.m_collisionObject->getUserPointer());
				if (pickedUp->parent.get() == ground.get())
				{
					pickedUp = NULL;
				}
			}
		}
		if (pickedUp != NULL)
		{
			// Press Y to throw the thing away
			int yb = SDL_JoystickGetButton(joy, 13);
			if (yb)
			{
				float force = 300.0f;
				pickedUp->rigidBody->applyCentralForce(GLToBtVector(hand.look) * force);
				pickedUp = NULL;
				return;
			}
			float powerfactor = 4.0f; // Higher values causes the targets moving faster to the holding point.
			float maxVel = 3.0f;      // Lower values prevent objects flying through walls.
			float holdDist = 5.0f;
			// Calculate the hold point in front of the camera
			glm::vec3 holdPos = hand.pos + (hand.look * holdDist);

			glm::vec3 v = holdPos - pickedUp->transform->position; // direction to move the Target
			v *= powerfactor; // powerfactor of the GravityGun

			if (v.length() > maxVel)
			{
				// if the correction-velocity is bigger than maximum
				v = glm::normalize(v);
				v *= maxVel; // just set correction-velocity to the maximum
			}
			pickedUp->rigidBody->setLinearVelocity(GLToBtVector(v));
			pickedUp->rigidBody->activate();

		}
	}
	else
	{
		pickedUp = NULL;
	}
}

void VRGame1::Update()
{

	string leftHandWhat = "Nothing";
	string rightHandWhat = "Nothing";

	dynamicsWorld->stepSimulation(Time::deltaTime, 100);

	const Uint8 * keyState = Game::Instance()->GetKeyState();

	float moveSpeed = speed;
	float timeToPass = 1.0f / fireRate;

	elapsed += Time::deltaTime;

	PrintText("Press R to reset scene");
	static bool lastPressed = false;
	if (keyState[SDL_SCANCODE_R])
	{
		if (!lastPressed)
		{
			ResetScene();
		}
		lastPressed = true;
	}
	else
	{
		lastPressed = false;
	}

	SDL_Joystick * joy;
	if (SDL_NumJoysticks() > 0)
	{
		// Open joystick
		joy = SDL_JoystickOpen(0);
		if (joy)
		{
			// Check left button
			int lb = SDL_JoystickGetButton(joy, 8);
			if (lb && (elapsed > timeToPass))
			{
				float dist = 3.0f;
				glm::vec3 pos = person->hands[0].pos + (person->hands[0].look * dist);
				FireProjectile(pos, person->hands[0].look);
				elapsed = 0.0f;
			}

			// Check right button
			int rb = SDL_JoystickGetButton(joy, 9);
			if (rb && (elapsed > timeToPass))
			{
				float dist = 3.0f;
				glm::vec3 pos = person->hands[1].pos + (person->hands[1].look * dist);
				FireProjectile(pos, person->hands[1].look);
				elapsed = 0.0f;
			}

			// Check the A key and spawn a car
			int ab = SDL_JoystickGetButton(joy, 10);
			if (ab && (elapsed > timeToPass))
			{
				glm::vec3 point;
				bool hit = ground->rayIntersectsWorldPlane(camera->transform->position, camera->transform->look, point);
				if (hit)
				{
					point.y = 5;
					physicsFactory->CreateCapsuleRagdoll(point);
					soundSystem->PlaySound("spawn", point);
				}
				elapsed = 0.0f;
			}

			// Check the B key and spawn a ball
			int bb = SDL_JoystickGetButton(joy, 11);
			if (bb && (elapsed > timeToPass))
			{
				glm::vec3 point;
				bool hit = ground->rayIntersectsWorldPlane(camera->transform->position, camera->transform->look, point);
				if (hit)
				{
					point.y = 5;
					physicsFactory->CreateSphere(3, point, glm::quat());
					soundSystem->PlaySound("spawn", point);
				}
				elapsed = 0.0f;
			}
			// Check the X key to spawn a random object
			int xb = SDL_JoystickGetButton(joy, 12);
			if (xb && (elapsed > timeToPass))
			{
				glm::vec3 point;
				bool hit = ground->rayIntersectsWorldPlane(camera->transform->position, camera->transform->look, point);
				if (hit)
				{
					point.y = 5;
					physicsFactory->CreateRandomObject(point, glm::quat());
					soundSystem->PlaySound("spawn", point);
				}
				elapsed = 0.0f;
			}

			if (person)
			{
				GravityGun(joy, 4, leftHandPickedUp, person->hands[0]);
				GravityGun(joy, 5, rightHandPickedUp, person->hands[1]);
			}
		}

		if (SDL_JoystickGetAttached(joy)) {
			SDL_JoystickClose(joy);
		}
	}

	Game::Update();
}

void VRGame1::Cleanup()
{
	Game::Cleanup();
}
