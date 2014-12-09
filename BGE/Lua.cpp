#include "Lua.h"
#include "Transform.h"
#include "Game.h"
#include "LuaWrapper.h"

void RegisterKeys(lua_State* l);
bool KeyStateWrapper(int key);

void Lua::RegisterMembers(lua_State* l)
{
	char* nsBGE = "bge";
	char* nsGLM = "glm";

	luabridge::getGlobalNamespace(l)

		.addFunction("sin", &sinf)
		.addFunction("cos", &cosf)
		.addFunction("tan", &tanf)

		.beginNamespace(nsGLM)

		.beginClass<glm::vec3>("vec3")
		.addConstructor<void(*)(const float&, const float&, const float&)>()
		.addData("x", &glm::vec3::x)
		.addData("y", &glm::vec3::y)
		.addData("z", &glm::vec3::z)
		.endClass()

		.beginClass<glm::quat>("quat")
		.addConstructor<void(*)(const float&, const float&, const float&, const float&)>()
		.addData("w", &glm::quat::w)
		.addData("x", &glm::quat::x)
		.addData("y", &glm::quat::y)
		.addData("z", &glm::quat::z)
		.endClass()
		.endNamespace();

	luabridge::getGlobalNamespace(l)
		.beginNamespace(nsBGE)

		.beginClass<BGE::Transform>("Transform")
		.addConstructor<void(*)()>()
		.addData("position", &BGE::Transform::position)
		.addData("orientation", &BGE::Transform::orientation)
		.addData("scale", &BGE::Transform::scale)
		.addFunction("Yaw", &BGE::Transform::Yaw)
		.addFunction("Pitch", &BGE::Transform::Pitch)
		.addFunction("Roll", &BGE::Transform::Roll)
		.addFunction("Walk", &BGE::Transform::Walk)
		.addFunction("Strafe", &BGE::Transform::Strafe)
		.endClass()
		.endNamespace();

	RegisterKeys(l);
}

void RegisterKeys(lua_State* l)
{
	luabridge::getGlobalNamespace(l)
		.beginNamespace("io")
		.addFunction("keyState", &KeyStateWrapper)
		.endNamespace();

	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_UNKNOWN), "SDL_SCANCODE_UNKNOWN");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_A), "SDL_SCANCODE_A");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_B), "SDL_SCANCODE_B");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_C), "SDL_SCANCODE_C");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_D), "SDL_SCANCODE_D");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_E), "SDL_SCANCODE_E");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_F), "SDL_SCANCODE_F");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_G), "SDL_SCANCODE_G");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_H), "SDL_SCANCODE_H");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_I), "SDL_SCANCODE_I");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_J), "SDL_SCANCODE_J");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_K), "SDL_SCANCODE_K");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_L), "SDL_SCANCODE_L");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_M), "SDL_SCANCODE_M");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_N), "SDL_SCANCODE_N");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_O), "SDL_SCANCODE_O");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_P), "SDL_SCANCODE_P");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_Q), "SDL_SCANCODE_Q");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_R), "SDL_SCANCODE_R");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_S), "SDL_SCANCODE_S");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_T), "SDL_SCANCODE_T");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_U), "SDL_SCANCODE_U");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_V), "SDL_SCANCODE_V");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_W), "SDL_SCANCODE_W");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_X), "SDL_SCANCODE_X");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_Y), "SDL_SCANCODE_Y");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_Z), "SDL_SCANCODE_Z");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_1), "SDL_SCANCODE_1");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_2), "SDL_SCANCODE_2");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_3), "SDL_SCANCODE_3");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_4), "SDL_SCANCODE_4");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_5), "SDL_SCANCODE_5");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_6), "SDL_SCANCODE_6");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_7), "SDL_SCANCODE_7");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_8), "SDL_SCANCODE_8");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_9), "SDL_SCANCODE_9");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_0), "SDL_SCANCODE_0");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_RETURN), "SDL_SCANCODE_RETURN");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_ESCAPE), "SDL_SCANCODE_ESCAPE");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_BACKSPACE), "SDL_SCANCODE_BACKSPACE");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_TAB), "SDL_SCANCODE_TAB");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_SPACE), "SDL_SCANCODE_SPACE");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_MINUS), "SDL_SCANCODE_MINUS");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_EQUALS), "SDL_SCANCODE_EQUALS");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_LEFTBRACKET), "SDL_SCANCODE_LEFTBRACKET");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_RIGHTBRACKET), "SDL_SCANCODE_RIGHTBRACKET");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_BACKSLASH), "SDL_SCANCODE_BACKSLASH");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_NONUSHASH), "SDL_SCANCODE_NONUSHASH");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_SEMICOLON), "SDL_SCANCODE_SEMICOLON");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_APOSTROPHE), "SDL_SCANCODE_APOSTROPHE");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_GRAVE), "SDL_SCANCODE_GRAVE");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_COMMA), "SDL_SCANCODE_COMMA");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_PERIOD), "SDL_SCANCODE_PERIOD");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_SLASH), "SDL_SCANCODE_SLASH");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_CAPSLOCK), "SDL_SCANCODE_CAPSLOCK");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_F1), "SDL_SCANCODE_F1");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_F2), "SDL_SCANCODE_F2");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_F3), "SDL_SCANCODE_F3");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_F4), "SDL_SCANCODE_F4");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_F5), "SDL_SCANCODE_F5");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_F6), "SDL_SCANCODE_F6");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_F7), "SDL_SCANCODE_F7");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_F8), "SDL_SCANCODE_F8");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_F9), "SDL_SCANCODE_F9");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_F10), "SDL_SCANCODE_F10");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_F11), "SDL_SCANCODE_F11");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_F12), "SDL_SCANCODE_F12");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_PRINTSCREEN), "SDL_SCANCODE_PRINTSCREEN");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_SCROLLLOCK), "SDL_SCANCODE_SCROLLLOCK");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_PAUSE), "SDL_SCANCODE_PAUSE");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_INSERT), "SDL_SCANCODE_INSERT");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_HOME), "SDL_SCANCODE_HOME");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_PAGEUP), "SDL_SCANCODE_PAGEUP");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_DELETE), "SDL_SCANCODE_DELETE");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_END), "SDL_SCANCODE_END");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_PAGEDOWN), "SDL_SCANCODE_PAGEDOWN");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_RIGHT), "SDL_SCANCODE_RIGHT");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_LEFT), "SDL_SCANCODE_LEFT");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_DOWN), "SDL_SCANCODE_DOWN");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_UP), "SDL_SCANCODE_UP");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_NUMLOCKCLEAR), "SDL_SCANCODE_NUMLOCKCLEAR");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_DIVIDE), "SDL_SCANCODE_KP_DIVIDE");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_MULTIPLY), "SDL_SCANCODE_KP_MULTIPLY");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_MINUS), "SDL_SCANCODE_KP_MINUS");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_PLUS), "SDL_SCANCODE_KP_PLUS");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_ENTER), "SDL_SCANCODE_KP_ENTER");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_1), "SDL_SCANCODE_KP_1");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_2), "SDL_SCANCODE_KP_2");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_3), "SDL_SCANCODE_KP_3");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_4), "SDL_SCANCODE_KP_4");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_5), "SDL_SCANCODE_KP_5");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_6), "SDL_SCANCODE_KP_6");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_7), "SDL_SCANCODE_KP_7");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_8), "SDL_SCANCODE_KP_8");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_9), "SDL_SCANCODE_KP_9");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_0), "SDL_SCANCODE_KP_0");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_PERIOD), "SDL_SCANCODE_KP_PERIOD");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_NONUSBACKSLASH), "SDL_SCANCODE_NONUSBACKSLASH");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_APPLICATION), "SDL_SCANCODE_APPLICATION");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_POWER), "SDL_SCANCODE_POWER");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_EQUALS), "SDL_SCANCODE_KP_EQUALS");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_F13), "SDL_SCANCODE_F13");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_F14), "SDL_SCANCODE_F14");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_F15), "SDL_SCANCODE_F15");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_F16), "SDL_SCANCODE_F16");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_F17), "SDL_SCANCODE_F17");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_F18), "SDL_SCANCODE_F18");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_F19), "SDL_SCANCODE_F19");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_F20), "SDL_SCANCODE_F20");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_F21), "SDL_SCANCODE_F21");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_F22), "SDL_SCANCODE_F22");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_F23), "SDL_SCANCODE_F23");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_F24), "SDL_SCANCODE_F24");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_EXECUTE), "SDL_SCANCODE_EXECUTE");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_HELP), "SDL_SCANCODE_HELP");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_MENU), "SDL_SCANCODE_MENU");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_SELECT), "SDL_SCANCODE_SELECT");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_STOP), "SDL_SCANCODE_STOP");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_AGAIN), "SDL_SCANCODE_AGAIN");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_UNDO), "SDL_SCANCODE_UNDO");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_CUT), "SDL_SCANCODE_CUT");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_COPY), "SDL_SCANCODE_COPY");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_PASTE), "SDL_SCANCODE_PASTE");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_FIND), "SDL_SCANCODE_FIND");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_MUTE), "SDL_SCANCODE_MUTE");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_VOLUMEUP), "SDL_SCANCODE_VOLUMEUP");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_VOLUMEDOWN), "SDL_SCANCODE_VOLUMEDOWN");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_COMMA), "SDL_SCANCODE_KP_COMMA");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_EQUALSAS400), "SDL_SCANCODE_KP_EQUALSAS400");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_INTERNATIONAL1), "SDL_SCANCODE_INTERNATIONAL1");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_INTERNATIONAL2), "SDL_SCANCODE_INTERNATIONAL2");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_INTERNATIONAL3), "SDL_SCANCODE_INTERNATIONAL3");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_INTERNATIONAL4), "SDL_SCANCODE_INTERNATIONAL4");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_INTERNATIONAL5), "SDL_SCANCODE_INTERNATIONAL5");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_INTERNATIONAL6), "SDL_SCANCODE_INTERNATIONAL6");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_INTERNATIONAL7), "SDL_SCANCODE_INTERNATIONAL7");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_INTERNATIONAL8), "SDL_SCANCODE_INTERNATIONAL8");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_INTERNATIONAL9), "SDL_SCANCODE_INTERNATIONAL9");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_LANG1), "SDL_SCANCODE_LANG1");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_LANG2), "SDL_SCANCODE_LANG2");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_LANG3), "SDL_SCANCODE_LANG3");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_LANG4), "SDL_SCANCODE_LANG4");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_LANG5), "SDL_SCANCODE_LANG5");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_LANG6), "SDL_SCANCODE_LANG6");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_LANG7), "SDL_SCANCODE_LANG7");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_LANG8), "SDL_SCANCODE_LANG8");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_LANG9), "SDL_SCANCODE_LANG9");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_ALTERASE), "SDL_SCANCODE_ALTERASE");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_SYSREQ), "SDL_SCANCODE_SYSREQ");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_CANCEL), "SDL_SCANCODE_CANCEL");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_CLEAR), "SDL_SCANCODE_CLEAR");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_PRIOR), "SDL_SCANCODE_PRIOR");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_RETURN2), "SDL_SCANCODE_RETURN2");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_SEPARATOR), "SDL_SCANCODE_SEPARATOR");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_OUT), "SDL_SCANCODE_OUT");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_OPER), "SDL_SCANCODE_OPER");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_CLEARAGAIN), "SDL_SCANCODE_CLEARAGAIN");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_CRSEL), "SDL_SCANCODE_CRSEL");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_EXSEL), "SDL_SCANCODE_EXSEL");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_00), "SDL_SCANCODE_KP_00");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_000), "SDL_SCANCODE_KP_000");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_THOUSANDSSEPARATOR), "SDL_SCANCODE_THOUSANDSSEPARATOR");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_DECIMALSEPARATOR), "SDL_SCANCODE_DECIMALSEPARATOR");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_CURRENCYUNIT), "SDL_SCANCODE_CURRENCYUNIT");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_CURRENCYSUBUNIT), "SDL_SCANCODE_CURRENCYSUBUNIT");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_LEFTPAREN), "SDL_SCANCODE_KP_LEFTPAREN");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_RIGHTPAREN), "SDL_SCANCODE_KP_RIGHTPAREN");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_LEFTBRACE), "SDL_SCANCODE_KP_LEFTBRACE");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_RIGHTBRACE), "SDL_SCANCODE_KP_RIGHTBRACE");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_TAB), "SDL_SCANCODE_KP_TAB");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_BACKSPACE), "SDL_SCANCODE_KP_BACKSPACE");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_A), "SDL_SCANCODE_KP_A");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_B), "SDL_SCANCODE_KP_B");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_C), "SDL_SCANCODE_KP_C");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_D), "SDL_SCANCODE_KP_D");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_E), "SDL_SCANCODE_KP_E");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_F), "SDL_SCANCODE_KP_F");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_XOR), "SDL_SCANCODE_KP_XOR");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_POWER), "SDL_SCANCODE_KP_POWER");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_PERCENT), "SDL_SCANCODE_KP_PERCENT");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_LESS), "SDL_SCANCODE_KP_LESS");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_GREATER), "SDL_SCANCODE_KP_GREATER");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_AMPERSAND), "SDL_SCANCODE_KP_AMPERSAND");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_DBLAMPERSAND), "SDL_SCANCODE_KP_DBLAMPERSAND");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_VERTICALBAR), "SDL_SCANCODE_KP_VERTICALBAR");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_DBLVERTICALBAR), "SDL_SCANCODE_KP_DBLVERTICALBAR");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_COLON), "SDL_SCANCODE_KP_COLON");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_HASH), "SDL_SCANCODE_KP_HASH");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_SPACE), "SDL_SCANCODE_KP_SPACE");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_AT), "SDL_SCANCODE_KP_AT");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_EXCLAM), "SDL_SCANCODE_KP_EXCLAM");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_MEMSTORE), "SDL_SCANCODE_KP_MEMSTORE");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_MEMRECALL), "SDL_SCANCODE_KP_MEMRECALL");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_MEMCLEAR), "SDL_SCANCODE_KP_MEMCLEAR");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_MEMADD), "SDL_SCANCODE_KP_MEMADD");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_MEMSUBTRACT), "SDL_SCANCODE_KP_MEMSUBTRACT");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_MEMMULTIPLY), "SDL_SCANCODE_KP_MEMMULTIPLY");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_MEMDIVIDE), "SDL_SCANCODE_KP_MEMDIVIDE");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_PLUSMINUS), "SDL_SCANCODE_KP_PLUSMINUS");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_CLEAR), "SDL_SCANCODE_KP_CLEAR");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_CLEARENTRY), "SDL_SCANCODE_KP_CLEARENTRY");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_BINARY), "SDL_SCANCODE_KP_BINARY");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_OCTAL), "SDL_SCANCODE_KP_OCTAL");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_DECIMAL), "SDL_SCANCODE_KP_DECIMAL");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KP_HEXADECIMAL), "SDL_SCANCODE_KP_HEXADECIMAL");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_LCTRL), "SDL_SCANCODE_LCTRL");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_LSHIFT), "SDL_SCANCODE_LSHIFT");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_LALT), "SDL_SCANCODE_LALT");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_LGUI), "SDL_SCANCODE_LGUI");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_RCTRL), "SDL_SCANCODE_RCTRL");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_RSHIFT), "SDL_SCANCODE_RSHIFT");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_RALT), "SDL_SCANCODE_RALT");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_RGUI), "SDL_SCANCODE_RGUI");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_MODE), "SDL_SCANCODE_MODE");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_AUDIONEXT), "SDL_SCANCODE_AUDIONEXT");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_AUDIOPREV), "SDL_SCANCODE_AUDIOPREV");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_AUDIOSTOP), "SDL_SCANCODE_AUDIOSTOP");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_AUDIOPLAY), "SDL_SCANCODE_AUDIOPLAY");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_AUDIOMUTE), "SDL_SCANCODE_AUDIOMUTE");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_MEDIASELECT), "SDL_SCANCODE_MEDIASELECT");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_WWW), "SDL_SCANCODE_WWW");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_MAIL), "SDL_SCANCODE_MAIL");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_CALCULATOR), "SDL_SCANCODE_CALCULATOR");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_COMPUTER), "SDL_SCANCODE_COMPUTER");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_AC_SEARCH), "SDL_SCANCODE_AC_SEARCH");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_AC_HOME), "SDL_SCANCODE_AC_HOME");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_AC_BACK), "SDL_SCANCODE_AC_BACK");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_AC_FORWARD), "SDL_SCANCODE_AC_FORWARD");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_AC_STOP), "SDL_SCANCODE_AC_STOP");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_AC_REFRESH), "SDL_SCANCODE_AC_REFRESH");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_AC_BOOKMARKS), "SDL_SCANCODE_AC_BOOKMARKS");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_BRIGHTNESSDOWN), "SDL_SCANCODE_BRIGHTNESSDOWN");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_BRIGHTNESSUP), "SDL_SCANCODE_BRIGHTNESSUP");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_DISPLAYSWITCH), "SDL_SCANCODE_DISPLAYSWITCH");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KBDILLUMTOGGLE), "SDL_SCANCODE_KBDILLUMTOGGLE");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KBDILLUMDOWN), "SDL_SCANCODE_KBDILLUMDOWN");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_KBDILLUMUP), "SDL_SCANCODE_KBDILLUMUP");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_EJECT), "SDL_SCANCODE_EJECT");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_SLEEP), "SDL_SCANCODE_SLEEP");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_APP1), "SDL_SCANCODE_APP1");
	luabridge::setGlobal(l, static_cast<int>(SDL_SCANCODE_APP2), "SDL_SCANCODE_APP2");
}

bool KeyStateWrapper(int key)
{
	return BGE::Game::Instance()->keyState[key];
}