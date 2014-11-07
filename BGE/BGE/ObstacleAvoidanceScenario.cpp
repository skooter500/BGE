#include "ObstacleAvoidanceScenario.h"
#include "Params.h"
#include "SteeringGame.h"
#include "Content.h"
#include "Sphere.h"

using namespace BGE;
using namespace std;

struct ObstacleParam
{
public:
	glm::vec3 pos;
	float radius;
	ObstacleParam(glm::vec3 pos, float radius)
	{
		this->pos = pos;
		this->radius = radius;
	}
};

ObstacleAvoidanceScenario::ObstacleAvoidanceScenario(void)
{
}

string ObstacleAvoidanceScenario::Description()
{
	return "Obstacle Avoidance Scenario";
}

void ObstacleAvoidanceScenario::Initialise()
{
	Params::Load("default");
	shared_ptr<SteeringGame> game = dynamic_pointer_cast<SteeringGame>(Game::Instance());

	shared_ptr<GameComponent>leader = make_shared<GameComponent>(true);
	leader->tag = "steerable";
	shared_ptr<SteeringController> leaderController = make_shared<SteeringController>();
	leader->Attach(leaderController);
	leaderController->transform->position = glm::vec3(10, 120, 20);            
	leaderController->TurnOn(SteeringController::behaviour_type::arrive);
	leaderController->TurnOn(SteeringController::behaviour_type::obstacle_avoidance);
	//leaderController->TurnOn(SteeringController::behaviour_type::separation);
	leaderController->TurnOn(SteeringController::behaviour_type::wall_avoidance);
	leaderController->targetPos = glm::vec3(10, 100, -550);
	leader->Attach(Content::LoadModel("viper", glm::rotate(glm::mat4(1), 180.0f, Transform::basisUp)));
	leader->transform->scale = glm::vec3(5,5,5);
	game->Attach(leader);

	// Add some Obstacles
	vector<ObstacleParam> obsParams;
	obsParams.push_back(ObstacleParam(glm::vec3(5, 115, -30), 5));
	
	obsParams.push_back(ObstacleParam(glm::vec3(-10, 126, -80), 17));
	obsParams.push_back(ObstacleParam(glm::vec3(10, 115, -120), 10));
	obsParams.push_back(ObstacleParam(glm::vec3(5, 120, -150), 12));
	obsParams.push_back(ObstacleParam(glm::vec3(-2, 125, -200), 20));
	obsParams.push_back(ObstacleParam(glm::vec3(-25, 80, -250), 10));
	obsParams.push_back(ObstacleParam(glm::vec3(20, 80, -250), 10));
	obsParams.push_back(ObstacleParam(glm::vec3(5, 130, -350), 35));

	for (int i = 0 ; i < obsParams.size() ; i ++)
	{
		shared_ptr<Sphere> obstacle = make_shared<Sphere>(obsParams[i].radius);
		obstacle->tag = "obstacle";
		obstacle->transform->position = obsParams[i].pos;
		obstacle->Initialise();
		game->Attach(obstacle);
	}

	// Now make a fleet
	int fleetSize = 10;
	float xOff = 12;
	float zOff = 12;
	for (int i = 2; i < fleetSize; i++)
	{
		for (int j = 0; j < i; j++)
		{
			float z = (i - 1) * +zOff;
			shared_ptr<GameComponent> fleet = make_shared<GameComponent>(true);
			fleet->tag = "steerable";
			shared_ptr<SteeringController> fleetController = make_shared<SteeringController>();
			fleet->Attach(fleetController);
			fleetController->leader = leaderController;
			fleetController->offset = glm::vec3((xOff * (-i / 2.0f)) + (j * xOff), 0, z);
			fleetController->transform->position = leaderController->transform->position + fleetController->offset;
			fleetController->TurnOn(SteeringController::behaviour_type::offset_pursuit);
			//fleetController->TurnOn(SteeringController::behaviour_type::separation);
			fleetController->TurnOn(SteeringController::behaviour_type::wall_avoidance);
			fleetController->TurnOn(SteeringController::behaviour_type::obstacle_avoidance);			
			fleet->transform->scale = glm::vec3(5,5,5);
			fleet->Attach(Content::LoadModel("cobramk1", glm::rotate(glm::mat4(1), 180.0f, Transform::basisUp)));
			game->Attach(fleet);
		}
	}

	game->camFollower = make_shared<GameComponent>(true);
	//game->camFollower->tag = "steerable";
	shared_ptr<SteeringController> camController = make_shared<SteeringController>();
	game->camFollower->Attach(camController);
	camController->offset = glm::vec3(0,0,3);
	camController->leader = leader;
	camController->transform->position = glm::vec3(0, 115, fleetSize * zOff);;
	camController->offset = glm::vec3(0, 5, fleetSize * zOff);
	camController->TurnOffAll();
	camController->TurnOn(SteeringController::behaviour_type::offset_pursuit);
	camController->TurnOn(SteeringController::behaviour_type::wall_avoidance);
	camController->TurnOn(SteeringController::behaviour_type::obstacle_avoidance);
	game->Attach(game->camFollower);	
	game->camera->transform->position = camController->transform->position;
	shared_ptr<Ground> ground = make_shared<Ground>();
	game->Attach(ground);
	game->ground= ground;
}
