#pragma once

#include "raylib.h"
#include "Utility.h"

class Editor 
{
	struct Tile {
		Vector2 position;
		Rectangle rectangle;
	};

public:
	Editor(int width, int height, const std::string& projectPath);
	virtual ~Editor();

	void Start();

private:
	std::string m_projectPath;
	Texture2D m_tileset;
	bool m_isRunning;
	Camera2D m_mainCamera;

	std::vector<EditorItem> m_panelItems;

	EditorItem m_selectedItem;

	std::vector<EditorItem> m_canvasItem;

	Rectangle m_btnSaveRec;
	bool m_btnSaveMouseHover;
	bool m_showSaveMessage;
	int m_saveMessageCounter;

	Rectangle m_btnClearRec;
	bool m_btnClearMouseHover;

	Rectangle m_btnRandomRec;
	bool m_btnRandomMouseHover;

	void InitializePanel();

	void InitializeCanvas();

	void HandleInput();

	void DrawPanel();

	void DrawTileGrid();

	void DrawCanvas();

	void SaveLevel();

	void LoadLevel();

	void ClearLevel();

	void RandomLevel();

	void UpdateSaveMsgTimer(float frameDelta);
};