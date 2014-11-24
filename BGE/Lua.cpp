#include "Lua.h"
#include "Transform.h"

void registerKeys(lua_State* l);
void registerMouse(lua_State* l);

float toRadiansLua(float degrees);
float randomLua(float min, float max);

void Lua::registerMembers(lua_State* l)
{
	char* nsCore = "Core";
	char* nsMath = "Math";
	char* nsInput = "IO";

	luabridge::getGlobalNamespace(l)
		.beginNamespace(nsMath)
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
		.beginNamespace(nsCore)
		.beginClass<BGE::Transform>("Transform")
		.addConstructor<void(*)()>()
		.addData("position", &BGE::Transform::position)
		.addData("orientation", &BGE::Transform::orientation)
		.addData("scale", &BGE::Transform::scale)
		.addData("test", &BGE::Transform::test)
		.addFunction("Yaw", &BGE::Transform::Yaw)
		.addFunction("Pitch", &BGE::Transform::Pitch)
		.addFunction("Roll", &BGE::Transform::Roll)
		.addFunction("Walk", &BGE::Transform::Walk)
		.addFunction("Strafe", &BGE::Transform::Strafe)
		.endClass()
		.endNamespace();

	/*luabridge::getGlobalNamespace(l)
		.beginNamespace(nsCore)
		.beginClass<Transform>("Transform")
		.addConstructor<void(*)(const Vector3&, const Quaternion&, const float&)>()
		.addFunction("setPosition", &Transform::setPosition)
		.addFunction("getPosition", &Transform::getPositionLua)
		.addFunction("rotate", &Transform::rotateLua)
		.endClass()
		.endNamespace();

	luabridge::getGlobalNamespace(l)
		.beginNamespace(nsInput)
		.beginClass<Input>("Input")
		.addConstructor<void(*)(void)>()
		.addFunction("getKey", &Input::getKey)
		.addFunction("getKeyDown", &Input::getKeyDown)
		.addFunction("getKeyUp", &Input::getKeyUp)
		.addFunction("getMouse", &Input::getMouse)
		.addFunction("getMouseDown", &Input::getMouseDown)
		.addFunction("getMouseUp", &Input::getMouseUp)
		.addFunction("getMousePosition", &Input::getMousePosition)
		.endClass()
		.endNamespace();

	registerKeys(l);
	registerMouse(l);

	luabridge::getGlobalNamespace(l)
		.beginNamespace(nsMath)

		.addFunction("toRadians", &toRadiansLua)
		.addFunction("random", &randomLua)

		.beginClass<Vector2>("Vector2")
		.addConstructor<void(*)(const float&, const float&)>()
		.addFunction("length", &Vector2::length)
		.addFunction("dot", &Vector2::dot)
		.addFunction("cross", &Vector2::cross)
		.addFunction("normalised", &Vector2::normalised)
		.addFunction("inversed", &Vector2::inversed)
		.addFunction("lerp", &Vector2::lerp)
		.addFunction("getX", &Vector2::getX)
		.addFunction("getY", &Vector2::getY)
		.addFunction("setX", &Vector2::setX)
		.addFunction("setY", &Vector2::setY)
		.endClass()

		.beginClass<Vector3>("Vector3")
		.addConstructor<void(*)(const float&, const float&, const float&)>()
		.addFunction("length", &Vector3::length)
		.addFunction("squareLength", &Vector3::squareLength)
		.addFunction("dot", &Vector3::dot)
		.addFunction("cross", &Vector3::cross)
		.addFunction("normalised", &Vector3::normalised)
		.addFunction("inversed", &Vector3::inversed)
		.addFunction("reflect", &Vector3::reflect)
		.addFunction("lerp", &Vector3::lerp)
		.addFunction("absolute", &Vector3::absolute)
		.addFunction("getX", &Vector3::getX)
		.addFunction("getY", &Vector3::getY)
		.addFunction("getZ", &Vector3::getZ)
		.addFunction("setX", &Vector3::setX)
		.addFunction("setY", &Vector3::setY)
		.addFunction("setZ", &Vector3::setZ)
		.endClass()

		.beginClass<Vector4>("Vector4")
		.addConstructor<void(*)(const float&, const float&, const float&, const float&)>()
		.addFunction("length", &Vector4::length)
		.addFunction("normalised", &Vector4::normalised)
		.addFunction("getX", &Vector4::getX)
		.addFunction("getY", &Vector4::getY)
		.addFunction("getZ", &Vector4::getZ)
		.addFunction("getW", &Vector4::getW)
		.addFunction("setX", &Vector4::setX)
		.addFunction("setY", &Vector4::setY)
		.addFunction("setZ", &Vector4::setZ)
		.addFunction("setW", &Vector4::setW)
		.endClass()

		.beginClass<Quaternion>("Quaternion")
		.addConstructor<void(*)(const float&, const float&, const float&, const float&)>()
		.addConstructor<void(*)(const Vector3&, const float&)>()
		.addFunction("length", &Quaternion::length)
		.addFunction("normalised", &Quaternion::normalised)
		.addFunction("conjugate", &Quaternion::conjugate)
		.addFunction("dot", &Quaternion::dot)
		.addFunction("nlerp", &Quaternion::nlerp)
		.addFunction("slerp", &Quaternion::slerp)
		.addFunction("getX", &Quaternion::getX)
		.addFunction("getY", &Quaternion::getY)
		.addFunction("getZ", &Quaternion::getZ)
		.addFunction("getW", &Quaternion::getW)
		.addFunction("setX", &Quaternion::setX)
		.addFunction("setY", &Quaternion::setY)
		.addFunction("setZ", &Quaternion::setZ)
		.addFunction("setW", &Quaternion::setW)
		.endClass()
		.endNamespace();*/
}

void registerKeys(lua_State* l)
{
	/*luabridge::setGlobal(l, static_cast<int>(Input::KEY_UNKNOWN), "KEY_UNKNOWN");

	luabridge::setGlobal(l, static_cast<int>(Input::KEY_A), "KEY_A");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_B), "KEY_B");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_C), "KEY_C");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_D), "KEY_D");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_E), "KEY_E");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_F), "KEY_F");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_G), "KEY_G");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_H), "KEY_H");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_I), "KEY_I");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_J), "KEY_J");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_K), "KEY_K");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_L), "KEY_L");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_M), "KEY_M");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_N), "KEY_N");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_O), "KEY_O");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_P), "KEY_P");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_Q), "KEY_Q");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_R), "KEY_R");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_S), "KEY_S");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_T), "KEY_T");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_U), "KEY_U");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_V), "KEY_V");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_W), "KEY_W");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_X), "KEY_X");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_Y), "KEY_Y");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_Z), "KEY_Z");

	luabridge::setGlobal(l, static_cast<int>(Input::KEY_1), "KEY_1");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_2), "KEY_2");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_3), "KEY_3");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_4), "KEY_4");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_5), "KEY_5");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_6), "KEY_6");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_7), "KEY_7");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_8), "KEY_8");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_9), "KEY_9");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_0), "KEY_0");

	luabridge::setGlobal(l, static_cast<int>(Input::KEY_RETURN), "KEY_RETURN");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_ESCAPE), "KEY_ESCAPE");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_BACKSPACE), "KEY_BACKSPACE");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_TAB), "KEY_TAB");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_SPACE), "KEY_SPACE");

	luabridge::setGlobal(l, static_cast<int>(Input::KEY_MINUS), "KEY_MINUS");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_EQUALS), "KEY_EQUALS");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_LEFTBRACKET), "KEY_LEFTBRACKET");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_RIGHTBRACKET), "KEY_RIGHTBRACKET");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_BACKSLASH), "KEY_BACKSLASH");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_NONUSHASH), "KEY_NONUSHASH");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_SEMICOLON), "KEY_SEMICOLON");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_APOSTROPHE), "KEY_APOSTROPHE");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_GRAVE), "KEY_GRAVE");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_COMMA), "KEY_COMMA");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_PERIOD), "KEY_PERIOD");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_SLASH), "KEY_SLASH");

	luabridge::setGlobal(l, static_cast<int>(Input::KEY_CAPSLOCK), "KEY_CAPSLOCK");

	luabridge::setGlobal(l, static_cast<int>(Input::KEY_F1), "KEY_F1");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_F2), "KEY_F2");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_F3), "KEY_F3");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_F4), "KEY_F4");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_F5), "KEY_F5");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_F6), "KEY_F6");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_F7), "KEY_F7");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_F8), "KEY_F8");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_F9), "KEY_F9");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_F10), "KEY_F10");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_F11), "KEY_F11");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_F12), "KEY_F12");

	luabridge::setGlobal(l, static_cast<int>(Input::KEY_PRINTSCREEN), "KEY_PRINTSCREEN");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_SCROLLLOCK), "KEY_SCROLLLOCK");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_PAUSE), "KEY_PAUSE");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_INSERT), "KEY_INSERT");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_HOME), "KEY_HOME");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_PAGEUP), "KEY_PAGEUP");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_DELETE), "KEY_DELETE");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_END), "KEY_END");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_PAGEDOWN), "KEY_PAGEDOWN");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_RIGHT), "KEY_RIGHT");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_LEFT), "KEY_LEFT");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_DOWN), "KEY_DOWN");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_UP), "KEY_UP");

	luabridge::setGlobal(l, static_cast<int>(Input::KEY_NUMLOCKCLEAR), "KEY_NUMLOCKCLEAR");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_NUMDIVIDE), "KEY_NUMDIVIDE");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_NUMMULTIPLY), "KEY_NUMMULTIPLY");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_NUMMINUS), "KEY_NUMMINUS");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_NUMPLUS), "KEY_NUMPLUS");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_NUMENTER), "KEY_NUMENTER");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_NUM1), "KEY_NUM1");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_NUM2), "KEY_NUM2");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_NUM3), "KEY_NUM3");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_NUM4), "KEY_NUM4");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_NUM5), "KEY_NUM5");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_NUM6), "KEY_NUM6");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_NUM7), "KEY_NUM7");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_NUM8), "KEY_NUM8");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_NUM9), "KEY_NUM9");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_NUM0), "KEY_NUM0");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_NUMDOT), "KEY_NUMDOT");

	luabridge::setGlobal(l, static_cast<int>(Input::KEY_NONUSBACKSLASH), "KEY_NONUSBACKSLASH");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_APPLICATION), "KEY_APPLICATION");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_POWER), "KEY_POWER");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_EQUALS), "KEY_KP_EQUALS");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_F13), "KEY_F13");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_F14), "KEY_F14");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_F15), "KEY_F15");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_F16), "KEY_F16");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_F17), "KEY_F17");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_F18), "KEY_F18");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_F19), "KEY_F19");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_F20), "KEY_F20");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_F21), "KEY_F21");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_F22), "KEY_F22");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_F23), "KEY_F23");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_F24), "KEY_F24");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_EXECUTE), "KEY_EXECUTE");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_HELP), "KEY_HELP");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_MENU), "KEY_MENU");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_SELECT), "KEY_SELECT");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_STOP), "KEY_STOP");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_AGAIN), "KEY_AGAIN");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_UNDO), "KEY_UNDO");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_CUT), "KEY_CUT");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_COPY), "KEY_COPY");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_PASTE), "KEY_PASTE");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_FIND), "KEY_FIND");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_MUTE), "KEY_MUTE");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_VOLUMEUP), "KEY_VOLUMEUP");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_VOLUMEDOWN), "KEY_VOLUMEDOWN");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_LOCKINGCAPSLOCK), "KEY_LOCKINGCAPSLOCK");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_LOCKINGNUMLOCK), "KEY_LOCKINGNUMLOCK");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_LOCKINGSCROLLLOCK), "KEY_LOCKINGSCROLLLOCK");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_COMMA), "KEY_KP_COMMA");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_EQUALSAS400), "KEY_KP_EQUALSAS400");

	luabridge::setGlobal(l, static_cast<int>(Input::KEY_INTERNATIONAL1), "KEY_INTERNATIONAL1");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_INTERNATIONAL2), "KEY_INTERNATIONAL2");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_INTERNATIONAL3), "KEY_INTERNATIONAL3");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_INTERNATIONAL4), "KEY_INTERNATIONAL4");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_INTERNATIONAL5), "KEY_INTERNATIONAL5");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_INTERNATIONAL6), "KEY_INTERNATIONAL6");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_INTERNATIONAL7), "KEY_INTERNATIONAL7");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_INTERNATIONAL8), "KEY_INTERNATIONAL8");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_INTERNATIONAL9), "KEY_INTERNATIONAL9");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_LANG1), "KEY_LANG1");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_LANG2), "KEY_LANG2");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_LANG3), "KEY_LANG3");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_LANG4), "KEY_LANG4");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_LANG5), "KEY_LANG5");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_LANG6), "KEY_LANG6");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_LANG7), "KEY_LANG7");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_LANG8), "KEY_LANG8");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_LANG9), "KEY_LANG9");

	luabridge::setGlobal(l, static_cast<int>(Input::KEY_ALTERASE), "KEY_ALTERASE");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_SYSREQ), "KEY_SYSREQ");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_CANCEL), "KEY_CANCEL");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_CLEAR), "KEY_CLEAR");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_PRIOR), "KEY_PRIOR");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_RETURN2), "KEY_RETURN2");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_SEPARATOR), "KEY_SEPARATOR");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_OUT), "KEY_OUT");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_OPER), "KEY_OPER");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_CLEARAGAIN), "KEY_CLEARAGAIN");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_CRSEL), "KEY_CRSEL");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_EXSEL), "KEY_EXSEL");

	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_00), "KEY_KP_00");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_000), "KEY_KP_000");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_THOUSANDSSEPARATOR), "KEY_THOUSANDSSEPARATOR");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_DECIMALSEPARATOR), "KEY_DECIMALSEPARATOR");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_CURRENCYUNIT), "KEY_CURRENCYUNIT");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_CURRENCYSUBUNIT), "KEY_CURRENCYSUBUNIT");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_LEFTPAREN), "KEY_KP_LEFTPAREN");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_RIGHTPAREN), "KEY_KP_RIGHTPAREN");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_LEFTBRACE), "KEY_KP_LEFTBRACE");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_RIGHTBRACE), "KEY_KP_RIGHTBRACE");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_TAB), "KEY_KP_TAB");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_BACKSPACE), "KEY_KP_BACKSPACE");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_A), "KEY_KP_A");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_B), "KEY_KP_B");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_C), "KEY_KP_C");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_D), "KEY_KP_D");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_E), "KEY_KP_E");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_F), "KEY_KP_F");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_XOR), "KEY_KP_XOR");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_POWER), "KEY_KP_POWER");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_PERCENT), "KEY_KP_PERCENT");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_LESS), "KEY_KP_LESS");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_GREATER), "KEY_KP_GREATER");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_AMPERSAND), "KEY_KP_AMPERSAND");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_DBLAMPERSAND), "KEY_KP_DBLAMPERSAND");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_VERTICALBAR), "KEY_KP_VERTICALBAR");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_DBLVERTICALBAR), "KEY_KP_DBLVERTICALBAR");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_COLON), "KEY_KP_COLON");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_HASH), "KEY_KP_HASH");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_SPACE), "KEY_KP_SPACE");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_AT), "KEY_KP_AT");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_EXCLAM), "KEY_KP_EXCLAM");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_MEMSTORE), "KEY_KP_MEMSTORE");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_MEMRECALL), "KEY_KP_MEMRECALL");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_MEMCLEAR), "KEY_KP_MEMCLEAR");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_MEMADD), "KEY_KP_MEMADD");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_MEMSUBTRACT), "KEY_KP_MEMSUBTRACT");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_MEMMULTIPLY), "KEY_KP_MEMMULTIPLY");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_MEMDIVIDE), "KEY_KP_MEMDIVIDE");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_PLUSMINUS), "KEY_KP_PLUSMINUS");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_CLEAR), "KEY_KP_CLEAR");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_CLEARENTRY), "KEY_KP_CLEARENTRY");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_BINARY), "KEY_KP_BINARY");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_OCTAL), "KEY_KP_OCTAL");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_DECIMAL), "KEY_KP_DECIMAL");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KP_HEXADECIMAL), "KEY_KP_HEXADECIMAL");

	luabridge::setGlobal(l, static_cast<int>(Input::KEY_LCTRL), "KEY_LCTRL");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_LSHIFT), "KEY_LSHIFT");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_LALT), "KEY_LALT");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_LGUI), "KEY_LGUI");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_RCTRL), "KEY_RCTRL");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_RSHIFT), "KEY_RSHIFT");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_RALT), "KEY_RALT");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_RGUI), "KEY_RGUI");

	luabridge::setGlobal(l, static_cast<int>(Input::KEY_MODE), "KEY_MODE");

	luabridge::setGlobal(l, static_cast<int>(Input::KEY_AUDIONEXT), "KEY_AUDIONEXT");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_AUDIOPREV), "KEY_AUDIOPREV");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_AUDIOSTOP), "KEY_AUDIOSTOP");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_AUDIOPLAY), "KEY_AUDIOPLAY");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_AUDIOMUTE), "KEY_AUDIOMUTE");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_MEDIASELECT), "KEY_MEDIASELECT");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_WWW), "KEY_WWW");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_MAIL), "KEY_MAIL");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_CALCULATOR), "KEY_CALCULATOR");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_COMPUTER), "KEY_COMPUTER");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_AC_SEARCH), "KEY_AC_SEARCH");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_AC_HOME), "KEY_AC_HOME");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_AC_BACK), "KEY_AC_BACK");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_AC_FORWARD), "KEY_AC_FORWARD");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_AC_STOP), "KEY_AC_STOP");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_AC_REFRESH), "KEY_AC_REFRESH");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_AC_BOOKMARKS), "KEY_AC_BOOKMARKS");

	luabridge::setGlobal(l, static_cast<int>(Input::KEY_BRIGHTNESSDOWN), "KEY_BRIGHTNESSDOWN");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_BRIGHTNESSUP), "KEY_BRIGHTNESSUP");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_DISPLAYSWITCH), "KEY_DISPLAYSWITCH");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KBDILLUMTOGGLE), "KEY_KBDILLUMTOGGLE");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KBDILLUMDOWN), "KEY_KBDILLUMDOWN");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_KBDILLUMUP), "KEY_KBDILLUMUP");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_EJECT), "KEY_EJECT");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_SLEEP), "KEY_SLEEP");

	luabridge::setGlobal(l, static_cast<int>(Input::KEY_APP1), "KEY_APP1");
	luabridge::setGlobal(l, static_cast<int>(Input::KEY_APP2), "KEY_APP2");*/
}

void registerMouse(lua_State* l)
{
	/*luabridge::setGlobal(l, static_cast<int>(Input::MOUSE_LEFT), "MOUSE_LEFT");
	luabridge::setGlobal(l, static_cast<int>(Input::MIDDLE_MOUSE), "MIDDLE_MOUSE");
	luabridge::setGlobal(l, static_cast<int>(Input::MOUSE_LEFT), "MOUSE_RIGHT");
	luabridge::setGlobal(l, static_cast<int>(Input::MOUSE_WHEEL_UP), "MOUSE_WHEEL_UP");
	luabridge::setGlobal(l, static_cast<int>(Input::MOUSE_WHEEL_DOWN), "MOUSE_WHEEL_DOWN");*/
}

float toRadiansLua(float degrees)
{
	return 0;
	//return toRadians(degrees);
}

float randomLua(float min, float max)
{
	return 0;
	//return random(min, max);
}