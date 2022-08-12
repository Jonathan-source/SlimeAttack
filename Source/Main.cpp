#include "pch.h"
#include "Engine.h"
#include "Editor.h"

//#define EDITOR

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

    bool isRunning = true;
    do 
    {
        std::string input = "";
        std::cout << "'game', 'editor', 'exit'" << std::endl;
        std::getline(std::cin, input);

        if (input == "game")
        {
            lua_State* L = luaL_newstate();
            luaL_openlibs(L);

            Engine* engine = new Engine(L, projectPath.string());
            if (engine->Preload())
            {
                if (engine->Initialize())
                {
                    engine->Start();
                }
            }
            delete engine;

            DumpStack(L);

            lua_close(L);
        }
        else if (input == "editor")
        {
            Editor* editor = new Editor(800, 600, projectPath.string());
            editor->Start();
            delete editor;
        }
        else if (input == "exit")
        {
            isRunning = false;
        }
    } while (isRunning);

    return 0;
}