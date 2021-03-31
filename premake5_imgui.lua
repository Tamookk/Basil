project "ImGui"
	location "Basil/vendor/imgui"
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
		defines { "IMGUI_API=__declspec(dllexport)" }
		staticruntime "On"

	filter "configurations:Debug"
		runtime "Debug"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		buildoptions "/MD"
		optimize "On"
