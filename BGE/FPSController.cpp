#include "FPSController.h"
#include "Game.h"
#include "Params.h"
#include "Utils.h"

using namespace BGE;

FPSController::FPSController(void)
{
	gravity = glm::vec3(0, -9.8f, 0);
	jumping = false;
}

bool FPSController::Initialise()
{
	return GameComponent::Initialise();
}

FPSController::~FPSController(void)
{
}

void FPSController::Jump(float height, float duration)
{
	// Dont allow Double Jumping
	if (jumping)
	{
		return;
	}
	jumping = true;
	jumpTheta = 0.0f;
	jumpHeight = height;
	jumpDuration = duration;
	jumpY = transform->position.y;

	// For Physics Jumping use this instead
	// See: http://math.stackexchange.com/questions/785375/calculate-initial-velocity-to-reach-height-y
	transform->velocity.y = glm::sqrt(2.0f * glm::abs(gravity.y) * jumpHeight);
}

void FPSController::Update()
{
	const Uint8 * keyState = Game::Instance()->GetKeyState();

	float moveSpeed = speed;

	if (keyState[SDL_SCANCODE_LSHIFT])
	{
		moveSpeed *= 10.0f;
	}

	if (keyState[SDL_SCANCODE_W])
	{
		transform->Walk(moveSpeed * Time::deltaTime);
	}

	if (keyState[SDL_SCANCODE_S])
	{
		transform->Walk(-moveSpeed * Time::deltaTime);
	}

	if (keyState[SDL_SCANCODE_A])
	{
		transform->Strafe(-moveSpeed * Time::deltaTime);
	}

	if (keyState[SDL_SCANCODE_D])
	{
		transform->Strafe(moveSpeed * Time::deltaTime);
	}

	if (keyState[SDL_SCANCODE_J])
	{
		Jump(20, 2);
	}

	int x, y;
	int midX, midY;
	SDL_GetMouseState(&x,&y);
	midX = Params::GetFloat("width") / 2;
	midY = Params::GetFloat("height") / 2;
	float yaw, pitch;
	yaw = midX - x;
	pitch = midY - y;

	float scale = 0.1f;
	if (yaw != 0)
	{
		transform->Yaw(yaw * scale);
	}
	if (pitch != 0)
	{
		transform->Pitch(pitch * scale);
	}
	SDL_WarpMouseInWindow(
		NULL
		,midX
		,midY
		);

	if (jumping)
	{
		transform->position.y = jumpY + (glm::sin(jumpTheta) * jumpHeight);
		float thetaInc = (glm::pi<float>() / jumpDuration) * Time::deltaTime;
		jumpTheta += thetaInc;
		if (jumpTheta > glm::pi<float>())
		{
		jumping = !jumping;
		transform->position.y = jumpY;
		}

		// Or Physics Jumping...
		
		/*
		transform->velocity += gravity * Time::deltaTime;
		transform->position += transform->velocity * Time::deltaTime;
		if (transform->position.y < jumpY)
		{
			jumping = !jumping;
			transform->position.y = jumpY;
		}
		*/
	}
	
	GameComponent::Update();
}
