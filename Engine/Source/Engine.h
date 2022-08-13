#pragma once

#include "raylib.h"
#include "ResourceManager.h"
#include "Utility.h"

class Engine
{
public:
    Engine(lua_State* L, const std::string &projectPath);
    virtual ~Engine() = default;

    bool Preload();

    bool Initialize();

    void Start();

private:
    lua_State* L;
    bool m_isRunning;

    std::string m_projectPath;
    std::string m_mainScript;
    Camera2D m_mainCamera;

    int m_windowWidth;
    int m_windowHeight;
    int m_targetFPS;
    std::string m_title;

    float m_frameTimeAccumulator;
    int m_frameCount;

    std::vector<TextureRec> m_renderTextureBatch;
    std::queue<TextData> m_renderTextQueue;

    // Core Engine 
    void RegisterLuaFunctions();
    void LoadResources();
    bool LoadMainScript();

    void UpdateFPS();
    void OnUpdate();
    void OnRender();
    void Shutdown();

    void InitializeEngine(int width, int height, int targetFPS, const std::string& title, const std::string& script);
    LevelData LoadLevel(const std::string &level);

    // LuaWrappers
    static int wrap_InitializeEngine(lua_State * L);
    static int wrap_LoadLevel(lua_State* L);
    static int wrap_DrawTextureRec(lua_State* L);
    static int wrap_IsKeyDown(lua_State* L);
    static int wrap_IsKeyPressed(lua_State* L);
    static int wrap_SetCameraTarget(lua_State* L);
    static int wrap_SetCameraOffset(lua_State* L);
    static int wrap_SetCameraZoom(lua_State* L);
    static int wrap_SetCameraRotation(lua_State* L);
    static int wrap_DrawRectangle(lua_State* L);
    static int wrap_CheckCollisionRect(lua_State* L);
    static int wrap_DrawTexturePro(lua_State* L);
    static int wrap_GetMousePosition(lua_State* L);
    static int wrap_IsMouseButtonDown(lua_State* L);
    static int wrap_IsMouseButtonPressed(lua_State* L);
    static int wrap_GetMouseScreenToWorld2D(lua_State* L);
    static int wrap_AddToRenderBatch(lua_State* L);
    static int wrap_DrawText(lua_State* L);
    static int wrap_PostRenderText(lua_State* L);
};