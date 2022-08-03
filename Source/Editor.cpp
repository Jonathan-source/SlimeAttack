#include "pch.h"
#include "Editor.h"


Editor::Editor(int width, int height, const std::string &projectPath)
    : m_isRunning(false)
    , m_windowWidth(width)
    , m_windowHeight(height)
    , m_projectPath(projectPath)
    , m_panelOffset({ OPT_WIDTH, 0.f })
    , m_selectedItem({})
    , m_framesTime(0.f)
    , m_frameCount(0)
{
    InitWindow(width, height, "Editor");
    SetTargetFPS(60);

    m_tileset = LoadTexture(std::string(m_projectPath + "\\Assets\\Textures\\tileset.png").c_str());
    SetTextureFilter(m_tileset, TEXTURE_FILTER_TRILINEAR);

 
    InitOptionPanel();

    InitEditableTiles();
}

void Editor::Start()
{
    //Camera2D mainCamera;
    //mainCamera.target = { 0,0 };
    //mainCamera.offset = { 400, 300 };
    //mainCamera.zoom = 10.0f;
    //mainCamera.rotation = 0.0f;

    m_isRunning = true;
    while (!WindowShouldClose() && m_isRunning)
    {
        UpdateFPS();

        // Input
        if (IsKeyDown(KEY_LEFT))    m_panelOffset.x += 500.0f * GetFrameTime();
        if (IsKeyDown(KEY_RIGHT))   m_panelOffset.x -= 500.0f * GetFrameTime();
        if (IsKeyDown(KEY_UP))      m_panelOffset.y += 500.0f * GetFrameTime();
        if (IsKeyDown(KEY_DOWN))    m_panelOffset.y -= 500.0f * GetFrameTime();
        if (IsKeyDown(KEY_SPACE)) { m_panelOffset.x = OPT_WIDTH; m_panelOffset.y = 0.f; }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            const Vector2 mouse = GetMousePosition();

            for (int i = 0; i < m_optionPanelTiles.size(); i++)
            {
                if (CheckCollisionPointRec(mouse, m_optionPanelTiles[i]))
                {
                    m_selectedItem = m_optionPanelTiles[i];
                    break;
                }
            }

            if (mouse.x > OPT_WIDTH)
            {
                int fx = (mouse.x - m_panelOffset.x) / TILE_SIZE;
                int fy = mouse.y / TILE_SIZE;

                m_levelMatrix[fy][fx].textureRec = m_selectedItem;
                m_levelMatrix[fy][fx].position.x = fx;
                m_levelMatrix[fy][fx].position.y = fy;

                std::cout << fx << " " << fy << std::endl;

            }
        }
        //BeginMode2D(mainCamera);
        BeginDrawing();

        // Drawing     
        ClearBackground(RAYWHITE);

        // Draw Editable area
        DrawLevel();

        // Draw Grid lines
        DrawEditorGrid();

        // Draw options
        DrawOptions();

        //EndMode2D();
        EndDrawing();
    }

    UnloadTexture(m_tileset);

    CloseAudioDevice();
    CloseWindow();
}

void Editor::InitOptionPanel()
{
    for (int y = 0; y < TILESET_HEIGHT; y++)
    {
        for (int x = 0; x < TILESET_WIDTH; x++)
        {
            Rectangle tile {
                static_cast<float>(x * TILE_SIZE),
                static_cast<float>(y * TILE_SIZE),
                static_cast<float>(TILE_SIZE),
                static_cast<float>(TILE_SIZE)
            };
            m_optionPanelTiles.emplace_back(tile);
        }
    }
}

void Editor::InitEditableTiles()
{
    // Default nothing map
    for (int y = 0; y < LEVEL_HEIGHT; y++)
    {
        for (int x = 0; x < LEVEL_WIDTH; x++)
        {
        }
    }
}

void Editor::UpdateFPS()
{
    m_framesTime += GetFrameTime();
    m_frameCount++;
    if (m_framesTime > 1.0) {
        char fpsText[32];
        sprintf(fpsText, std::string("Editor: %d FPS").c_str(), m_frameCount);
        SetWindowTitle(fpsText);
        m_frameCount = 0;
        m_framesTime = 0;
    }
}

void Editor::DrawLevel()
{
    for (int y = 0; y < LEVEL_HEIGHT; y++)
    {
        for (int x = 0; x < LEVEL_WIDTH; x++)
        {
            Vector2 position{
                static_cast<float>(x * TILE_SIZE + m_panelOffset.x),
                static_cast<float>(y * TILE_SIZE)
            };

            DrawTextureRec(m_tileset, m_levelMatrix[y][x].textureRec, position, WHITE);
        }
    }
}

void Editor::DrawOptions()
{
    // Draw panel
    DrawRectangle(0.f, 0.f, OPT_WIDTH - MARGIN_SIZE, GetScreenHeight(), ColorAlpha(LIGHTGRAY, 0.5f));
    Vector2 start{ OPT_WIDTH - MARGIN_SIZE, 0.f };
    Vector2 end{ OPT_WIDTH - MARGIN_SIZE, GetScreenHeight() };
    DrawLineEx(start, end, MARGIN_SIZE, DARKGRAY);

    // Draw tileset
    for (int y = 0; y < 20; y++)
    {
        for (int x = 0; x < 10; x++)
        {
            Rectangle source{
                static_cast<float>(x * TILE_SIZE),
                static_cast<float>(y * TILE_SIZE),
                static_cast<float>(TILE_SIZE),
                static_cast<float>(TILE_SIZE)
            };

            Vector2 position{
                static_cast<float>(x * TILE_SIZE) + (x + 1),
                static_cast<float>(y * TILE_SIZE) + (y + 1)
            };

            DrawTextureRec(m_tileset, source, position, WHITE);
        }
    }

    // Draw layers


    // Draw current selected tile overlay
    DrawRectangle(m_selectedItem.x, m_selectedItem.y, m_selectedItem.width, m_selectedItem.height, ColorAlpha(RED, 0.5f));
}

void Editor::DrawEditorGrid()
{
    for (int y = 0; y < LEVEL_HEIGHT + 1; y++)
    {
        DrawLine(m_panelOffset.x, y * TILE_SIZE, m_panelOffset.x + LEVEL_WIDTH * TILE_SIZE, y * TILE_SIZE, BLACK);
        for (int x = 0; x < LEVEL_WIDTH; x++)
        {
            int xPos = m_panelOffset.x + x * TILE_SIZE;
            DrawLine(xPos, 0, xPos, LEVEL_HEIGHT * TILE_SIZE, BLACK);
        }
    }
}
