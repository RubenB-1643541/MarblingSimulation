workspace "MarblingSimulation"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}
	
	startproject "Simulation"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "RenderEngine"
	location "RenderEngine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "RenderEngine/src/pch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/dep/GLEW/include",
		"%{prj.name}/dep/GLFW/include",
		"%{prj.name}/dep/GLM/include",
		"%{prj.name}/dep/ImGui",
		"%{prj.name}/dep/stb_image",
		"%{prj.name}/src"
	}

	libdirs 
	{
		"%{prj.name}/dep/GLEW/lib/Release/x64",
		"%{prj.name}/dep/GLFW/lib64-vc2019",
		"%{prj.name}/dep/GLM/lib",
		"%{prj.name}/dep/ImGui/lib"
	}

	links
	{
		"opengl32.lib",
		"glm_static.lib",
		"glew32s.lib",
		"glfw3.lib",
		"ImGui.lib",
		"kernel32.lib",
		"user32.lib",
		"gdi32.lib",
		"winspool.lib",
		"comdlg32.lib",
		"advapi32.lib",
		"shell32.lib",
		"ole32.lib",
		"oleaut32.lib",
		"uuid.lib",
		"odbc32.lib",
		"odbccp32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"PLATFORM_WINDOWS",
			"MAKEDLL",
			"GLEW_STATIC"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Simulation")
		}

	filter "configurations:Debug"
		defines "DEBUG"
		symbols "On"
		buildoptions "/MDd"

	filter "configurations:Release"
		defines "RELEASE"
		optimize "On"
		buildoptions "/MD"

project "Simulation"
	location "Simulation"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}
	
	includedirs
	{
		"RenderEngine/src"
	}

	links
	{
		"RenderEngine.dll"
	}

