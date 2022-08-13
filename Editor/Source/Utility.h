#pragma once 

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


