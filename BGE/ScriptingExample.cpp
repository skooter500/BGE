#include "ScriptingExample.h"

#include "Box.h"
#include "Capsule.h"
#include "Sphere.h"
#include "ScriptManager.h"

using namespace BGE;

ScriptingExample::ScriptingExample(void)
{
}

ScriptingExample::~ScriptingExample(void)
{
}

bool ScriptingExample::Initialise()
{
	shared_ptr<ScriptManager> scriptMngr = make_shared<ScriptManager>();
	scriptMngr->AddScript("roll.lua");
	scriptMngr->AddScript("template.lua");

	shared_ptr<Box> boxObj = make_shared<Box>(10, 10, 10);
	boxObj->transform->position.x -= 50;

	boxObj->Attach(scriptMngr);
	Attach(boxObj);
	
	shared_ptr<ScriptManager> scriptMngr2 = make_shared<ScriptManager>();
	scriptMngr2->AddScript("roll.lua");
	scriptMngr2->AddScript("pulse.lua");

	//scriptMngr2->AddScript("test.lua");
	//scriptMngr2->AddScript("test2.lua");

	shared_ptr<Box> boxObj2 = make_shared<Box>(10, 10, 10);
	boxObj2->transform->position.x += 20;

	boxObj2->Attach(scriptMngr2);
	Attach(boxObj2);

	//scriptMngr->SetGlobal("pitch", "action");
	//scriptMngr2->SetGlobal("yaw", "action");

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