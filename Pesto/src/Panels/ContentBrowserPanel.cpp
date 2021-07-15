#include "Panels/ContentBrowserPanel.h"

#include <imgui.h>

namespace Basil
{
	// Temporary
	constexpr char* assetPath = "assets";

	// Constructor
	ContentBrowserPanel::ContentBrowserPanel()
	{
		// Initialise variables
		currentDirectory = assetPath;
	}

	// On ImGui render
	void ContentBrowserPanel::onImGuiRender()
	{
		// Create and render the Content Browser panel
		ImGui::Begin("Content Browser");

		// Render a back button if we are not in the root assets directory
		if (currentDirectory != std::filesystem::path(assetPath))
		{
			if (ImGui::Button("<-"))
			{
				currentDirectory = currentDirectory.parent_path();
			}
		}

		// For each top-level item in the current directory
		for (auto& directoryEntry : std::filesystem::directory_iterator(currentDirectory))
		{
			// Get the path, relative path, and file name of the item
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, assetPath);
			std::string fileNameString = relativePath.filename().string();

			if (directoryEntry.is_directory())
			{
				// Enter directory if button is clicked
				if (ImGui::Button(fileNameString.c_str()))
				{
					currentDirectory /= path.filename();
				}
			}
			else
			{
				if(ImGui::Button(fileNameString.c_str()))
				{ }
			}

			
		}

		ImGui::End();
	}
}