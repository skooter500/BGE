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
	person->headCamera = true;

	ResetScene();

	if (!Game::Initialise()) {
		return false;
	}

	return true;
}

/*
// Note that pickedUp is passed by reference and so can be changed!!
void VRGame1::GravityGun(PhysicsController * & pickedUp, Hand * hand)
{
	if (hand->state == HandState::HandState_Closed)
	{
		float dist = 1000.0f;
		if (pickedUp == nullptr)
		{		
			btVector3 rayFrom = GLToBtVector(hand->pos); // Has to be some distance in front of the camera otherwise it will collide with the camera all the time
			btVector3 rayTo = GLToBtVector(hand->pos + (hand->look * dist));

			btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom, rayTo);
			dynamicsWorld->rayTest(rayFrom, rayTo, rayCallback);

			if (rayCallback.hasHit())
			{
				pickedUp = reinterpret_cast<PhysicsController*>(rayCallback.m_collisionObject->getUserPointer());
				if (pickedUp->parent == ground)
				{
					pickedUp = nullptr;
				}
			}
		}
		if (pickedUp != nullptr)
		{			
			float powerfactor = 4.0f; // Higher values causes the targets moving faster to the holding point.
			float maxVel = 3.0f;      // Lower values prevent objects flying through walls.
			float holdDist = 5.0f;
			// Calculate the hold point in front of the camera
			glm::vec3 holdPos = hand->pos + (hand->look * holdDist);

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
			Game::PrintText("Picked up: " + pickedUp->tag);
		}
	}
	else
	{
		pickedUp = nullptr;
	}
}
*/

void VRGame1::Update()
{
	//if (Game::Instance()->frame == 100)
	//{
	//	theSphere->FindComponentByTag("");
	//	if (theSphere != nullptr)
	//	{
	//		theSphere->alive = false;
	//		//theSphere = nullptr;
	//	}
	//}

	//if (Game::Instance()->frame == 105)
	//{
	//	theSphere = nullptr;
	//}

	string leftHandWhat = "Nothing";
	string rightHandWhat = "Nothing";



	const Uint8 * keyState = Game::Instance()->GetKeyState();

	float moveSpeed = speed;
	float timeToPass = 1.0f / fireRate;

	elapsed += Time::deltaTime;

	PrintText("Press R to reset scene");
	static bool lastPressed = false;
	if (keyState[SDL_SCANCODE_R])
	{
		if (! lastPressed)
		{
			ResetScene();
		}
		lastPressed = true;
	}
	else
	{
		lastPressed = false;
	}	

	if (person)
	{
		//GravityGun(leftHandPickedUp, & person->hands[0]);
		//GravityGun(rightHandPickedUp, & person->hands[1]);
	}

	Game::Update();
}

void VRGame1::Cleanup()
{
	Game::Cleanup();
}
