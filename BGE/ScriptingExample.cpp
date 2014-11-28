/*
	An example game demonstrating how to use lua scripts
*/


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
	shared_ptr<ScriptManager> scriptMngr = make_shared<ScriptManager>();		// Declare Scripting Manager
	scriptMngr->AddScript("roll.lua");											// Call to add scripts
	scriptMngr->AddScript("template.lua");										// Call to add scripts

	shared_ptr<Box> boxObj = make_shared<Box>(10, 10, 10);						// Make an object
	boxObj->transform->position.x -= 50;

	boxObj->Attach(scriptMngr);													// Attach the Script Manager to the object
	Attach(boxObj);																// Attach the object to the world
	
	shared_ptr<ScriptManager> scriptMngr2 = make_shared<ScriptManager>();
	scriptMngr2->AddScript("roll.lua");
	scriptMngr2->AddScript("pulse.lua");

	shared_ptr<Box> boxObj2 = make_shared<Box>(10, 10, 10);
	boxObj2->transform->position.x += 20;

	boxObj2->Attach(scriptMngr2);
	Attach(boxObj2);

	scriptMngr->AddScript("global.lua");										// Loads a script with a global variable
	scriptMngr2->AddScript("global.lua");

	scriptMngr->SetGlobal("pitch", "action");									// Assign value to the global
	scriptMngr2->SetGlobal(1234, "action");										// Assign value to the global

	//shared_ptr<ScriptManager> scriptMngr3 = make_shared<ScriptManager>();
	//scriptMngr3->AddScript("roll.lua");
	//Attach(scriptMngr3);

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