#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Lua.h"
#include "Game.h"
#include "GameComponent.h"

enum FUNC_TYPE
{
	INIT,
	UPDATE,
	PREDRAW,
	DRAW,
	POSTDRAW,
	CLEANUP,
	OTHER
};

namespace BGE
{
	class ScriptManager : public GameComponent
	{
	private:
		void LoadScript(const std::string& script);
	public:
		lua_State* L;
		std::string scriptName;
		std::ofstream finalScript;

		std::vector<std::string> otherCode;
		std::vector<std::string> localCode;
		std::vector<std::string> initCode;
		std::vector<std::string> inputCode;
		std::vector<std::string> updateCode;
		std::vector<std::string> predrawCode;
		std::vector<std::string> drawCode;
		std::vector<std::string> postdrawCode;
		std::vector<std::string> cleanupCode;

		ScriptManager(std::vector<std::string> scripts = {});
		~ScriptManager();

		virtual bool Initialise();
		virtual void Update();
		virtual void PreDraw();
		virtual void Draw();
		virtual void PostDraw();
		virtual void Cleanup();

		void AddScript(const std::string& script);

		void AddFunctionCode(const std::string& code, int function);
		void AddLocalCode(const std::string& code);

		void GenerateFunctionBody(std::ifstream& file, int function, const std::string& functionDeclartion = "");

		template<class T>
		void SetGlobal(T t, const std::string& name)
		{
			luabridge::setGlobal(L, t, name.c_str());
		}

		void GenerateScriptName(void* object);

		void SetFunctionCode(const std::string& code, int index, int function);
		void SetLocalCode(const std::string& code, int index);

		std::vector<std::string> GetFunctionCode(int function) const;
		std::vector<std::string> GetLocalCode() const;
	};
}