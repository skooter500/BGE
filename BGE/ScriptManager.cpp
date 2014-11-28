#include "Utils.h"
#include "ScriptManager.h"

std::string SCRIPT_DIR = "Content/scripts/";
std::string SCRIPT_GEN = SCRIPT_DIR + "gen/";

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
void GenerateFinalScript(std::ofstream& file, BGE::ScriptManager& scriptManager);
void GenerateFunctionCode(std::ofstream& file, BGE::ScriptManager& scriptManager, int function, bool includeAnyEnds);
void RenameFunctionVariables(BGE::ScriptManager& scriptManager, int function, const std::vector<std::string>& old_, const std::vector<std::string>& new_);

namespace BGE
{
	ScriptManager::ScriptManager(std::vector<std::string> scripts)
	{
		L = luaL_newstate();
		luaL_openlibs(L);
		Lua::RegisterMembers(L);

		GenerateScriptName(this);

		for(int i = 0; i < scripts.size(); i++)
		{
			AddScript(scripts[i]);
		}
	}

	ScriptManager::~ScriptManager()
	{
	}

	bool ScriptManager::Initialise()
	{
		lua_getglobal(L, LUA_INIT.c_str());

		if(lua_isfunction(L, lua_gettop(L)))
		{
			lua_call(L, 0, 0);
		}

		SetGlobal(*transform, "transform");

		return GameComponent::Initialise();
	}

	void ScriptManager::Update()
	{
		lua_getglobal(L, LUA_UPDATE.c_str());

		if(lua_isfunction(L, lua_gettop(L)))
		{
			lua_pushnumber(L, Time::deltaTime);
			lua_call(L, 1, 0);
		}

		Transform t = (Transform)luabridge::getGlobal(L, "transform");
		
		if(transform->position != t.position ||
			transform->orientation != t.orientation ||
			transform->scale != t.scale)
		{
			SetGlobal(t, "transform");
		}
		
		transform->position = t.position;
		transform->orientation = t.orientation;
		transform->scale = t.scale;

		GameComponent::Update();
	}

	void ScriptManager::PreDraw()
	{
		lua_getglobal(L, LUA_PREDRAW.c_str());

		if(lua_isfunction(L, lua_gettop(L)))
		{
			lua_call(L, 0, 0);
		}

		GameComponent::PreDraw();
	}

	void ScriptManager::Draw()
	{
		lua_getglobal(L, LUA_DRAW.c_str());

		if(lua_isfunction(L, lua_gettop(L)))
		{
			lua_call(L, 0, 0);
		}

		GameComponent::Draw();
	}

	void ScriptManager::PostDraw()
	{
		lua_getglobal(L, LUA_POSTDRAW.c_str());

		if(lua_isfunction(L, lua_gettop(L)))
		{
			lua_call(L, 0, 0);
		}
	}

	void ScriptManager::Cleanup()
	{
		lua_getglobal(L, LUA_CLEANUP.c_str());

		if(lua_isfunction(L, lua_gettop(L)))
		{
			lua_call(L, 0, 0);
		}

		remove(scriptName.c_str());

		GameComponent::Cleanup();
	}

	void ScriptManager::AddScript(const std::string& script)
	{
		finalScript.open(scriptName);
		LoadScript(SCRIPT_DIR + script);
		finalScript.close();

		if(luaL_dofile(L, scriptName.c_str()))
		{
			const char* err = lua_tostring(L, -1);
			printf("%s\n", err);
		}
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
		scriptName = SCRIPT_GEN + address.str();
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

	void ScriptManager::LoadScript(const std::string& fileName)
	{
		std::ifstream fileIn;
		std::vector<std::string> old_;
		std::vector<std::string> new_;

		fileIn.open(fileName.c_str());

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

					AddLocalCode(AddParts(parts));
				}
				else if(parts[0] == "function")
				{
					if(parts[1] == INIT_STR.c_str())
					{
						GenerateFunctionBody(fileIn, FUNC_TYPE::INIT);
					}
					else if(parts[1] == UPDATE_STR.c_str())
					{
						GenerateFunctionBody(fileIn, FUNC_TYPE::UPDATE);
					}
					else if(parts[1] == PREDRAW_STR.c_str())
					{
						GenerateFunctionBody(fileIn, FUNC_TYPE::PREDRAW);
					}
					else if(parts[1] == DRAW_STR.c_str())
					{
						GenerateFunctionBody(fileIn, FUNC_TYPE::DRAW);
					}
					else if(parts[1] == POSTDRAW_STR.c_str())
					{
						GenerateFunctionBody(fileIn, FUNC_TYPE::POSTDRAW);
					}
					else if(parts[1] == CLEANUP_STR.c_str())
					{
						GenerateFunctionBody(fileIn, FUNC_TYPE::CLEANUP);
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

						GenerateFunctionBody(fileIn, FUNC_TYPE::OTHER, "function " + line_ + params);
					}
				}

				RenameFunctionVariables(*this, FUNC_TYPE::OTHER, old_, new_);
				RenameFunctionVariables(*this, FUNC_TYPE::INIT, old_, new_);
				RenameFunctionVariables(*this, FUNC_TYPE::UPDATE, old_, new_);
				RenameFunctionVariables(*this, FUNC_TYPE::PREDRAW, old_, new_);
				RenameFunctionVariables(*this, FUNC_TYPE::DRAW, old_, new_);
				RenameFunctionVariables(*this, FUNC_TYPE::POSTDRAW, old_, new_);
				RenameFunctionVariables(*this, FUNC_TYPE::CLEANUP, old_, new_);
			}

			GenerateFinalScript(finalScript, *this);
		}
		else
		{
			std::cout << "Error with script " << fileName << std::endl;
		}

		old_.clear();
		new_.clear();

		fileIn.close();
	}
}

std::string AddParts(std::vector<std::string> s)
{
	std::string line;
	for(int i = 0; i < s.size(); i++)
	{
		line += s[i] + " ";
	}

	return line;
}

void GenerateFinalScript(std::ofstream& file, BGE::ScriptManager& scriptManager)
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

void RenameFunctionVariables(BGE::ScriptManager& scriptManager, int function, const std::vector<std::string>& old_, const std::vector<std::string>& new_)
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

void GenerateFunctionCode(std::ofstream& file, BGE::ScriptManager& scriptManager, int function, bool includeAnyEnds)
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