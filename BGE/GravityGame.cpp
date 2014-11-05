#include "GravityGame.h"
#include "GravityController.h"
#include "Game.h"
#include "GameComponent.h"

using namespace BGE;

GravityGame::GravityGame(void)
{
	elapsed = 10000;
}

bool GravityGame::Initialise()
{
	box = make_shared<Box>();
	//sphere = make_shared<Sphere>();
	//sgc = make_shared<GravityController>();
	bgc = make_shared<GravityController>();

	box->Attach(bgc);
	//sphere->Attach(sgc);

	Game::Initialise();
	return true;
}

void GravityGame::Update(float timeDelta)
{
	box->Update(timeDelta);
	sphere->Update(timeDelta);
	Game::Update(timeDelta);
}