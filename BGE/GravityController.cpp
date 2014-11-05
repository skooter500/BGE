#include "GravityController.h"
#include "Game.h"
#include "GameComponent.h"
using namespace BGE;

GravityController::GravityController(void):GameComponent(true)
{
	gravity = glm::vec3(0, -1, 0);

}

void GravityController::Update(float timeDelta)
{
	transform->velocity += gravity * timeDelta;
	transform->position += transform->velocity * timeDelta;
}