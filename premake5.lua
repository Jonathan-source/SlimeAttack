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

function IncludeThirdParty()
	includedirs { 
		"ThirdParty/lua542/include",
		"ThirdParty/raylib/include",
		"ThirdParty/entt"
	}
end

function LinkThirdParty()
	libdirs "ThirdParty/raylib/"

	links {
        "raylib.lib",
        "winmm.lib"
    }
end

function RunThirdParty()
	group "ThirdParty"
end

workspace "DV1570"
	architecture "x86_64"
	platforms { "Win64" }
	configurations { "Debug", "Release" }
	startproject "SuperMario"
	
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
	
project "SlimeAttack"
	language "C++"
	kind "ConsoleApp"
	
	location "Build/Generated"
	buildlog "Build/Logs/GameBuildLog.log"
	basedir "../../"
	objdir "Build/Intermediate/%{cfg.platform}_%{cfg.buildcfg}"
	targetdir "Build/Bin/%{cfg.platform}_%{cfg.buildcfg}"
	
	targetname "SlimeAttack"
	
	pchheader "pch.h"
	pchsource "Source/pch.cpp"

	includedirs { "Source" }
	
	-- General Build
		
	filter {}
		--flags { "NoPCH" }
		--clr "Off"
		--rtti "Off"
		--characterset "Unicode"
		staticruntime "Off"
		warnings "Default"
						
		-- Extra third party defines
		defines {
			--"IMGUI_DEFINE_MATH_OPERATORS",
		}
		
	-- Specific Build
		
	filter { "platforms:Win64" }
		system "Windows"
	
	postbuildcommands {
		"{COPYFILE} %{wks.location}ThirdParty/lua542/lua54.dll %{wks.location}Build/Bin/%{cfg.platform}_%{cfg.buildcfg}/",
		"{COPYFILE} %{wks.location}ThirdParty/raylib/raylib.dll %{wks.location}Build/Bin/%{cfg.platform}_%{cfg.buildcfg}/"
	}		

	-- Configurations
		
	filter { "configurations:Debug" }
		symbols "On"
		optimize "Off"
		omitframepointer "Off"
		exceptionhandling "On"

	filter { "configurations:Release" }
		flags { "LinkTimeOptimization", "NoRuntimeChecks" }
		symbols "Off"
		optimize "Speed"  -- /O2 instead of /Ox on MSVS with "Full".
		omitframepointer "On"
		exceptionhandling "Off"     

	-- General Files
		
	filter {}
		files { "Source/*.h", "Source/*.cpp" }

	filter {}
				
	-- Third Party Files
	
	IncludeThirdParty()
	
	-- General Links
	
	libdirs {}
	
	links { }
	
	-- Specific Links
	
	filter { "platforms:Win64" }
		links { "Synchronization" }
						
	filter {}
		
	-- Third Party Links
	
	LinkThirdParty()
	
	-- Run Third Party Build Scripts
	
	RunThirdParty()