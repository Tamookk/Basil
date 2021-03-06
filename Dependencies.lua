
-- Basil Dependencies

VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["spdlog"] = "%{wks.location}/Basil/vendor/spdlog/include"
IncludeDir["GLFW"] = "%{wks.location}/Basil/vendor/GLFW/include"
IncludeDir["glad"] = "%{wks.location}/Basil/vendor/glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Basil/vendor/imgui"
IncludeDir["GLM"] = "%{wks.location}/Basil/vendor/glm"
IncludeDir["stb"] = "%{wks.location}/Basil/vendor/stb"
IncludeDir["EnTT"] = "%{wks.location}/Basil/vendor/entt"
IncludeDir["ImGuizmo"] = "%{wks.location}/Basil/vendor/ImGuizmo"
IncludeDir["yaml_cpp"] = "%{wks.location}/Basil/vendor/yaml-cpp/include"
IncludeDir["Box2D"] = "%{wks.location}/Basil/vendor/box2d/include"
IncludeDir["mono"] = "%{wks.location}/Basil/vendor/mono/include"
IncludeDir["SPIRV_Cross"] = "%{wks.location}/Basil/vendor/SPIRV-Cross"
IncludeDir["shaderc"] = "%{wks.location}/Basil/vendor/shaderc/include"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

LibraryDir = {}
LibraryDir["mono"] = "%{wks.location}/Basil/vendor/mono/lib/%{cfg.buildcfg}"
LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
LibraryDir["VulkanSDK_Debug"] = "%{wks.location}/Basil/vendor/VulkanSDK/Lib"
LibraryDir["VulkanSDK_DebugDLL"] = "%{wks.location}/Basil/vendor/VulkanSDK/Bin"

Library = {}
Library["mono"] = "%{LibraryDir.mono}/libmono-static-sgen.lib"

Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"

-- Windows
Library["WinSock"] = "Ws2_32.lib"
Library["WinMM"] = "Winmm.lib"
Library["WinVersion"] = "Version.lib"
Library["BCrypt"] = "Bcrypt.lib"