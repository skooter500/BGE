#include <fstream>
#include <iostream>

#include "ScriptManager.h"
#include "Lua.h"

ScriptManager::ScriptManager()
{
	L = luaL_newstate();
	luaL_openlibs(L);
	Lua::RegisterMembers(L);
}

ScriptManager::~ScriptManager()
{
}

void ScriptManager::AddFunctionCode(const std::string& code, int function)
{
	switch(function)
	{
	case FUNC_TYPE::INIT:
		initCode.push_back(code);
		break;
	case FUNC_TYPE::UPDATE:
		updateCode.push_back(code);
		break;
	case FUNC_TYPE::PREDRAW:
		predrawCode.push_back(code);
		break;
	case FUNC_TYPE::DRAW:
		drawCode.push_back(code);
		break;
	case FUNC_TYPE::POSTDRAW:
		postdrawCode.push_back(code);
		break;
	case FUNC_TYPE::CLEANUP:
		cleanupCode.push_back(code);
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
	case FUNC_TYPE::INIT:
		initCode[index] = code;
		break;
	case FUNC_TYPE::UPDATE:
		updateCode[index] = code;
		break;
	case FUNC_TYPE::PREDRAW:
		predrawCode[index] = code;
		break;
	case FUNC_TYPE::DRAW:
		drawCode[index] = code;
		break;
	case FUNC_TYPE::POSTDRAW:
		postdrawCode[index] = code;
		break;
	case FUNC_TYPE::CLEANUP:
		cleanupCode[index] = code;
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
	case FUNC_TYPE::INIT:
		return initCode;
		break;
	case FUNC_TYPE::UPDATE:
		return updateCode;
		break;
	case FUNC_TYPE::PREDRAW:
		return predrawCode;
		break;
	case FUNC_TYPE::DRAW:
		return drawCode;
		break;
	case FUNC_TYPE::POSTDRAW:
		return postdrawCode;
		break;
	case FUNC_TYPE::CLEANUP:
		return cleanupCode;
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