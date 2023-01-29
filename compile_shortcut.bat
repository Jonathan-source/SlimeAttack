ThirdParty\Premake\premake5.exe vs2022

if not defined DevEnvDir (
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"
)

set solutionFile="Workspace.sln"
msbuild /t:Build /p:Configuration=Debug /p:Platform=Win64 %solutionFile%

PAUSE
