workspace "Basil"
	architecture "x64"
	configurations { "Debug", "Release" }
	startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "premake5_glfw.lua"
	include "premake5_glad.lua"
	include "premake5_imgui.lua"
group ""

project "Basil"
	location "Basil"
	kind "SharedLib"
	language "C++"
	staticruntime "Off"
	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")
	pchheader "pch.h"
	pchsource "Basil/src/pch.cpp"
	files
	{
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/include/**.h",
		"%{prj.name}/vendor/glm/**.hpp",
		"%{prj.name}/vendor/glm/**.inl"
	}
	includedirs
	{
		"%{prj.name}/include",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/GLFW/include",
		"%{prj.name}/vendor/glad/include",
		"%{prj.name}/vendor/imgui",
		"%{prj.name}/vendor/glm/"
	}
	links { "GLFW", "GLAD", "ImGui", "opengl32.lib" }
	
	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		defines { "PLATFORM_WINDOWS", "BUILD_DLL" }
		postbuildcommands {"{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox\""}

	filter "configurations:Debug"
		defines { "DEBUG" }
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines { "RELEASE" }
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "Off"
	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")
	files { "%{prj.name}/src/**.cpp", "%{prj.name}/include/**.h" }
	includedirs
	{
		"%{prj.name}/include",
		"Basil/include",
		"Basil/vendor/spdlog/include",
		"Basil/vendor/GLFW/include",
		"Basil/vendor/glad/include",
		"Basil/vendor/imgui",
		"Basil/vendor/glm"
	}
	links { "Basil" }

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		defines { "PLATFORM_WINDOWS" }

	filter "configurations:Debug"
		defines { "DEBUG" }
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines { "RELEASE" }
		runtime "Release"
		optimize "On"