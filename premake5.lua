workspace "MarblingSimulation"
	architecture "x64"
	startproject "Simulation"

	configurations
	{
		"Debug",
		"Release"
	}

	flags
	{
		"MultiProcessorCompile"
	}
	

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- include directories
IncludeDir = {}
IncludeDir["GLFW"] = "RenderEngine/dep/GLFW/include"
IncludeDir["GLEW"] = "RenderEngine/dep/GLEW/include"
IncludeDir["GLM"] = "RenderEngine/dep/GLM/include"
IncludeDir["SPDLOG"] = "RenderEngine/dep/spdlog/include"
IncludeDir["ImGui"] = "RenderEngine/dep/ImGui",

include "RenderEngine/dep/GLFW"

project "RenderEngine"
	location "RenderEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLEW}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.SPDLOG}",
		"%{IncludeDir.ImGui}",
		"%{prj.name}/dep/stb_image",
		"%{prj.name}/src"
	}

	libdirs 
	{
		"%{prj.name}/dep/GLEW/lib/Release/x64",
		"%{prj.name}/dep/GLM/lib",
		"%{prj.name}/dep/ImGui/lib"
	}

	links
	{
		"GLFW",
		"opengl32.lib",
		"glm_static.lib",
		"glew32s.lib",
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
			"GLEW_STATIC"
		}

		postbuildcommands
		{
			--("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Simulation")
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
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLEW}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.SPDLOG}",
		"%{IncludeDir.ImGui}",
		"RenderEngine/src",
		"FluidLib/src"
	}

	links
	{
		"RenderEngine",
		"FluidLib"
	}

	filter "configurations:Debug"
		defines "DEBUG"
		symbols "On"
		buildoptions "/MDd"

	filter "configurations:Release"
		defines "RELEASE"
		optimize "On"
		buildoptions "/MD"

project "FluidLib"
	location "FluidLib"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}
	
	includedirs
	{
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLEW}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.SPDLOG}",
		"%{prj.name}/src"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "DEBUG"
		symbols "On"
		buildoptions "/MDd"

	filter "configurations:Release"
		defines "RELEASE"
		optimize "On"
		buildoptions "/MD"