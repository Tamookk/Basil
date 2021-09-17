project "Box2D"
	location "Basil/vendor/"
	kind "StaticLib"
	language "C++"
	box2ddir = "Basil/vendor/box2d/"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		(box2ddir .. "src/**.cpp"),
		(box2ddir .. "src/**.h"),
		(box2ddir .. "include/**.h")
	}

	includedirs { box2ddir .. "/include", box2ddir .. "/src"}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
