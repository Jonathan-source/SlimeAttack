#include "pch.h"
#include "Engine.h"
#include "Editor.h"

#define EDITOR

int main(int argc, char* argv[])
{
#if defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    std::filesystem::path pathToShow{ std::filesystem::current_path() };
    std::filesystem::path projectPath;
    for (const auto& part : pathToShow) {
        projectPath /= part;
        if (part == "SlimeAttack") break;
    }

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

#ifdef EDITOR
    Editor* editor = new Editor(1920, 1080, projectPath.string());
    editor->Start();
    delete editor;
#else
    Engine * engine = new Engine(L, projectPath.string());
    if (engine->Preload())
    {
        if (engine->Initialize())
        {
            engine->Start();
        }
    }
    delete engine;
#endif

    DumpStack(L);

    lua_close(L);

    return 0;
}