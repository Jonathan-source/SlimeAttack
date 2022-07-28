#pragma once

#include "raylib.h"

#define LEVEL_WIDTH 30
#define LEVEL_HEIGHT 20

#define VISIBLE_TILES_X 16
#define VISIBLE_TILES_Y 12

#define TILE_SIZE 32

#define TILESET_WIDTH 20
#define TILESET_HEIGHT 20

#define OPT_WIDTH 400 
#define MARGIN_SIZE 5

class Editor
{
public:
    Editor(int width, int height, const std::string &projectPath);
    virtual ~Editor() = default;

    void Run();

private:
    bool m_isRunning;

    int m_windowWidth;
    int m_windowHeight;
    std::string m_projectPath;

    Texture m_tileset;
    Vector2 m_panelOffset;

    std::vector<Rectangle> m_optionPanelTiles;
    std::vector<Rectangle> m_editableTiles;

    Rectangle m_selectedItem;

    float m_framesTime;
    int m_frameCount;

    struct Tile {
        Vector2 position;
        Rectangle textureRec;
    };

    std::array<std::vector<Tile>, 4> m_layers;

    void InitOptionPanel();
    void InitEditableTiles();

    Tile m_levelMatrix[LEVEL_HEIGHT][LEVEL_WIDTH];

    void UpdateFPS();

    void LoadLevel();
    void SaveLevel();

    void DrawLevel();
    void DrawOptions();

    void DrawEditorGrid();
};