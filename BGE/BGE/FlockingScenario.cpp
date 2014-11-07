#include "FlockingScenario.h"
#include "Params.h"
#include "SteeringGame.h"
#include "Content.h"
#include "Utils.h"
#include "Sphere.h"


using namespace BGE;

FlockingScenario::FlockingScenario(void)
{
}

string FlockingScenario::Description()
{
	return string("Flocking Scenario");
}

void FlockingScenario::Initialise()
{
	shared_ptr<SteeringGame> game = dynamic_pointer_cast<SteeringGame>(Game::Instance()); 
	Params::Load("flocking");
	float range = Params::GetFloat("world_range");

	shared_ptr<GameComponent> bigFighter = make_shared<GameComponent>(true);
	bigFighter->tag = "steerable";
	glm::vec3 pos = RandomPosition(range);
	shared_ptr<SteeringController> bigFighterController = make_shared<SteeringController>();
	bigFighter->Attach(bigFighterController);
	bigFighter->transform->position = pos;
	bigFighterController->TurnOn(SteeringController::behaviour_type::obstacle_avoidance);
	bigFighterController->TurnOn(SteeringController::behaviour_type::pursuit);
	bigFighterController->TurnOn(SteeringController::behaviour_type::wander);
	bigFighterController->TurnOn(SteeringController::behaviour_type::sphere_constrain);
	bigFighter->transform->scale = glm::vec3(10, 10, 10);	
	bigFighter->Attach(Content::LoadModel("ferdelance", glm::rotate(glm::mat4(1), 180.0f, Transform::basisUp)));
	game->Attach(bigFighter);
	bigFighterController->Initialise();

	shared_ptr<GameComponent> fighter;
	shared_ptr<SteeringController> fighterController;
	for (int i = 0; i < Params::GetFloat("num_boids"); i++)
	{
		glm::vec3 pos = RandomPosition(range);

		fighter = make_shared<GameComponent>(true);
		fighter->tag = "steerable";
		fighterController = make_shared<SteeringController>();
		fighter->Attach(fighterController);
		fighterController->transform->position = fighter->transform->position = pos;
		fighterController->target = bigFighter;
		fighterController->TurnOffAll();
		fighterController->TurnOn(SteeringController::behaviour_type::separation);
		fighterController->TurnOn(SteeringController::behaviour_type::cohesion);
		fighterController->TurnOn(SteeringController::behaviour_type::alignment);
		fighterController->TurnOn(SteeringController::behaviour_type::wander);
		fighterController->TurnOn(SteeringController::behaviour_type::evade);
		fighterController->TurnOn(SteeringController::behaviour_type::sphere_constrain);
		fighterController->TurnOn(SteeringController::behaviour_type::obstacle_avoidance);
		fighter->Attach(Content::LoadModel("adder", glm::rotate(glm::mat4(1), 180.0f, Transform::basisUp)));
		fighter->transform->scale = glm::vec3(5,5, 5);
		
		fighterController->Initialise();
		game->Attach(fighter);
	}

	bigFighterController->target = fighter;

	int numObstacles = 10;
	float dist = (range * 2) / numObstacles;
	for (float x = -range; x < range; x += dist)
	{
		for (float z = -range; z < range; z += dist)
		{
			shared_ptr<Sphere> obstacle = make_shared<Sphere>(10);
			obstacle->tag = "obstacle";
			obstacle->transform->position = glm::vec3(x, 0, z);
			obstacle->Initialise();
			game->Attach(obstacle);
		}
	}

	bigFighterController->target = fighter;
	game->camFollower = make_shared<GameComponent>(true);
	shared_ptr<SteeringController> camController = make_shared<SteeringController>();
	game->camFollower->Attach(camController);
	camController->offset = glm::vec3(0,0,3);
	camController->leader = bigFighter;
	camController->transform->position = game->camFollower->transform->position = bigFighter->transform->position + camController->offset;
	
	camController->TurnOffAll();
	camController->TurnOn(SteeringController::behaviour_type::offset_pursuit);
	camController->TurnOn(SteeringController::behaviour_type::sphere_constrain);
	game->Attach(game->camFollower);	
}
