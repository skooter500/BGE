#pragma once

#include <fstream>

#include "GameComponent.h"
#include "ScriptManager.h"

namespace BGE
{
	class Script : public GameComponent
	{
	public:
		ScriptManager& scriptManager;
		int scriptNumber;

		std::string scriptName;
		std::ofstream finalScript;

		Script(const std::string& script, GameComponent* component);
		~Script();

		void LoadScript(const std::string& script, ScriptManager& scriptManager);

		virtual bool Initialise();
		virtual void Update();
		virtual void PreDraw();
		virtual void Draw();
		virtual void PostDraw();
		virtual void Cleanup();
	};
};