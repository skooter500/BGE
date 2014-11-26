#include <fstream>
#include <iostream>

#include "Script.h"
#include "Utils.h"

std::string INIT_STR = "initialise()";
std::string UPDATE_STR = "update(delta)";
std::string PREDRAW_STR = "predraw()";
std::string DRAW_STR = "draw()";
std::string POSTDRAW_STR = "postdraw()";
std::string CLEANUP_STR = "cleanup()";

std::string FINAL_INIT_STR = "function final_initialise()";
std::string FINAL_UPDATE_STR = "function final_update(delta)";
std::string FINAL_PREDRAW_STR = "function final_predraw()";
std::string FINAL_DRAW_STR = "function final_draw()";
std::string FINAL_POSTDRAW_STR = "function final_postdraw()";
std::string FINAL_CLEANUP_STR = "function final_cleanup()";

std::string LUA_INIT = "final_initialise";
std::string LUA_UPDATE = "final_update";
std::string LUA_PREDRAW = "final_predraw";
std::string LUA_DRAW = "final_draw";
std::string LUA_POSTDRAW = "final_postdraw";
std::string LUA_CLEANUP = "final_cleanup";

std::string AddParts(std::vector<std::string> s);
void GenerateFinalScript(std::ofstream& file, ScriptManager& scriptManager);
void GenerateFunctionCode(std::ofstream& file, ScriptManager& scriptManager, int function, bool includeAnyEnds);
void RenameFunctionVariables(ScriptManager& scriptManager, int function, const std::vector<std::string>& old_, const std::vector<std::string>& new_);

namespace BGE
{
	Script::Script(const std::string& script, GameComponent* component) :
		scriptManager(*component->scriptManager),
		scriptName("Content/scripts/gen/" + scriptManager.scriptName)
	{
		finalScript.open(scriptName);

		LoadScript(script, scriptManager);

		if(luaL_dofile(scriptManager.L, scriptName.c_str()))
		{
			const char* err = lua_tostring(scriptManager.L, -1);
			printf("%s\n", err);
		}
	}

	Script::~Script()
	{
	}

	bool Script::Initialise()
	{
		lua_getglobal(scriptManager.L, LUA_INIT.c_str());

		if(lua_isfunction(scriptManager.L, lua_gettop(scriptManager.L)))
		{
			lua_call(scriptManager.L, 0, 0);
		}

		return GameComponent::Initialise();
	}

	void Script::Update()
	{
		lua_getglobal(scriptManager.L, LUA_UPDATE.c_str());

		if(lua_isfunction(scriptManager.L, lua_gettop(scriptManager.L)))
		{
			lua_pushnumber(scriptManager.L, Time::deltaTime);
			lua_call(scriptManager.L, 1, 0);
		}

		Transform t = (Transform)luabridge::getGlobal(scriptManager.L, "transform");
		
		if(transform->position != t.position ||
			transform->orientation != t.orientation ||
			transform->scale != t.scale)
		{
			scriptManager.SetGlobal(t, "transform");
		}
		
		transform->position = t.position;
		transform->orientation = t.orientation;
		transform->scale = t.scale;
	}

	void Script::PreDraw()
	{
		lua_getglobal(scriptManager.L, LUA_PREDRAW.c_str());

		if(lua_isfunction(scriptManager.L, lua_gettop(scriptManager.L)))
		{
			lua_call(scriptManager.L, 0, 0);
		}

		GameComponent::PreDraw();
	}

	void Script::Draw()
	{
		lua_getglobal(scriptManager.L, LUA_DRAW.c_str());

		if(lua_isfunction(scriptManager.L, lua_gettop(scriptManager.L)))
		{
			lua_call(scriptManager.L, 0, 0);
		}

		GameComponent::Draw();
	}

	void Script::PostDraw()
	{
		lua_getglobal(scriptManager.L, LUA_POSTDRAW.c_str());

		if(lua_isfunction(scriptManager.L, lua_gettop(scriptManager.L)))
		{
			lua_call(scriptManager.L, 0, 0);
		}

		GameComponent::PostDraw();
	}

	void Script::Cleanup()
	{
		lua_getglobal(scriptManager.L, LUA_CLEANUP.c_str());

		if(lua_isfunction(scriptManager.L, lua_gettop(scriptManager.L)))
		{
			lua_call(scriptManager.L, 0, 0);
		}

		remove(scriptName.c_str());
		GameComponent::Cleanup();
	}

	void Script::LoadScript(const std::string& fileName, ScriptManager& scriptManager)
	{
		std::ifstream fileIn;
		std::vector<std::string> old_;
		std::vector<std::string> new_;

		fileIn.open(("Content/scripts/" + fileName).c_str());

		std::string line;
		int seed = rand();
		std::string seedPrefix = "b" + std::to_string(seed);

		if(fileIn.is_open())
		{
			while(fileIn.good())
			{
				getline(fileIn, line);

				std::vector<std::string> parts = Split(line, ' ');

				if(parts[0] == "local")
				{
					old_.push_back(parts[1]);
					parts[1] = seedPrefix + parts[1];
					new_.push_back(parts[1]);
					parts.erase(parts.begin());

					scriptManager.AddLocalCode(AddParts(parts));
				}
				else if(parts[0] == "function")
				{
					if(parts[1] == INIT_STR.c_str())
					{
						scriptManager.GenerateFunctionBody(fileIn, FUNC_TYPE::INIT);
					}
					else if(parts[1] == UPDATE_STR.c_str())
					{
						scriptManager.GenerateFunctionBody(fileIn, FUNC_TYPE::UPDATE);
					}
					else if(parts[1] == PREDRAW_STR.c_str())
					{
						scriptManager.GenerateFunctionBody(fileIn, FUNC_TYPE::PREDRAW);
					}
					else if(parts[1] == DRAW_STR.c_str())
					{
						scriptManager.GenerateFunctionBody(fileIn, FUNC_TYPE::DRAW);
					}
					else if(parts[1] == POSTDRAW_STR.c_str())
					{
						scriptManager.GenerateFunctionBody(fileIn, FUNC_TYPE::POSTDRAW);
					}
					else if(parts[1] == CLEANUP_STR.c_str())
					{
						scriptManager.GenerateFunctionBody(fileIn, FUNC_TYPE::CLEANUP);
					}
					else
					{
						parts.erase(parts.begin());
						std::string line_ = AddParts(parts);
						int paramsStart = line_.find("(");
						std::string params = line_.substr(paramsStart, line_.length() - paramsStart);
						line_ = line_.substr(0, paramsStart);
						old_.push_back(line_);
						line_ = seedPrefix + line_;
						new_.push_back(line_);

						scriptManager.GenerateFunctionBody(fileIn, FUNC_TYPE::OTHER, "function " + line_ + params);
					}
				}

				RenameFunctionVariables(scriptManager, FUNC_TYPE::OTHER, old_, new_);
				RenameFunctionVariables(scriptManager, FUNC_TYPE::INIT, old_, new_);
				RenameFunctionVariables(scriptManager, FUNC_TYPE::UPDATE, old_, new_);
				RenameFunctionVariables(scriptManager, FUNC_TYPE::PREDRAW, old_, new_);
				RenameFunctionVariables(scriptManager, FUNC_TYPE::DRAW, old_, new_);
				RenameFunctionVariables(scriptManager, FUNC_TYPE::POSTDRAW, old_, new_);
				RenameFunctionVariables(scriptManager, FUNC_TYPE::CLEANUP, old_, new_);
			}

			GenerateFinalScript(finalScript, scriptManager);
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

std::string AddParts(std::vector<std::string> s)
{
	std::string line;
	for(int i = 0; i < s.size(); i++)
	{
		line += s[i] + " ";
	}

	return line;
}

void GenerateFinalScript(std::ofstream& file, ScriptManager& scriptManager)
{
	for(int i = 0; i < scriptManager.getLocalCode().size(); i++)
	{
		file << scriptManager.getLocalCode()[i] << "\n";
	}

	GenerateFunctionCode(file, scriptManager, FUNC_TYPE::OTHER, true);

	file << FINAL_INIT_STR << "\n";
	GenerateFunctionCode(file, scriptManager, FUNC_TYPE::INIT, false);

	file << FINAL_UPDATE_STR << "\n";
	GenerateFunctionCode(file, scriptManager, FUNC_TYPE::UPDATE, false);

	file << FINAL_PREDRAW_STR << "\n";
	GenerateFunctionCode(file, scriptManager, FUNC_TYPE::PREDRAW, false);

	file << FINAL_DRAW_STR << "\n";
	GenerateFunctionCode(file, scriptManager, FUNC_TYPE::DRAW, false);

	file << FINAL_POSTDRAW_STR << "\n";
	GenerateFunctionCode(file, scriptManager, FUNC_TYPE::POSTDRAW, false);

	file << FINAL_CLEANUP_STR << "\n";
	GenerateFunctionCode(file, scriptManager, FUNC_TYPE::CLEANUP, false);
}

void RenameFunctionVariables(ScriptManager& scriptManager, int function, const std::vector<std::string>& old_, const std::vector<std::string>& new_)
{
	for(int i = 0; i < scriptManager.GetFunctionCode(function).size(); i++)
	{
		for(int j = 0; j < old_.size(); j++)
		{
			std::string s = scriptManager.GetFunctionCode(function)[i];
			BGE::FindAndReplace(s, old_[j], new_[j], { '.', ':', '_' });
			scriptManager.SetFunctionCode(s, i, function);
		}
	}
}

void GenerateFunctionCode(std::ofstream& file, ScriptManager& scriptManager, int function, bool includeAnyEnds)
{
	for(int i = 0; i < scriptManager.GetFunctionCode(function).size(); i++)
	{
		if(scriptManager.GetFunctionCode(function)[i] != "end")
		{
			file << scriptManager.GetFunctionCode(function)[i] << "\n";
		}
		else
		{
			if(includeAnyEnds)
			{
				file << scriptManager.GetFunctionCode(function)[i] << "\n";
			}
		}
	}

	if(!includeAnyEnds)
	{
		file << "end\n";
	}
}