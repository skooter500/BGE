#include "Lab3.h"
#include "Content.h"
#include "VectorDrawer.h"
#include "LazerBeam.h"
#include <math.h>
#include "Utils.h"

using namespace BGE;

Lab3::Lab3(void)
{
	elapsed = 10000;
}

float fireTimer = 0.0f;

bool Lab3::Initialise()
{
	std::shared_ptr<GameComponent> ground = make_shared<Ground>();
	Attach(ground);	

	ship1 = make_shared<GameComponent>(true);
	ship1->Attach(Content::LoadModel("cobramk3", glm::rotate(glm::mat4(1), 180.0f, glm::vec3(0,1,0))));
	ship1->Attach(make_shared<VectorDrawer>(glm::vec3(5,5,5)));
	ship1->transform->position = glm::vec3(-10, 2, -10);
	Attach(ship1);

	ship2 = make_shared<GameComponent>(true);
	ship2->Attach(Content::LoadModel("python", glm::rotate(glm::mat4(1), 180.0f, glm::vec3(0,1,0))));
	ship2->Attach(make_shared<VectorDrawer>(glm::vec3(5,5,5)));
	ship2->transform->position = glm::vec3(10, 2, -10);
	Attach(ship2);
	Game::Initialise();

	camera->transform->position = glm::vec3(0, 4, 20);
	return true;
}

void Lab3::Update()
{
	static float timeToFire = 1.0f / 2.0f;
	// Movement of ship1
	if (keyState[SDL_SCANCODE_U])
	{
		ship1->transform->position += ship1->transform->look * speed * Time::deltaTime;
	}
	if (keyState[SDL_SCANCODE_J])
	{
		ship1->transform->position -= ship1->transform->look * speed * Time::deltaTime;
	}
	if (keyState[SDL_SCANCODE_H])
	{
		ship1->transform->Yaw(Time::deltaTime * speed * speed);
	}
	if (keyState[SDL_SCANCODE_K])
	{
		ship1->transform->Yaw(-Time::deltaTime * speed * speed);
	}

	// Movement of ship2
	if (keyState[SDL_SCANCODE_UP])
	{
		ship2->transform->position += ship2->transform->look * speed * Time::deltaTime;
	}
	if (keyState[SDL_SCANCODE_DOWN])
	{
		ship2->transform->position -= ship2->transform->look * speed * Time::deltaTime;
	}
	if (keyState[SDL_SCANCODE_LEFT])
	{
		ship2->transform->Yaw(Time::deltaTime * speed * speed);
	}
	if (keyState[SDL_SCANCODE_RIGHT])
	{
		ship2->transform->Yaw(-Time::deltaTime * speed * speed);
	}

	if (keyState[SDL_SCANCODE_U])
	{
		ship1->transform->position += ship1->transform->look * speed * Time::deltaTime;
	}
	if (keyState[SDL_SCANCODE_J])
	{
		ship1->transform->position -= ship1->transform->look * speed * Time::deltaTime;
	}
	if (keyState[SDL_SCANCODE_H])
	{
		ship1->transform->Yaw(Time::deltaTime * speed * 5);
	}
	if (keyState[SDL_SCANCODE_K])
	{
		ship1->transform->Yaw(-Time::deltaTime * speed * 5);
	}
	elapsed += Time::deltaTime;

	glm::vec3 distance =
		ship1->transform->position - ship2->transform->position;

	int intDistance = glm::length(distance);

	if (glm::abs(intDistance) <= 5)
		PrintText("In Range!", glm::vec2(200, 200));
	else
		PrintText("Not in Range!", glm::vec2(200, 200));

	glm::vec3 v1 = glm::normalize(ship2->transform->look);
	glm::vec3 v2 = glm::normalize(ship1->transform->position - ship2->transform->position);
	float dotted = glm::dot(v1, v2);
	float angle = acos(dotted) * 180 / 3.14;

	fireTimer += Time::deltaTime;

	if ( angle > 45)
		PrintText("Ship is behind!", glm::vec2(400, 200));
	else
	{
		PrintText("Ship is in front!", glm::vec2(400, 200));
		
		if (fireTimer >= 0.5f)
		{
			shared_ptr<LazerBeam> lazer = make_shared<LazerBeam>();
			lazer->transform->position = ship2->transform->position;
			lazer->transform->look = ship2->transform->look;
			Attach(lazer);
			fireTimer = 0.0f;
		}
	}
	elapsed += Time::deltaTime;


	Game::Update();
}
