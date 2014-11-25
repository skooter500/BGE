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
		std::string scriptName;
		std::ofstream finalScript;

		Script(const std::string& script, ScriptManager& scriptManager);
		~Script();

		void LoadScript(const std::string& script, ScriptManager& scriptManager);

		template<class T>
		void addParameter(T t, const std::string& name)
		{
			scriptManager.setGlobal(t, name);
		}

		virtual void Update();
		virtual void Draw();
	};
};