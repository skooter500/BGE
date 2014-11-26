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
	shared_ptr<Box> boxObj = make_shared<Box>(10, 10, 10);
	shared_ptr<ScriptManager> scriptMngr = make_shared<ScriptManager>();
	scriptMngr->AddScript("roll.lua");
	boxObj->Attach(scriptMngr);
	scriptMngr->AddScript("template.lua");
	boxObj->transform->position.x -= 50;
	Attach(boxObj);

	shared_ptr<Box> boxObj2 = make_shared<Box>(10, 10, 10);
	shared_ptr<ScriptManager> scriptMngr2 = make_shared<ScriptManager>();
	boxObj2->Attach(scriptMngr2);
	scriptMngr2->AddScript("roll.lua");
	scriptMngr2->AddScript("pulse.lua");
	boxObj2->transform->position.x += 20;

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