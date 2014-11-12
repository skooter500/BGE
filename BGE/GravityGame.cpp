#include "GravityGame.h"
#include "Sphere.h"
#include "GravityController.h"
#include "Box.h"
#include "Steerable3DController.h"
#include "Content.h"


BGE::GravityGame::GravityGame()
{
}


BGE::GravityGame::~GravityGame()
{
}

bool BGE::GravityGame::Initialise()
{
	std::shared_ptr<Ground> ground = make_shared<Ground>();
	SetGround(ground);

	shared_ptr<Box> box = make_shared<Box>(10, 10, 10);
	box->Attach(make_shared<Steerable3DController>(Content::LoadModel("cobramk3")));
	box->transform->position = glm::vec3(-10, 50, 0);
	Attach(box);

	return Game::Initialise();
}

void BGE::GravityGame::Update()
{
	Game::Update();
}
