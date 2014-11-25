#include <fstream>
#include <iostream>

#include "ScriptManager.h"
#include "Lua.h"

ScriptManager::ScriptManager()
{
	L = luaL_newstate();
	luaL_openlibs(L);
	Lua::registerMembers(L);
}

ScriptManager::~ScriptManager()
{
}

void ScriptManager::AddFunctionCode(const std::string& code, int function)
{
	switch(function)
	{
	case FUNC_TYPE::UPDATE:
		updateCode.push_back(code);
		break;
	case FUNC_TYPE::RENDER:
		renderCode.push_back(code);
		break;
	case FUNC_TYPE::OTHER:
		otherCode.push_back(code);
		break;
	}
}

void ScriptManager::AddLocalCode(const std::string& code)
{
	localCode.push_back(code);
}

void ScriptManager::GenerateFunctionBody(std::ifstream& file, int function, const std::string& functionDeclartion)
{
	std::string s;

	AddFunctionCode(functionDeclartion, function);

	while(s != "end")
	{
		getline(file, s);
		AddFunctionCode(s, function);
	}
}

void ScriptManager::GenerateScriptName(void* object)
{
	std::ostringstream address;
	address << (void const *)object << ".lua";
	scriptName = address.str();
}

void ScriptManager::SetFunctionCode(const std::string& code, int index, int function)
{
	switch(function)
	{
	case FUNC_TYPE::UPDATE:
		updateCode[index] = code;
		break;
	case FUNC_TYPE::RENDER:
		renderCode[index] = code;
		break;
	case FUNC_TYPE::OTHER:
		otherCode[index] = code;
		break;
	}
}

void ScriptManager::SetLocalCode(const std::string& code, int index)
{
	localCode[index] = code;
}

std::vector<std::string> ScriptManager::GetFunctionCode(int function) const
{
	switch(function)
	{
	case FUNC_TYPE::UPDATE:
		return updateCode;
		break;
	case FUNC_TYPE::RENDER:
		return renderCode;
		break;
	case FUNC_TYPE::OTHER:
		return otherCode;
		break;
	}
}

std::vector<std::string> ScriptManager::getLocalCode() const
{
	return localCode;
}