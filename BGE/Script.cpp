#include <fstream>
#include <iostream>

#include "Script.h"
#include "Utils.h"

std::string UPDATE_STR = "update(delta)";
std::string DRAW_STR = "draw()";

std::string FINAL_UPDATE_STR = "function final_update(delta)";
std::string FINAL_DRAW_STR = "function final_draw()";

std::string LUA_UPDATE = "final_update";
std::string LUA_DRAW = "final_draw";

std::string addParts(std::vector<std::string> s);
void generateFinalScript(std::ofstream& file, ScriptManager& scriptManager);
void generateFunctionCode(std::ofstream& file, ScriptManager& scriptManager, int function, bool includeAnyEnds);
void renameFunctionVariables(ScriptManager& scriptManager, int function, const std::vector<std::string>& old_, const std::vector<std::string>& new_);

namespace BGE
{
	Script::Script(const std::string& script, ScriptManager& scriptManager) :
		scriptManager(scriptManager),
		scriptName("Content/scripts/gen/" + scriptManager.getScriptName())
	{
		finalScript.open(scriptName);

		loadScript(script, scriptManager);

		if(luaL_dofile(scriptManager.getL(), scriptName.c_str()))
		{
			const char* err = lua_tostring(scriptManager.getL(), -1);
			printf("%s\n", err);
		}
	}

	Script::~Script()
	{
		remove(scriptName.c_str());
	}

	void Script::Update()
	{
		lua_getglobal(scriptManager.getL(), LUA_UPDATE.c_str());

		if(lua_isfunction(scriptManager.getL(), lua_gettop(scriptManager.getL())))
		{
			lua_pushnumber(scriptManager.getL(), Time::deltaTime);
			lua_call(scriptManager.getL(), 1, 0);
		}

		Transform t = (Transform)luabridge::getGlobal(scriptManager.getL(), "transform");
		
		lua_getglobal(scriptManager.getL(), "ttt");
		float f = lua_tonumber(scriptManager.getL(), -1);
		
		if(transform->position != t.position ||
			transform->orientation != t.orientation ||
			transform->scale != t.scale)
		{
			scriptManager.setGlobal(*transform, "transform");
		}
		
		transform->position = t.position;
		transform->orientation = t.orientation;
		transform->scale = t.scale;
	}

	void Script::Draw()
	{
		GameComponent::Draw();
	}

	void Script::loadScript(const std::string& fileName, ScriptManager& scriptManager)
	{
		std::ifstream fileIn;
		std::vector<std::string> old_;
		std::vector<std::string> new_;

		fileIn.open(("Content/scripts/" + fileName).c_str());

		std::string line;
		int seed = rand();
		std::string seedPrefix = "b_" + std::to_string(seed);

		if(fileIn.is_open())
		{
			while(fileIn.good())
			{
				getline(fileIn, line);

				std::vector<std::string> parts = split(line, ' ');

				if(parts[0] == "local")
				{
					old_.push_back(parts[1]);
					parts[1] = seedPrefix + "_" + parts[1];
					new_.push_back(parts[1]);
					parts.erase(parts.begin());

					scriptManager.addLocalCode(addParts(parts));
				}
				else if(parts[0] == "function")
				{
					if(parts[1] == UPDATE_STR.c_str())
					{
						scriptManager.generateFunctionBody(fileIn, FUNC_TYPE::UPDATE);
					}
					else if(parts[1] == DRAW_STR.c_str())
					{
						scriptManager.generateFunctionBody(fileIn, FUNC_TYPE::RENDER);
					}
					else
					{
						parts.erase(parts.begin());
						std::string line_ = addParts(parts);
						int paramsStart = line_.find("(");
						std::string params = line_.substr(paramsStart, line_.length() - paramsStart);
						line_ = line_.substr(0, paramsStart);
						old_.push_back(line_);
						line_ = seedPrefix + "_" + line_;
						new_.push_back(line_);

						scriptManager.generateFunctionBody(fileIn, FUNC_TYPE::OTHER, "function " + line_ + params);
					}
				}

				for(int i = 0; i < old_.size(); i++)
				{
				//	old_[i] += seedPrefix;
				}

				renameFunctionVariables(scriptManager, FUNC_TYPE::OTHER, old_, new_);
				renameFunctionVariables(scriptManager, FUNC_TYPE::UPDATE, old_, new_);
				renameFunctionVariables(scriptManager, FUNC_TYPE::RENDER, old_, new_);
			}

			generateFinalScript(finalScript, scriptManager);
		}
		else
		{
			std::cout << "Error with script " << fileName << std::endl;
		}

		old_.clear();
		new_.clear();

		fileIn.close();
		finalScript.close();
	}
};

std::string addParts(std::vector<std::string> s)
{
	std::string line;
	for(int i = 0; i < s.size(); i++)
	{
		line += s[i] + " ";
	}

	return line;
}

void generateFinalScript(std::ofstream& file, ScriptManager& scriptManager)
{
	for(int i = 0; i < scriptManager.getLocalCode().size(); i++)
	{
		file << scriptManager.getLocalCode()[i] << "\n";
	}

	generateFunctionCode(file, scriptManager, FUNC_TYPE::OTHER, true);

	file << FINAL_UPDATE_STR << "\n";
	generateFunctionCode(file, scriptManager, FUNC_TYPE::UPDATE, false);

	file << FINAL_DRAW_STR << "\n";
	generateFunctionCode(file, scriptManager, FUNC_TYPE::RENDER, false);
}

void renameFunctionVariables(ScriptManager& scriptManager, int function, const std::vector<std::string>& old_, const std::vector<std::string>& new_)
{
	for(int i = 0; i < scriptManager.getFunctionCode(function).size(); i++)
	{
		for(int j = 0; j < old_.size(); j++)
		{
			std::string s = scriptManager.getFunctionCode(function)[i];
			BGE::findAndReplace(s, old_[j], new_[j]);
			scriptManager.setFunctionCode(s, i, function);
		}
	}
}

void generateFunctionCode(std::ofstream& file, ScriptManager& scriptManager, int function, bool includeAnyEnds)
{
	for(int i = 0; i < scriptManager.getFunctionCode(function).size(); i++)
	{
		if(scriptManager.getFunctionCode(function)[i] != "end")
		{
			file << scriptManager.getFunctionCode(function)[i] << "\n";
		}
		else
		{
			if(includeAnyEnds)
			{
				file << scriptManager.getFunctionCode(function)[i] << "\n";
			}
		}
	}

	if(!includeAnyEnds)
	{
		file << "end\n";
	}
}