#include "GravityGame.h"
#include "GravityController.h"


using namespace BGE;

GravityGame::GravityGame(void)
{
	
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