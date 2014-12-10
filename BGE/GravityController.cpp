#include "GravityController.h"
#include "Utils.h"


BGE::GravityController::GravityController()
{
	gravity = glm::vec3(0.0f, -9.8f, 0.0f);
}


BGE::GravityController::~GravityController()
{
}

void BGE::GravityController::Update()
{
	transform->velocity += gravity * Time::deltaTime;
	transform->position += transform->velocity * Time::deltaTime;

	if (transform->position.y - transform->scale.x < 0)
	{
		transform->velocity = - transform->velocity;
		transform->position.y = transform->scale.y;
	}
}
