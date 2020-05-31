#include "pch.h"

#include "dump/WowGameOffsets.h"

#include "Lua.h"

typedef uint64_t(__fastcall FrameScript_ExecuteBuffer)(const char*, const char*, uint64_t);

int lua_callback(void* L)
{
	//int myvar = lua_toint(L);
	//Logger << myvar << std::endl;
	return 0;
}



Lua::Lua(WowGame& game) :
	mGame(game) {

	//	PatchFunction();
	//RegisterFunction("test_call", lua_callback);
}

Lua::~Lua() {

}

void Lua::run(const std::string& script) {
	run(script.c_str());
}

void Lua::run(const char* script) {
	mGame.getFunction<FrameScript_ExecuteBuffer>(WowGameOffsets::WowGame::FunctionFrameScript_ExecuteBuffer)(script, script, 0);
}
