#include "EmptyGame.h"

using namespace BGE;


EmptyGame::EmptyGame()
{
}


EmptyGame::~EmptyGame()
{
}

bool BGE::EmptyGame::Initialise()
{
	dynamicsWorld->setGravity(btVector3(0, -9, 0));

	physicsFactory->CreateCameraPhysics();
	physicsFactory->CreateGroundPhysics();
	return Game::Initialise();
}

void EmptyGame::Update()
{
	Game::Update();
}
