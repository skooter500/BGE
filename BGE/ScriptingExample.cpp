#include "ScriptingExample.h"

#include "Box.h"
#include "Capsule.h"
#include "Sphere.h"
#include "Script.h"

using namespace BGE;

ScriptingExample::ScriptingExample(void)
{
}

ScriptingExample::~ScriptingExample(void)
{
}

bool ScriptingExample::Initialise()
{
	shared_ptr<Box> boxObj = make_shared<Box>(10, 10, 10);
	boxObj->transform->position.x -= 50;
	boxObj->EnableScripting();
	boxObj->Attach(make_shared<Script>("roll.lua", boxObj.get()));
	//boxObj->scriptManager->SetGlobal("yaw", "action");
	Attach(boxObj);

	shared_ptr<Box> boxObj2 = make_shared<Box>(10, 10, 10);
	boxObj2->transform->position.x += 20;
	boxObj2->EnableScripting();
	boxObj2->Attach(make_shared<Script>("pulse.lua", boxObj2.get()));
	//boxObj2->scriptManager->SetGlobal("pitch", "action");


	//boxObj2->Attach(make_shared<Script>("test.lua", boxObj2.get()));
	//boxObj2->Attach(make_shared<Script>("test2.lua", boxObj2.get()));
	Attach(boxObj2);

	return Game::Initialise();
}

void BGE::ScriptingExample::Update()
{
	Game::Update();
}

void BGE::ScriptingExample::Cleanup()
{
	Game::Cleanup();
}