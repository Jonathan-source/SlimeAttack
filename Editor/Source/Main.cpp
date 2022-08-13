#include "pch.h"
#include "Editor.h"

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

    Editor* editor = new Editor(800, 600, projectPath.string());

    editor->Start();

    delete editor;

    return 0;
}