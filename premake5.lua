newaction {
	trigger = "clean",
	description = "Remove all binaries and intermediate binaries, and vs files.",
    execute = function()
        print("Removing binaries")
        os.rmdir("Build/Bin")

        print("Removing intermediate binaries")
        os.rmdir("Build/Intermediate")

        print("Removing project files")
        os.rmdir("Build/Generated")
        os.rmdir("./.vs")
        os.remove("**.sln")
        os.remove("**.vcxproj")
        os.remove("**.vcxproj.filters")
        os.remove("**.vcxproj.user")
        print("Done")
    end
}

workspace "Workspace"
	architecture "x86_64"
	platforms { "Win64" }
	configurations { "Debug", "Release" }
	startproject "Engine"

	-- General Settings
		
	cppdialect "C++17"
	
	-- Global Platform
	
	flags { "MultiProcessorCompile" }
	
	filter { "platforms:Win64" }
		defines { "WIN32", "_WIN32", "_CRT_SECURE_NO_WARNINGS" }
		systemversion "latest"
	
	-- Global Configurations
	
	filter { "configurations:Debug" }
		defines { "DEBUG", "_DEBUG" }
		symbols "On"
		optimize "Off"

	filter { "configurations:Release" }
		defines { "NDEBUG", "_UNICODE", "UNICODE" }
		symbols "Off"
		optimize "Speed"


include "Engine"
include "Editor"
