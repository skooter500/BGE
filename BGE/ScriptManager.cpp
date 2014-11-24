#include <fstream>
#include <iostream>

#include "ScriptManager.h"
#include "Lua.h"

ScriptManager::ScriptManager()
{
	m_L = luaL_newstate();
	luaL_openlibs(m_L);
	Lua::registerMembers(m_L);
}

ScriptManager::~ScriptManager()
{
}

void ScriptManager::addFunctionCode(const std::string& code, int function)
{
	switch(function)
	{
	case FUNC_TYPE::UPDATE:
		m_updateCode.push_back(code);
		break;
	case FUNC_TYPE::RENDER:
		m_renderCode.push_back(code);
		break;
	case FUNC_TYPE::OTHER:
		m_otherCode.push_back(code);
		break;
	}
}

void ScriptManager::addLocalCode(const std::string& code)
{
	m_localCode.push_back(code);
}

void ScriptManager::generateFunctionBody(std::ifstream& file, int function, const std::string& functionDeclartion)
{
	std::string s;

	addFunctionCode(functionDeclartion, function);

	while(s != "end")
	{
		getline(file, s);
		addFunctionCode(s, function);
	}
}

void ScriptManager::generateScriptName(void* object)
{
	std::ostringstream address;
	address << (void const *)object << ".lua";
	m_scriptName = address.str();
}

void ScriptManager::setFunctionCode(const std::string& code, int index, int function)
{
	switch(function)
	{
	case FUNC_TYPE::UPDATE:
		m_updateCode[index] = code;
		break;
	case FUNC_TYPE::RENDER:
		m_renderCode[index] = code;
		break;
	case FUNC_TYPE::OTHER:
		m_otherCode[index] = code;
		break;
	}
}

void ScriptManager::setLocalCode(const std::string& code, int index)
{
	m_localCode[index] = code;
}

lua_State* ScriptManager::getL()
{
	return m_L;
}

std::string ScriptManager::getScriptName() const
{
	return m_scriptName;
}

std::vector<std::string> ScriptManager::getFunctionCode(int function) const
{
	switch(function)
	{
	case FUNC_TYPE::UPDATE:
		return m_updateCode;
		break;
	case FUNC_TYPE::RENDER:
		return m_renderCode;
		break;
	case FUNC_TYPE::OTHER:
		return m_otherCode;
		break;
	}
}

std::vector<std::string> ScriptManager::getLocalCode() const
{
	return m_localCode;
}