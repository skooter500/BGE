#include "LeapHands.h"
#include "Utils.h"
#include "LineDrawer.h"
#include "PhysicsController.h"
#include "Game.h"
#include "KinematicMotionState.h"
#include "Params.h"

using namespace Leap;
using namespace BGE;

struct BoneData
{
	string key;
	glm::vec3 pos[2];
	bool knobs;
	BoneData(string key, glm::vec3 start, glm::vec3 end, bool knobs)
	{
		this->key = key;
		pos[0] = start;
		pos[1] = end;
		this->knobs = knobs;
	}
	BoneData()
	{

	}
};

std::map<string, BoneData> boneData;

const int boneDataPerHand = 28;

BGE::LeapHands::LeapHands() :GameComponent(true)
{
	trackedHands = 0;
	InitializeCriticalSection(& criticalSection);
}


BGE::LeapHands::~LeapHands()
{
	DeleteCriticalSection(& criticalSection);
}

bool BGE::LeapHands::Initialise()
{
	mapper = make_shared<SkeletonMapper>(This(), glm::vec3(0.1f, 0.1f, 0.1f));
	handTransform = make_shared<Transform>();
	if (Params::GetBool("leapHeadMode"))
	{

		controller.setPolicyFlags(Leap::Controller::PolicyFlag::POLICY_OPTIMIZE_HMD);
		//controller.config().setBool("head_mounted_display_mode", true);
	}

	controller.addListener(* this);

	pinch = false;
	
	return GameComponent::Initialise();
}

float lastSpawned = 10.0f;

void BGE::LeapHands::Update()
{	

	Game::Instance()->PrintFloat("Pinch distance", pinchDist);

	list<shared_ptr<GameComponent>>::iterator cit = children.begin();
	while (cit != children.end())
	{
		(*cit ++)->transform->position = glm::vec3(0, -1000, 0);
	}

	EnterCriticalSection(&criticalSection);
	map<string, BoneData>::iterator it = boneData.begin();
	int currentBone = 0;
	if (trackedHands > 0)
	{
		while (it != boneData.end())
		{
			BoneData bone = it->second;
			mapper->UpdateBone(bone.key, bone.pos[0], bone.pos[1], bone.knobs);
			++it;
			++currentBone;
			if ((trackedHands == 1) && (currentBone == boneDataPerHand))
			{
				break;
			}
		}
		TransformHand();
	}	
	LeaveCriticalSection(&criticalSection);

	if (lastSpawned > 1.0f)
	{
		switch (spawn)
		{
			case none:
			{
				break;
			}
			case model:
			{
				glm::vec3 point;
				bool hit = Game::Instance()->ground->rayIntersectsWorldPlane(
					Game::Instance()->camera->transform->position,
					Game::Instance()->camera->transform->look, point);
				if (hit)
				{
					Game::Instance()->physicsFactory->CreateRandomObject(point, glm::quat(), glm::vec3(7, 7, 7));
					Game::Instance()->soundSystem->PlaySound("spawn", point);
				}
				break;
			}
			case vehicle:
			{
				glm::vec3 point;				
				bool hit = Game::Instance()->ground->rayIntersectsWorldPlane(
					Game::Instance()->camera->transform->position,
					Game::Instance()->camera->transform->look, point);
				if (hit)
				{
					point.y = 20;
					Game::Instance()->physicsFactory->CreateCapsuleRagdoll(point);
					Game::Instance()->soundSystem->PlaySound("spawn", point);
				}
				break;
			}
				
		}
		lastSpawned = 0.0f;

		if (pinch)
		{

		}
	}
	spawn = none;
	lastSpawned += Time::deltaTime;
	
	GameComponent::Update();
}

void BGE::LeapHands::Draw()
{
	GameComponent::Draw();
}

void BGE::LeapHands::TransformHand()
{
	if (!Params::GetBool("leapHeadMode"))
	{
		SDL_Joystick *joy;
		if (SDL_NumJoysticks() > 0) {
			// Open joystick
			joy = SDL_JoystickOpen(0);
			if (joy) {
				stringstream ss;

				float range = 1;

				int useHandsAxis = SDL_JoystickGetAxis(joy, 4);
				Game::Instance()->PrintFloat("Axis:", useHandsAxis);
				handTransform->look = Game::Instance()->camera->transform->look;
				handTransform->right = Game::Instance()->camera->transform->right;
				if (useHandsAxis > 0)
				{
					int strafeAxis = SDL_JoystickGetAxis(joy, 0);

					if (glm::abs<int>(strafeAxis) > 8000)
					{
						float strafe = ((float)strafeAxis / (float)numeric_limits<short int>::max()) * range;
						handTransform->Strafe(strafe);
					}

					int walkAxis = SDL_JoystickGetAxis(joy, 1);
					CheckOverflow(walkAxis);
					if (glm::abs<int>(walkAxis) > 8000)
					{
						float walk = ((float)walkAxis / (float)numeric_limits<short int>::max()) * range;

						handTransform->Walk(-walk);
					}

					int flyAxis = SDL_JoystickGetAxis(joy, 3);
					CheckOverflow(flyAxis);
					if (glm::abs<int>(flyAxis) > 8000)
					{
						float fly = ((float)flyAxis / (float)numeric_limits<short int>::max()) * (range / 4.0f);

						handTransform->Fly(-fly);
					}
				}
			}
			else {
				Game::Instance()->PrintText("Could not get controller!!");
			}

			// Close if opened
			if (SDL_JoystickGetAttached(joy)) {
				SDL_JoystickClose(joy);
			}
		}
		else
		{
			Game::Instance()->PrintText("No game controller detected");
		}

		TransformChildren(handTransform);
	}
	else
	{
		if ((Params::GetBool("leapHeadMode")))
		{
			shared_ptr<Transform> cameraTransform = Game::Instance()->camera->transform;
			list<shared_ptr<GameComponent>>::iterator it = children.begin();
			while (it != children.end())
			{
				shared_ptr<GameComponent> child = *it;
				glm::vec3 pos = child->transform->position;
				pos = TransformPointRelativeToCamera(pos);
				// Rotate the bodies by the camera rotation
				child->transform->orientation = cameraTransform->orientation * child->transform->orientation;
				child->transform->position = pos;
				it++;
			}
		}
	}
}

glm::vec3 BGE::LeapHands::TransformPointRelativeToCamera(glm::vec3 pos)
{
	float distance = 20.0f;
	Transform temp = *Game::Instance()->camera->transform;
	temp.position += temp.look * distance;

	pos = temp.TransformPosition(pos);

	return pos;
}

void LeapHands::Cleanup()
{
	controller.removeListener(*this);
	GameComponent::Cleanup();
}

void LeapHands::onInit(const Controller& controller) 
{
	std::cout << "Initialized" << std::endl;
}

void LeapHands::onConnect(const Controller& controller) 
{
	std::cout << "Connected" << std::endl;
	controller.enableGesture(Gesture::TYPE_CIRCLE);
}

void LeapHands::onDisconnect(const Controller& controller) 
{
	// Note: not dispatched when running in a debugger.
	std::cout << "Disconnected" << std::endl;
}

void LeapHands::onExit(const Controller& controller) 
{
	std::cout << "Exited" << std::endl;
}

void LeapHands::onFocusGained(const Controller& controller) {
	std::cout << "Focus Gained" << std::endl;
}

void LeapHands::onFocusLost(const Controller& controller) {
	std::cout << "Focus Lost" << std::endl;
}

void LeapHands::onDeviceChange(const Controller& controller) {
	std::cout << "Device Changed" << std::endl;
	const DeviceList devices = controller.devices();

	for (int i = 0; i < devices.count(); ++i) {
		std::cout << "id: " << devices[i].toString() << std::endl;
		std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
	}
}

void LeapHands::onServiceConnect(const Controller& controller) {
	std::cout << "Service Connected" << std::endl;
}

void LeapHands::onServiceDisconnect(const Controller& controller) {
	std::cout << "Service Disconnected" << std::endl;
}

void LeapHands::onFrame(const Controller& controller) 
{
	//Game::Instance()->PrintFloat("Hand children: ", childrenMap.size());	
	std::map<string, BoneData> tempBoneData;
	HandList hands = controller.frame().hands();
	int handId = 0;

	hands[0].fingers()[(int) Finger::Type::TYPE_THUMB];

	for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
		// Get the first hand
		const Hand hand = *hl;
		// Get fingers
		const FingerList fingers = hand.fingers();
		int fingerId = 0;
		bool firstFinger = true;
		Finger previousFinger;

		stringstream ass;
		ass << "Arm: 0 Hand: " << handId;
		tempBoneData[ass.str()] = BoneData(ass.str(), LeapToGlVec3(hand.arm().wristPosition()), LeapToGlVec3(hand.arm().elbowPosition()), true);
		ass.clear();

		glm::vec3 thumbBone = LeapToGlVec3(hand.fingers()[Finger::Type::TYPE_THUMB].bone(Bone::Type::TYPE_DISTAL).nextJoint());
		glm::vec3 indexBone = LeapToGlVec3(hand.fingers()[Finger::Type::TYPE_INDEX].bone(Bone::Type::TYPE_DISTAL).nextJoint());
		pinchDist = glm::length(thumbBone - indexBone);
		if (pinchDist < 5.0f)
		{
			pinch = true;
		}
		else
		{
			pinch = false;
		}

		for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
			const Finger finger = *fl;

			// Get finger bones
			for (int b = 0; b < 4; ++b) {
				Bone::Type boneType = static_cast<Bone::Type>(b);
				Bone bone = finger.bone(boneType);
				stringstream ss;
				ss << "Hand: " << handId << " Finger: " << fingerId << " Bone: " << b;
				tempBoneData[ss.str()] = BoneData(ss.str(), LeapToGlVec3(bone.prevJoint()), LeapToGlVec3(bone.nextJoint()), true);


			}
			// Draw some other bits of the hand
			if (!firstFinger)
			{
				for (int b = 0; b < 2; ++b)
				{
					stringstream ss;
					ss << "Hand: " << handId << "Finger: " << (fingerId - 1) << "Finger: " << (fingerId) << " Bone: " << b;
					Bone startBone = previousFinger.bone(static_cast<Bone::Type>(b));
					Bone endBone = finger.bone(static_cast<Bone::Type>(b));
					if ((b == 1) && (fingerId == 1))
					{
						tempBoneData[ss.str()] = BoneData(ss.str(), LeapToGlVec3(startBone.nextJoint()), LeapToGlVec3(endBone.prevJoint()), false);
					}
					else
					{
						tempBoneData[ss.str()] = BoneData(ss.str(), LeapToGlVec3(startBone.prevJoint()), LeapToGlVec3(endBone.prevJoint()), false);
					}
				}
			}

			const GestureList gestures = controller.frame().gestures();
			for (int g = 0; g < gestures.count(); ++g) 
			{
				Gesture gesture = gestures[g];

				switch (gesture.type())
				{
					case Gesture::TYPE_CIRCLE:
					{
						CircleGesture circle = gesture;
						if (gesture.durationSeconds() > 1)
						{
							if (circle.pointable().direction().angleTo(circle.normal()) <= PI / 2) {
								
								spawn = vehicle;
							}
							else {
								spawn = model;
							}
						}
					}
				}
			}

			previousFinger = finger;
			firstFinger = false;
			++fingerId;
		}
		++handId;
	}	
	EnterCriticalSection(&criticalSection);
	trackedHands = handId;
	map<string, BoneData>::iterator it = tempBoneData.begin();
	boneData = tempBoneData;
	LeaveCriticalSection(&criticalSection);
}