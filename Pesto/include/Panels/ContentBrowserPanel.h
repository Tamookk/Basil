/*
 * This header file declares a panel for content browsing.
 */
#pragma once

#include <filesystem>

#include "Renderer/Texture2D.h"

namespace Basil
{
	class ContentBrowserPanel
	{
		public:
			ContentBrowserPanel();
			void onImGuiRender();
		private:
			std::filesystem::path currentDirectory;
			Shared<Texture2D> directoryIcon;
			Shared<Texture2D> fileIcon;
	};
}