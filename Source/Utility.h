#pragma once 

struct LevelData {
	int width;
	int height;
	std::vector<int> cells;
};

bool CheckLua(lua_State * L, int err);

void DumpStack(lua_State * L);

