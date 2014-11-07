#include "Lab6.h"
#include "Content.h"
#include "VectorDrawer.h"
#include "LazerBeam.h"
#include "FountainEffect.h"
#include "Utils.h"


using namespace BGE;

Lab6::Lab6(void)
{
	elapsed = 10000;
	turnRate = glm::half_pi<float>(); // Turn half_pi radians per second
}


Lab6::~Lab6(void)
{
}

bool Lab6::Initialise()
{
	std::shared_ptr<GameComponent> ground = make_shared<Ground>();
	Attach(ground);	

	ship1 = make_shared<GameComponent>(true);
	ship1->Attach(Content::LoadModel("Gizmo", glm::rotate(glm::mat4(1), 180.0f, glm::vec3(0,1,0))));
	ship1->transform->position = glm::vec3(-10, 2, -10);
	ship1->Attach(make_shared<VectorDrawer>());
	Attach(ship1);

	ship2 = make_shared<GameComponent>(true);
	ship2->Attach(Content::LoadModel("ferdelance", glm::rotate(glm::mat4(1), 180.0f, glm::vec3(0,1,0))));
	ship2->Attach(make_shared<VectorDrawer>());
	ship2->transform->diffuse= glm::vec3(1.0f,0.0f,0.0f);
	ship2->transform->specular = glm::vec3(1.2f, 1.2f, 1.2f);

	ship2->transform->position = glm::vec3(10, 2, -10);
	Attach(ship2);

	slerping = false;
	t = 0.0f;

	Game::Initialise();

	camera->transform->position = glm::vec3(0, 4, 20);
	return true;
}

void Lab6::Update()
{	
	// Movement of ship2
	if (keyState[SDL_SCANCODE_UP])
	{
		ship2->transform->Walk(speed * Time::deltaTime);
	}
	if (keyState[SDL_SCANCODE_DOWN])
	{
		ship2->transform->Walk(-speed * Time::deltaTime);
	}
	if (keyState[SDL_SCANCODE_LEFT])
	{
		ship2->transform->Yaw(Time::deltaTime * speed * speed);
	}
	if (keyState[SDL_SCANCODE_RIGHT])
	{
		ship2->transform->Yaw(-Time::deltaTime * speed * speed);
	}

	if (keyState[SDL_SCANCODE_O])
	{
		ship2->transform->Fly(Time::deltaTime * speed);
	}

	if (keyState[SDL_SCANCODE_L])
	{
		ship2->transform->Fly(-Time::deltaTime * speed);
	}

	if (keyState[SDL_SCANCODE_SPACE] && ! slerping)
	{
		slerping = true;
		fromQuaternion = ship1->transform->orientation;

		glm::vec3 toShip2 = ship2->transform->position - ship1->transform->position;
		toShip2 = glm::normalize(toShip2);
		glm::vec3 axis = glm::cross(Transform::basisLook, toShip2);
		axis = glm::normalize(axis);
		float theta = glm::acos(glm::dot(toShip2, Transform::basisLook));
		toQuaternion = glm::angleAxis(glm::degrees(theta), axis);

		// Calculate the angle of rotation
		toRotate = glm::acos(glm::dot(toShip2, ship1->transform->look));
	}

	if (slerping)
	{
		ship1->transform->orientation = glm::mix(fromQuaternion, toQuaternion, t);
		float time = toRotate / turnRate;
		t += 1.0f * (Time::deltaTime / time);
		if (t > 1.0f)
		{
			t = 0.0f;
			slerping = false;
		}
	}


	// Put code for ship1 here!!!
	
	// part 1	
	//glm::vec3 toShip2 = ship2->transform->position - ship1->transform->position;
	//toShip2 = glm::normalize(toShip2);
	//glm::vec3 axis = glm::cross(Transform::basisLook, toShip2);
	//axis = glm::normalize(axis);
	//float theta = glm::acos(glm::dot(toShip2, Transform::basisLook));
	//ship1->transform->orientation = glm::angleAxis(glm::degrees(theta), axis);

	Game::Update();

}
