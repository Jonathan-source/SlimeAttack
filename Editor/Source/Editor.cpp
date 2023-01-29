#include "pch.h"
#include "Editor.h"

#define PANEL_WIDTH 80
#define PANEL_ITEM_SIZE 64
#define NUM_PANEL_ITEMS 8

#define REAL_TILE_SIZE 16

#define LEVEL_WIDTH	35
#define LEVEL_HEIGHT 35

Editor::Editor(int width, int height, const std::string& projectPath)
	: m_projectPath(projectPath)
	, m_mainCamera({0})
	, m_isRunning(false)
	, m_saveMessageCounter(0)
	, m_showSaveMessage(false)
	, m_btnSaveMouseHover(false)
	, m_btnClearMouseHover(false)
{
	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
	InitWindow(width, height, "Editor");
	SetTargetFPS(60);

	std::string tilesetPath = projectPath + "\\Assets\\Textures\\tileset.png";
	m_tileset = LoadTexture(tilesetPath.c_str());

	InitializePanel();
	m_selectedItem = m_panelItems[0];

	InitializeCanvas();
	LoadLevel();

	m_mainCamera.zoom = 1.0f;
	m_isRunning = true;
}

Editor::~Editor()
{
	UnloadTexture(m_tileset);
}

void Editor::Start()
{
	while (!WindowShouldClose() && m_isRunning)
	{
		// Update
		UpdateSaveMsgTimer(GetFrameTime());

		HandleInput();

		// Render
		ClearBackground(RAYWHITE);

		BeginDrawing();

		BeginMode2D(m_mainCamera);

		DrawTileGrid();
		DrawCanvas();

		EndMode2D();

		DrawPanel();

		EndDrawing();
	}

	CloseWindow();
}

void Editor::InitializePanel()
{
	const int OFFSET_X = 8;
	const int OFFSET_Y = 6;
	for (int i = 0; i < NUM_PANEL_ITEMS; i++)
	{
		int posY = (i * PANEL_ITEM_SIZE + OFFSET_Y) + (i * OFFSET_Y);

		Rectangle source { (float)(i * REAL_TILE_SIZE), 0, (float)REAL_TILE_SIZE, (float)REAL_TILE_SIZE };
		Rectangle dest { (float)(GetScreenWidth() - PANEL_WIDTH + OFFSET_X), (float)posY, (float)PANEL_ITEM_SIZE, (float)PANEL_ITEM_SIZE};
		EditorItem panelItem { source, dest };

		m_panelItems.push_back(panelItem);
	}

	m_btnClearRec = { (float)(GetScreenWidth() - (PANEL_WIDTH * 2.5)), 20, 85, 60 };
	m_btnSaveRec = { (float)(GetScreenWidth() - (PANEL_WIDTH * 2.5)), 100, 85, 60 };
	m_btnRandomRec = { (float)(GetScreenWidth() - (PANEL_WIDTH * 2.5)), 180, 85, 60 };
}

void Editor::InitializeCanvas()
{
	for (int y = 0; y < LEVEL_HEIGHT; y++)
	{
		for (int x = 0; x < LEVEL_WIDTH; x++)
		{
			int idx = (rand() % 5 + 1);

			Rectangle source{ (float)(idx * REAL_TILE_SIZE), 0, (float)REAL_TILE_SIZE, (float)REAL_TILE_SIZE };
			Rectangle dest{ (float)(x * REAL_TILE_SIZE), (float)(y * REAL_TILE_SIZE), (float)REAL_TILE_SIZE, (float)REAL_TILE_SIZE };
			EditorItem canvasItem{ source, dest, idx };
			m_canvasItem.push_back(canvasItem);
		}
	}
}

void Editor::DrawPanel()
{
	// Drawn panel
	DrawRectangle(GetScreenWidth() - PANEL_WIDTH, 0, PANEL_WIDTH, GetScreenHeight(), DARKGRAY);

	// Drawn panel buttons
	for (auto& item : m_panelItems) {
		DrawTextureTiled(m_tileset, item.source, item.dest, {}, 0, 4.0f, WHITE);
	}

	// Drawn selected panel button
	DrawRectangle(m_selectedItem.dest.x, m_selectedItem.dest.y, m_selectedItem.dest.width, m_selectedItem.dest.height, ColorAlpha(RED, 0.5f));

	// Drawn clear button
	DrawRectangleLinesEx(m_btnClearRec, 2, m_btnClearMouseHover ? RED : BLACK);
	DrawText("CLEAR", m_btnClearRec.x + 10, m_btnClearRec.y + 20, 20, m_btnClearMouseHover ? RED : BLACK);

	// Draw save button
	DrawRectangleLinesEx(m_btnSaveRec, 2, m_btnSaveMouseHover ? RED : BLACK);
	DrawText("SAVE!", m_btnSaveRec.x + 10, m_btnSaveRec.y + 20, 20, m_btnSaveMouseHover ? RED : BLACK);

	// Draw save button
	DrawRectangleLinesEx(m_btnRandomRec, 2, m_btnRandomMouseHover ? RED : BLACK);
	DrawText("RAND!", m_btnRandomRec.x + 10, m_btnRandomRec.y + 20, 20, m_btnRandomMouseHover ? RED : BLACK);


	// Draw save image message
	if (m_showSaveMessage)
	{
		DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(RAYWHITE, 0.8f));
		DrawRectangle(0, 150, GetScreenWidth(), 80, BLACK);
		std::string levelName = "level.eff";
		std::string text = "LEVEL SAVED: '" + levelName + "'";
		DrawText(text.c_str(), GetScreenWidth() / 3, 180, 20, RAYWHITE);
	}
}

void Editor::HandleInput()
{
	const Vector2 mouse = GetMousePosition();

	// Panel
	if (mouse.x >= GetScreenWidth() - PANEL_WIDTH)
	{
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			for (int i = 0; i < m_panelItems.size(); i++)
			{
				if (CheckCollisionPointRec(mouse, m_panelItems[i].dest))
				{
					m_selectedItem = m_panelItems[i];
					m_selectedItem.tileID = i;
					break;
				}
			}
		}
	}
	else // Canvas
	{
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			for (int i = 0; i < m_canvasItem.size(); i++)
			{
				if (CheckCollisionPointRec(mouse, m_canvasItem[i].dest))
				{
					m_canvasItem[i].source = m_selectedItem.source;
					m_canvasItem[i].tileID = m_selectedItem.tileID;
				}
			}
		}
	}

	// Check mouse hover save button
	m_btnSaveMouseHover = false;
	if (CheckCollisionPointRec(mouse, m_btnSaveRec))
	{
		m_btnSaveMouseHover = true;
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
		{
			m_showSaveMessage = true;
			if (m_canvasItem.empty())
			{
				if (std::filesystem::remove("level.eff"))
				{
					std::cout << "\n'level.eff' was successfully removed." << std::endl;
				}
			}
			else
			{
				SaveLevel();
			}
		}
	}

	// Check mouse hover clear button
	m_btnClearMouseHover = false;
	if (CheckCollisionPointRec(mouse, m_btnClearRec))
	{
		m_btnClearMouseHover = true;
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
		{
			ClearLevel();
		}
	}

	// Check mouse hover random button
	m_btnRandomMouseHover = false;
	if (CheckCollisionPointRec(mouse, m_btnRandomRec))
	{
		m_btnRandomMouseHover = true;
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
		{
			RandomLevel();
		}
	}
}

void Editor::DrawTileGrid()
{
	const int MAX_WIDTH = LEVEL_WIDTH * REAL_TILE_SIZE;
	const int MAX_HEIGHT = LEVEL_HEIGHT * REAL_TILE_SIZE;

	for (int y = 0; y < LEVEL_HEIGHT; y++)
	{
		DrawLine(0, y * REAL_TILE_SIZE, MAX_WIDTH, y * REAL_TILE_SIZE, BLACK); // horizontal lines
		for (int x = 0; x < LEVEL_WIDTH; x++)
		{
			int xPos = x * REAL_TILE_SIZE;
			DrawLine(xPos, 0, xPos, MAX_HEIGHT, BLACK); // vertical lines
		}
	}
	DrawLine(0, MAX_WIDTH, MAX_WIDTH, MAX_HEIGHT, BLACK);
	DrawLine(MAX_HEIGHT, 0, MAX_HEIGHT, MAX_WIDTH, BLACK);
}

void Editor::DrawCanvas()
{
	for (auto& item : m_canvasItem) {
		DrawTextureRec(m_tileset, item.source, {item.dest.x, item.dest.y}, WHITE);
	}
}

void Editor::SaveLevel()
{
	std::fstream writer;
	writer.open(m_projectPath + "\\Assets\\Levels\\level.eff", std::ios::out | std::ios::binary);
	if (writer.is_open())
	{
		size_t level_width = (size_t)LEVEL_WIDTH;
		size_t level_height = (size_t)LEVEL_HEIGHT;

		writer.write(reinterpret_cast<char*>(&level_width), sizeof(size_t)); // width
		writer.write(reinterpret_cast<char*>(&level_height), sizeof(size_t)); // height
		writer.write(reinterpret_cast<char*>(&m_canvasItem[0]), (level_width * level_height) * sizeof(EditorItem));

		writer.close();

		std::cout << "\Editor::SaveLevel() complete.\n\n";
	}
	else
	{
		std::cout << "\nAn error occured when writing data to file.\n\n";
		writer.clear();
	}
}

void Editor::LoadLevel()
{
	std::fstream reader;
	reader.open(m_projectPath + "\\Assets\\Levels\\level.eff", std::ios::in | std::ios::binary);
	if (reader.is_open())
	{
		size_t level_width = 0;
		size_t level_height = 0;

		reader.read(reinterpret_cast<char*>(&level_width), sizeof(size_t));
		reader.read(reinterpret_cast<char*>(&level_height), sizeof(size_t));

		m_canvasItem.clear();
		m_canvasItem.resize(level_width * level_height);

		reader.read(reinterpret_cast<char*>(&m_canvasItem[0]), (level_width * level_height) * sizeof(EditorItem));

		reader.close();

		std::cout << "\Editor::LoadLevel() complete.\n\n";
	}
	else
	{
		std::cout << "\nAn error occured when reading data from file.\n\n";
		reader.clear();
	}
}

void Editor::ClearLevel()
{
	for (auto& item : m_canvasItem)
	{
		item.source.x = 0;
	}
}

void Editor::RandomLevel()
{
	for (auto& item : m_canvasItem)
	{
		item.tileID = rand() % 5 + 1;
		item.source.x = item.tileID * REAL_TILE_SIZE;
	}
}

void Editor::UpdateSaveMsgTimer(float frameDelta)
{
	if (m_showSaveMessage)
	{
		m_saveMessageCounter++;
		if (m_saveMessageCounter > 120) // 60 frames per sec: 2s = 120.
		{
			m_showSaveMessage = false;
			m_saveMessageCounter = 0;
		}
	}
}

