#include "FPSController.h"
#include "Game.h"
#include "Params.h"
#include "Utils.h"

using namespace BGE;

FPSController::FPSController(void)
{
}

bool FPSController::Initialise()
{
	return GameComponent::Initialise();
}

FPSController::~FPSController(void)
{
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
		Game::Instance()->GetMainWindow()
		,midX
		,midY
		);
	GameComponent::Update();
	//Controller::Update(this, this->parent);
}
