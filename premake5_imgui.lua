project "ImGui"
	location "Basil/vendor/"
	kind "StaticLib"
	language "C++"
	imguidir = "Basil/vendor/imgui/"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	files
	{
		(imguidir .. "imconfig.h"),
		(imguidir .. "imgui.cpp"),
		(imguidir .. "imgui.h"),
		(imguidir .. "imgui_demo.cpp"),
		(imguidir .. "imgui_draw.cpp"),
		(imguidir .. "imgui_internal.h"),
		(imguidir .. "imgui_tables.cpp"),
		(imguidir .. "imgui_widgets.cpp"),
		(imguidir .. "imstb_retpack.h"),
		(imguidir .. "imstb_textedit.h"),
		(imguidir .. "imstb_truetype.h")
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
