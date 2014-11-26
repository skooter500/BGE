#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include <string>
#include <vector>

#include "Lua.h"

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

class ScriptManager
{
public:
	lua_State* L;
	std::string scriptName;

	std::vector<std::string> otherCode;
	std::vector<std::string> localCode;
	std::vector<std::string> initCode;
	std::vector<std::string> inputCode;
	std::vector<std::string> updateCode;
	std::vector<std::string> predrawCode;
	std::vector<std::string> drawCode;
	std::vector<std::string> postdrawCode;
	std::vector<std::string> cleanupCode;

	ScriptManager();
	~ScriptManager();

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
	std::vector<std::string> getLocalCode() const;
};

#endif