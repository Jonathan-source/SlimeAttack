#include "pch.h"
#include "Engine.h"

Engine::Engine(lua_State * L, const std::string &projectPath)
    : L(L)
    , m_projectPath(projectPath)
    , m_isRunning(false)
    , m_frameTimeAccumulator(0.f)
    , m_frameCount(0)
    , m_windowWidth(100)
    , m_windowHeight(100)
    , m_title("Default")
{
    RegisterLuaFunctions();
}

bool Engine::Preload()
{
    std::string luaFileToLoad = m_projectPath + "\\Assets\\Scripts\\" + "Settings.lua";

    if (!CheckLua(L, luaL_dofile(L, luaFileToLoad.c_str())))
    {
        LOG_ERROR("settings file was not loaded correctly");
        return false;
    }

    if (!lua_getglobal(L, "preload") || !lua_isfunction(L, -1))
    {
        LOG_ERROR("'preload' function not found");
        return false;
    }

    lua_pushlightuserdata(L, this);
    return CheckLua(L, lua_pcall(L, 1, 0, 0));
}

bool Engine::Initialize()
{
    InitWindow(m_windowWidth, m_windowHeight, m_title.c_str());
    SetTargetFPS(m_targetFPS);
    InitAudioDevice();

    LoadResources();

    m_isRunning = LoadMainScript();
    return m_isRunning;
}

void Engine::Start()
{    
    while (!WindowShouldClose() && m_isRunning)
    {
        UpdateFPS();

        // Update
        OnUpdate();

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(m_mainCamera);

        // PreRender Map 
        for (const auto& it : m_renderTextureBatch) {
            DrawTextureRec(ResourceManager::Get().GetTexture(it.textureName), it.source, it.position, WHITE);
        }

        // Render
        OnRender();

        EndMode2D();

        // PostRender Text
        while (!m_renderTextQueue.empty()) {
            TextData data = m_renderTextQueue.front();
            m_renderTextQueue.pop();
            DrawText(data.text.c_str(), data.x, data.y, data.fontSize, data.color);
        }

        EndDrawing();

        UpdateMusicStream(m_music);
    }

    CloseAudioDevice();

    CloseWindow();
}

void Engine::RegisterLuaFunctions()
{
    lua_register(L, "_InitializeEngine",        wrap_InitializeEngine);
    lua_register(L, "_LoadLevel",               wrap_LoadLevel);
    lua_register(L, "_DrawTextureRec",          wrap_DrawTextureRec);
    lua_register(L, "_IsKeyDown",               wrap_IsKeyDown);
    lua_register(L, "_IsKeyPressed",            wrap_IsKeyPressed);
    lua_register(L, "_SetCameraTarget",         wrap_SetCameraTarget);
    lua_register(L, "_SetCameraOffset",         wrap_SetCameraOffset);
    lua_register(L, "_SetCameraZoom",           wrap_SetCameraZoom);
    lua_register(L, "_SetCameraRotation",       wrap_SetCameraRotation);
    lua_register(L, "_DrawRectangle",           wrap_DrawRectangle);
    lua_register(L, "_CheckCollisionRect",      wrap_CheckCollisionRect);
    lua_register(L, "_DrawTexturePro",          wrap_DrawTexturePro);
    lua_register(L, "_GetMousePosition",        wrap_GetMousePosition);
    lua_register(L, "_IsMouseButtonDown",       wrap_IsMouseButtonDown);
    lua_register(L, "_IsMouseButtonPressed",    wrap_IsMouseButtonPressed);
    lua_register(L, "_GetMouseScreenToWorld2D", wrap_GetMouseScreenToWorld2D);
    lua_register(L, "_AddToRenderBatch",        wrap_AddToRenderBatch);
    lua_register(L, "_DrawText",                wrap_DrawText);
    lua_register(L, "_PostRenderText",          wrap_PostRenderText);
    lua_register(L, "_PlaySound",               wrap_PlaySound);
    lua_register(L, "_PlayMusic",               wrap_PlayMusic);
    lua_register(L, "_StopMusic",               wrap_StopMusic);
}

void Engine::LoadResources()
{
    ResourceManager::Get().SetResourcePath(m_projectPath + "\\Assets");
    
    ResourceManager::Get().GetTexture("tileset.png");
    ResourceManager::Get().GetTexture("player_knight.png");
    ResourceManager::Get().GetTexture("enemy_slime.png");
    ResourceManager::Get().GetTexture("enemy_slime_red.png");
    ResourceManager::Get().GetTexture("weapon_knife.png");
    ResourceManager::Get().GetTexture("item_health_potion.png");

    ResourceManager::Get().GetFont("mono.ttf");

    ResourceManager::Get().GetSound("item_pickup.ogg");
    ResourceManager::Get().GetSound("life_lost.ogg");
    ResourceManager::Get().GetSound("game_over.ogg");

    ResourceManager::Get().GetMusic("overworld.ogg");
}

bool Engine::LoadMainScript()
{
    std::string luaFileToLoad = m_projectPath + "\\Assets\\Scripts\\" + m_mainScript;

    if (!CheckLua(L, luaL_dofile(L, luaFileToLoad.c_str())))
    {
        LOG_ERROR("main script was not loaded correctly");
        return false;
    }

    if (!lua_getglobal(L, "onUpdate") || !lua_isfunction(L, -1))
    {
        LOG_ERROR("'onUpdate' function not found");
        return false;
    }
    lua_pop(L, -1);

    if (!lua_getglobal(L, "onRender") || !lua_isfunction(L, -1))
    {
        LOG_ERROR("'onRender' function not found");
        return false;
    }
    lua_pop(L, -1);

    if (!lua_getglobal(L, "onInit") || !lua_isfunction(L, -1))
    {
        LOG_ERROR("'onInit' function not found");
        return false;
    }

    return CheckLua(L, lua_pcall(L, 0, 0, 0));
}

void Engine::UpdateFPS()
{
    m_frameTimeAccumulator += GetFrameTime();
    m_frameCount++;
    if (m_frameTimeAccumulator > 1.f) {
        char fpsText[32];
        sprintf(fpsText, std::string(m_title + ": %d FPS").c_str(), m_frameCount);
        SetWindowTitle(fpsText);
        m_frameTimeAccumulator = 0;
        m_frameCount = 0;
    }
}

void Engine::OnUpdate()
{
    lua_getglobal(L, "onUpdate");
    if (lua_isfunction(L, -1))
    {
        lua_pushnumber(L, (double)GetFrameTime());
        if (lua_pcall(L, 1, 0, 0) != LUA_OK)
        {
            LOG_ERROR("'onUpdate' function failed");
        }
    }
}

void Engine::OnRender()
{
    lua_getglobal(L, "onRender");
    if (lua_isfunction(L, -1))
    {
        if (lua_pcall(L, 0, 0, 0) != LUA_OK)
        {
            LOG_ERROR("'onRender' function failed");
        }
    }
}

void Engine::Shutdown()
{
    m_isRunning = false;
}

void Engine::InitializeEngine(int width, int height, int targetFPS, const std::string& title, const std::string& script)
{
    m_windowWidth = width;
    m_windowHeight = height;
    m_targetFPS = targetFPS;
    m_title = title;
    m_mainScript = script;
}

LevelData Engine::LoadLevel(const std::string& level)
{
    LevelData levelData;

    std::vector<EditorItem> temp;

    std::ifstream reader;
    reader.open(m_projectPath + "\\Assets\\Levels\\" + level, std::ios::in | std::ios::binary);
    if (reader.is_open())
    {
        size_t level_width = 0;
        size_t level_height = 0;

        reader.read(reinterpret_cast<char*>(&level_width), sizeof(size_t));
        reader.read(reinterpret_cast<char*>(&level_height), sizeof(size_t));

        temp.clear();
        temp.resize(level_width * level_height);

        reader.read(reinterpret_cast<char*>(&temp[0]), (level_width * level_height) * sizeof(EditorItem));

        reader.close();

        levelData.width = (int)level_width;
        levelData.height = (int)level_height;
        for (const auto& item : temp)
        {
            levelData.tiles.push_back(item.tileID);
        }
        std::cout << "\nEngine::LoadLevel() complete.\n\n";
    }
    else
    {
        std::cout << "\nAn error occured when reading data from file.\n\n";
        reader.clear();
    }

    return levelData;
}

int Engine::wrap_InitializeEngine(lua_State* L)
{
    if (lua_gettop(L) != 6) return -1;

    Engine* engine = (Engine*)lua_touserdata(L, 1);
    std::string windowTitle = lua_tostring(L, 2);
    int windowWidth = (int)lua_tointeger(L, 3);
    int windowHeight = (int)lua_tointeger(L, 4);
    int targetFPS = (int)lua_tointeger(L, 5);
    std::string mainScript  = lua_tostring(L, 6);
    
    engine->InitializeEngine(
        windowWidth,
        windowHeight,
        targetFPS,
        windowTitle,
        mainScript
    );

    return 0;
}

int Engine::wrap_DrawTextureRec(lua_State* L)
{
    if (lua_gettop(L) != 8) return -1;

    Engine* engine = (Engine*)lua_touserdata(L, 1);
    std::string textureName = lua_tostring(L, 2);
    float x = (float)lua_tonumber(L, 3);
    float y = (float)lua_tonumber(L, 4);
    float width = (float)lua_tonumber(L, 5);
    float height = (float)lua_tonumber(L, 6);
    float posX = (float)lua_tonumber(L, 7);
    float posY = (float)lua_tonumber(L, 8);

    DrawTextureRec(ResourceManager::Get().GetTexture(textureName),
        { x, y, width, height }, 
        { posX, posY }, 
        WHITE);

    return 0;
}

int Engine::wrap_DrawTexturePro(lua_State* L)
{
    if (lua_gettop(L) != 10) return -1;

    Engine* engine = (Engine*)lua_touserdata(L, 1);
    std::string textureName = lua_tostring(L, 2);
    float x = (float)lua_tonumber(L, 3);
    float y = (float)lua_tonumber(L, 4);
    float width = (float)lua_tonumber(L, 5);
    float height = (float)lua_tonumber(L, 6);
    float posX = (float)lua_tonumber(L, 7);
    float posY = (float)lua_tonumber(L, 8);
    float scale = (float)lua_tonumber(L, 9);
    float rotation = (float)lua_tonumber(L, 10);

    Texture texture = ResourceManager::Get().GetTexture(textureName);
    Rectangle source = { x, y, width, height };
    Rectangle dest = { posX , posY, scale * width, scale * height };
    DrawTexturePro(texture, source, dest, Vector2{}, rotation, WHITE);

    return 0;
}

int Engine::wrap_IsKeyDown(lua_State* L)
{
    if (lua_gettop(L) != 2) return -1;

    Engine* engine = (Engine*)lua_touserdata(L, 1);
    int key = (int)lua_tointeger(L, 2);
    bool isDown = IsKeyDown((KeyboardKey)key);
    lua_pushboolean(L, isDown);

    return 1;
}

int Engine::wrap_IsKeyPressed(lua_State* L)
{
    if (lua_gettop(L) != 2) return -1;

    Engine* engine = (Engine*)lua_touserdata(L, 1);
    int key = (int)lua_tointeger(L, 2);
    bool isPressed = IsKeyPressed((KeyboardKey)key);
    lua_pushboolean(L, isPressed);

    return 1;
}

int Engine::wrap_SetCameraTarget(lua_State* L)
{
    if (lua_gettop(L) != 3) return -1;

    Engine* engine = (Engine*)lua_touserdata(L, 1);
    float x = (float)lua_tonumber(L, 2);
    float y = (float)lua_tonumber(L, 3);
    Vector2 target = { x, y };
    engine->m_mainCamera.target = target;

    return 0;
}

int Engine::wrap_LoadLevel(lua_State* L)
{
    if (lua_gettop(L) != 2) return -1;

    Engine* engine = (Engine*)lua_touserdata(L, 1);
    std::string levelName = lua_tostring(L, 2);
    
    LevelData levelData = engine->LoadLevel(levelName);

    lua_pushinteger(L, levelData.width);
    lua_pushinteger(L, levelData.height);

    int size = levelData.width * levelData.height;
    lua_createtable(L, 0, size);
    for (int i = 0; i < size; ++i) {
        lua_pushinteger(L, (i + 1));
        lua_pushinteger(L, levelData.tiles[i]);
        lua_settable(L, -3);
    }

    return 3;
}

int Engine::wrap_SetCameraOffset(lua_State* L)
{
    if (lua_gettop(L) != 3) return -1;

    Engine* engine = (Engine*)lua_touserdata(L, 1);
    float x = (float)lua_tonumber(L, 2);
    float y = (float)lua_tonumber(L, 3);
    engine->m_mainCamera.offset = { x, y };

    return 0;
}

int Engine::wrap_SetCameraZoom(lua_State* L)
{
    if (lua_gettop(L) != 2) return -1;

    Engine* engine = (Engine*)lua_touserdata(L, 1);
    float zoom = (float)lua_tonumber(L, 2);
    engine->m_mainCamera.zoom = zoom;

    return 0;
}

int Engine::wrap_SetCameraRotation(lua_State* L)
{
    if (lua_gettop(L) != 2) return -1;

    Engine* engine = (Engine*)lua_touserdata(L, 1);
    float rotation = (float)lua_tonumber(L, 2);
    engine->m_mainCamera.rotation = rotation;

    return 0;
}

int Engine::wrap_DrawRectangle(lua_State* L)
{
    if (lua_gettop(L) != 6) return -1;

    Engine* engine = (Engine*)lua_touserdata(L, 1);
    float x = (float)lua_tonumber(L, 2);
    float y = (float)lua_tonumber(L, 3);
    float width = (float)lua_tonumber(L, 4);
    float height = (float)lua_tonumber(L, 5);
    EColor color = (EColor)lua_tointeger(L, 6);

    DrawRectangleRec({ x, y, width, height }, GetColor(color));

    return 0;
}

int Engine::wrap_CheckCollisionRect(lua_State* L)
{
    if (lua_gettop(L) != 9) return -1;

    Engine* engine = (Engine*)lua_touserdata(L, 1);

    if (engine)
    {
        float x = (float)lua_tonumber(L, 2);
        float y = (float)lua_tonumber(L, 3);
        float width = (float)lua_tonumber(L, 4);
        float height = (float)lua_tonumber(L, 5);

        float x2 = (float)lua_tonumber(L, 6);
        float y2 = (float)lua_tonumber(L, 7);
        float width2 = (float)lua_tonumber(L, 8);
        float height2 = (float)lua_tonumber(L, 9);

        bool bCollision = CheckCollisionRecs({ x, y, width, height }, { x2, y2, width2, height2 });
        lua_pushboolean(L, bCollision);
    }
    else {
        lua_pushnil(L);
    }

    return 1;
}

int Engine::wrap_GetMousePosition(lua_State* L)
{
    if (lua_gettop(L) != 1) return -1;

    Engine* engine = (Engine*)lua_touserdata(L, 1);

    Vector2 mouse = GetMousePosition();
    lua_pushnumber(L, mouse.x);
    lua_pushnumber(L, mouse.y);

    return 2;
}

int Engine::wrap_IsMouseButtonDown(lua_State* L)
{
    if (lua_gettop(L) != 2) return -1;

    Engine* engine = (Engine*)lua_touserdata(L, 1);
    int button = (int)lua_tointeger(L, 2);
    bool isDown = IsMouseButtonDown((MouseButton)button);
    lua_pushboolean(L, isDown);

    return 1;
}

int Engine::wrap_IsMouseButtonPressed(lua_State* L)
{
    if (lua_gettop(L) != 2) return -1;

    Engine* engine = (Engine*)lua_touserdata(L, 1);
    int button = (int)lua_tointeger(L, 2);
    bool isPressed = IsMouseButtonPressed((MouseButton)button);
    lua_pushboolean(L, isPressed);

    return 1;
}

int Engine::wrap_GetMouseScreenToWorld2D(lua_State* L)
{
    if (lua_gettop(L) != 1) return -1;

    Engine* engine = (Engine*)lua_touserdata(L, 1);

    Vector2 position = GetScreenToWorld2D(GetMousePosition(), engine->m_mainCamera);

    lua_pushnumber(L, position.x);
    lua_pushnumber(L, position.y);

    return 2;
}

int Engine::wrap_AddToRenderBatch(lua_State* L)
{
    if (lua_gettop(L) != 8) return -1;

    Engine* engine = (Engine*)lua_touserdata(L, 1);
    std::string textureName = lua_tostring(L, 2);
    float x = (float)lua_tonumber(L, 3);
    float y = (float)lua_tonumber(L, 4);
    float width = (float)lua_tonumber(L, 5);
    float height = (float)lua_tonumber(L, 6);
    float posX = (float)lua_tonumber(L, 7);
    float posY = (float)lua_tonumber(L, 8);
 
    TextureRec textRect = { textureName, { x, y, width, height }, { posX, posY } };
    engine->m_renderTextureBatch.push_back(textRect);

    return 0;
}

int Engine::wrap_DrawText(lua_State* L)
{
    if (lua_gettop(L) != 6) return -1;

    Engine* engine = (Engine*)lua_touserdata(L, 1);
    std::string text = lua_tostring(L, 2);
    int x = (int)lua_tointeger(L, 3);
    int y = (int)lua_tointeger(L, 4);
    int fontSize = (int)lua_tointeger(L, 5);
    EColor color = (EColor)lua_tointeger(L, 6);

    DrawText(text.c_str(), x, y, fontSize, GetColor(color));

    return 0;
}

int Engine::wrap_PostRenderText(lua_State* L)
{
    if (lua_gettop(L) != 6) return -1;

    Engine* engine = (Engine*)lua_touserdata(L, 1);
    std::string text = lua_tostring(L, 2);
    int x = (int)lua_tointeger(L, 3);
    int y = (int)lua_tointeger(L, 4);
    int fontSize = (int)lua_tointeger(L, 5);
    EColor color = (EColor)lua_tointeger(L, 6);

    TextData textInfo{ text, x, y, fontSize, GetColor(color)};
    engine->m_renderTextQueue.emplace(textInfo);

    return 0;
}

int Engine::wrap_PlaySound(lua_State* L)
{
    if (lua_gettop(L) != 2) return -1;

    Engine* engine = (Engine*)lua_touserdata(L, 1);
    std::string soundName = lua_tostring(L, 2);

    Sound sound = ResourceManager::Get().GetSound(soundName);
    SetSoundVolume(sound, 2.5);
    PlaySound(sound);

    return 0;
}

int Engine::wrap_PlayMusic(lua_State* L)
{
    if (lua_gettop(L) != 2) return -1;

    Engine* engine = (Engine*)lua_touserdata(L, 1);
    std::string musicName = lua_tostring(L, 2);

    engine->m_music = ResourceManager::Get().GetMusic(musicName);

    if (!IsMusicStreamPlaying(engine->m_music))
    {
        PlayMusicStream(engine->m_music);
    }

    return 0;
}

int Engine::wrap_StopMusic(lua_State* L)
{
    if (lua_gettop(L) != 2) return -1;

    Engine* engine = (Engine*)lua_touserdata(L, 1);
    std::string musicName = lua_tostring(L, 2);

    if (IsMusicStreamPlaying(engine->m_music))
    {
        StopMusicStream(engine->m_music);
    }

    return 0;
}
