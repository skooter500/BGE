#include "Lab3.h"
#include "Content.h"
#include "VectorDrawer.h"
#include "LazerBeam.h"
#include "Utils.h"

using namespace BGE;

Lab3::Lab3(void)
{
	elapsed = 10000;
}


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

	// Check Distance to ship 2
	glm::vec3 toShip2 = ship2->transform->position - ship1->transform->position;
	if (glm::length(toShip2) < 5)
	{
		PrintText("In range");
	}
	else
	{
		PrintText("Not in range");
	}
	// Check in front of or behind
	toShip2 = glm::normalize(toShip2);
	float dot = glm::dot(toShip2, ship1->transform->look);
	if (dot < 0)
	{
		PrintText("Behind");
	}
	else
	{
		PrintText("In Front");
	}

	// Check in the FOV of half 45 degrees	
	float angle = glm::acos(dot);
	float halffov = glm::radians(45.0f) / 2.0f;
	if (angle < halffov)
	{
		if (elapsed > timeToFire)
		{
			shared_ptr<LazerBeam> lazer = make_shared<LazerBeam>();
			lazer->transform->position = ship1->transform->position;
			lazer->transform->look = ship1->transform->look;
			Attach(lazer);
			elapsed = 0.0f;
		}
		
		PrintText("In FOV");
	}
	else
	{
		PrintText("Not in FOV");
	}
	elapsed += Time::deltaTime;


	Game::Update();
}
