project "GLAD"
	location "Basil/vendor/glad"
	kind "StaticLib"
	language "C"
	staticruntime "On"
	gladdir = "Basil/vendor/glad/"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		(gladdir .. "include/glad/glad.h"),
		(gladdir .. "include/KHR/khrplatform.h"),
		(gladdir .. "src/glad.c")
	}

	includedirs {gladdir .. "include"}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
