/*
 * This header file declares a panel for content browsing.
 */
#pragma once

#include <filesystem>

namespace Basil
{
	class ContentBrowserPanel
	{
		public:
			ContentBrowserPanel();
			void onImGuiRender();

		private:
			std::filesystem::path currentDirectory;
	};
}