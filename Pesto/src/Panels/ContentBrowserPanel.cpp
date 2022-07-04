#include "Panels/ContentBrowserPanel.h"

#include <imgui.h>

namespace Basil
{
	// Temporary
	extern const std::filesystem::path assetPath = "assets";

	// Constructor
	ContentBrowserPanel::ContentBrowserPanel()
	{
		// Initialise variables
		currentDirectory = assetPath;
		directoryIcon = Texture2D::create("res/icons/content_browser/folder.png");
		fileIcon = Texture2D::create("res/icons/content_browser/file.png");
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

		// Calculate number of columns in content browser
		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;
		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		
		// Ensure there is always at least one column
		if (columnCount < 1)
			columnCount = 1;

		// Set column count
		ImGui::Columns(columnCount, 0, false);

		// For each top-level item in the current directory
		for (auto& directoryEntry : std::filesystem::directory_iterator(currentDirectory))
		{
			// Get the path, relative path, and file name of the item
			const auto& path = directoryEntry.path();
			std::string fileNameString = path.filename().string();

			// Push an ImGui ID
			ImGui::PushID(fileNameString.c_str());

			// Get the icon and create a button with the icon
			Shared<Texture2D> icon = directoryEntry.is_directory() ? directoryIcon : fileIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((ImTextureID)icon->getRendererID(), { thumbnailSize, thumbnailSize }, { 0,1 }, { 1,0 });

			// Send drag and drop data to the viewport (currently a scene file)
			if (ImGui::BeginDragDropSource())
			{
				auto relativePath = std::filesystem::relative(path, assetPath);
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();

			// Enter the directory if the directory button is double clicked
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
					currentDirectory /= path.filename();
			}

			// Add the name of the directory/file under the button
			ImGui::TextWrapped(fileNameString.c_str());
			ImGui::NextColumn();
			ImGui::PopID();
		}

		// Reset column count
		ImGui::Columns(1);

		// Add sliders for setting thumbnail size and padding
		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 32);

		ImGui::End();
	}
}