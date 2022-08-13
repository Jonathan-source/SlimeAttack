project "Editor"
	language "C++"
	kind "ConsoleApp"
	
	location "../Build/Generated"
	buildlog "../Build/Logs/GameBuildLog.log"
	basedir "../../../"
	objdir "../Build/Intermediate/%{cfg.platform}_%{cfg.buildcfg}"
	targetdir "../Build/Bin/%{cfg.platform}_%{cfg.buildcfg}"
	
	targetname "Editor"
	
	pchheader "pch.h"
	pchsource "Source/pch.cpp"

	includedirs { 
		"Source",
		"../ThirdParty/lua542/include",
		"../ThirdParty/raylib/include"
	}

	libdirs "../ThirdParty/raylib/"

	links {
        "raylib.lib",
        "winmm.lib"
    }

	-- General Build
		
	filter {}
		characterset "Unicode"
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
				
	-- General Links
	
	-- Specific Links
	
	filter { "platforms:Win64" }
		links { "Synchronization" }
						