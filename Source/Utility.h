#pragma once 

enum class EColor {
    Black,
    White,
    Red,
    Green,
    Blue,
    Yellow,
};

struct EditorItem {
    Rectangle source;
    Rectangle dest;
    int tileID;
};

struct LevelData {
	int width;
	int height;
	std::vector<int> tiles;
};

struct TextureRec {
    std::string textureName;
    Rectangle source;
    Vector2 position;
};

struct TextData {
    std::string text;
    int x;
    int y;
    int fontSize;
    Color color;
};

bool CheckLua(lua_State * L, int err);

void DumpStack(lua_State * L);

Color GetColor(EColor color);

