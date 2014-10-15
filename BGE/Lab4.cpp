#include "Lab4.h"
#include "Content.h"
#include "VectorDrawer.h"
#include "LazerBeam.h"
#include "FountainEffect.h"

using namespace BGE;

Lab4::Lab4(void)
{
	elapsed = 10000;
}

bool Lab4::Initialise()
{
	std::shared_ptr<GameComponent> ground = make_shared<Ground>();
	Attach(ground);	

	ship1 = make_shared<GameComponent>(true);
	ship1->Attach(Content::LoadModel("cobramk3", glm::rotate(glm::mat4(1), 180.0f, glm::vec3(0,1,0))));
	ship1->transform->position = glm::vec3(-10, 2, -10);
	ship1->Attach(make_shared<VectorDrawer>());
	Attach(ship1);

	ship2 = make_shared<GameComponent>(true);
	ship2->Attach(Content::LoadModel("ferdelance", glm::rotate(glm::mat4(1), 180.0f, glm::vec3(0, 1, 0))));
	ship2->Attach(make_shared<VectorDrawer>());
	ship2->transform->diffuse = glm::vec3(1.0f, 0.0f, 0.0f);
	ship2->transform->specular = glm::vec3(1.2f, 1.2f, 1.2f);

	ship2->transform->position = glm::vec3(10, 2, -10);
	Attach(ship2);

	// 500 in the constructor indicates the number of particles in the effect. 
	// You may need to compile in release mode or reduce the number of particles to get an acceptable framerate
	shared_ptr<FountainEffect> centFountain = make_shared<FountainEffect>(500, true);
	centFountain->transform->position.x = centFountain->transform->position.y = 0;
	centFountain->transform->position.x = centFountain->transform->position.y = 0;
	centFountain->transform->position.y = FOUNTAIN_HEIGHT;
	centFountain->transform->diffuse = glm::vec3(1,1,0); // Sets the colour of the fountain

	Attach(centFountain);

	// make a circle of fountains
	
	fountainTheta = 0.0f; 
	for (int i = 0 ; i < NUM_FOUNTAINS ; i ++)
	{
		
	}

	Game::Initialise();

	camera->transform->position = glm::vec3(0, 4, 20);
	return true;
}

void Lab4::Update(float timeDelta)
{	
	// Movement of ship2
	if (keyState[SDL_SCANCODE_UP])
	{
		ship2->transform->position += ship2->transform->look * speed * timeDelta;
	}
	if (keyState[SDL_SCANCODE_DOWN])
	{
		ship2->transform->position -= ship2->transform->look * speed * timeDelta;
	}
	if (keyState[SDL_SCANCODE_LEFT])
	{
		ship2->transform->Yaw(timeDelta * speed * speed);
	}
	if (keyState[SDL_SCANCODE_RIGHT])
	{
		ship2->transform->Yaw(-timeDelta * speed * speed);
	}
	
	// Put code in here to control the height of the fountains....
	// Use fountainTheta
	for (int i = 0 ; i < fountains.size() ; i ++)
	{
		
		
	}
	fountainTheta += timeDelta;
	if (fountainTheta >= glm::pi<float>() * 2.0f)
	{
		fountainTheta = 0.0f;
	}

	Game::Update(timeDelta);

	// Put your code here to calculate the world transform matrix for ship1
	// You need to include the rotation bit
	ship1->transform->world = glm::translate(glm::mat4(1), ship1->transform->position);
}
