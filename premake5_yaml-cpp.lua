project "yaml-cpp"
	location "Basil/vendor/"
	kind "StaticLib"
	language "C++"
	staticruntime "On"
	yamlcppdir = "Basil/vendor/yaml-cpp/"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		(yamlcppdir .. "src/**.cpp"),
		(yamlcppdir .. "src/**.h"),
		(yamlcppdir .. "include/**.h")
	}

	includedirs { yamlcppdir .. "/include"}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
