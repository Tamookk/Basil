workspace "Basil"
	architecture "x64"
	configurations { "Debug", "Release" }
	startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "premake5_glfw.lua"

project "Basil"
	location "Basil"
	kind "SharedLib"
	language "C++"
	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")
	pchheader "pch.h"
	pchsource "Basil/src/pch.cpp"
	files { "%{prj.name}/src/**.cpp", "%{prj.name}/include/**.h" }
	includedirs
	{
		"%{prj.name}/include",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/GLFW/include"
	}
	links { "GLFW", "opengl32.lib" }
	
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		defines { "PLATFORM_WINDOWS", "BUILD_DLL" }
		postbuildcommands {"{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox"}

	filter "configurations:Debug"
		defines { "DEBUG" }
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines { "RELEASE" }
		buildoptions "/MD"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")
	files { "%{prj.name}/src/**.cpp", "%{prj.name}/include/**.h" }
	includedirs
	{
		"%{prj.name}/include",
		"Basil/include",
		"Basil/vendor/spdlog/include",
		"Basil/vendor/GLFW/include"
	}
	links { "Basil" }

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		defines { "PLATFORM_WINDOWS" }

	filter "configurations:Debug"
		defines { "DEBUG" }
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines { "RELEASE" }
		buildoptions "/MD"
		optimize "On"