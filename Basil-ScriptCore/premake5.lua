project "Basil-ScriptCore"
    kind "SharedLib"
    language "C#"
    dotnetframework "4.7.2"

    targetdir ("../Pesto/Resources/Scripts")
    objdir ("../Pesto/Resources/Scripts/Intermediates")

    files
    {
        "Source/**.cs",
        "Properties/**.cs"
    }

    filter "configurations:Debug"
        optimize "Off"
        symbols "Default"

    filter "configurations:Release"
        optimize "On"
        symbols "Default"