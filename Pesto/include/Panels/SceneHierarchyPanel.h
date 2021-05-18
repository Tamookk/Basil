/*
 * This header file declares a panel for displaying the hierarchy of objects within a scene.
 */
#pragma once

#include "Core/Core.h"
#include "Core/Log.h"
#include "Scene/Entity.h"
#include "Scene/Scene.h"

namespace Basil
{
	class SceneHierarchyPanel
	{
		public:
			SceneHierarchyPanel() = default;
			SceneHierarchyPanel(const Shared<Scene>& context);
			void setContext(const Shared<Scene>& context);
			Entity& getSelectedEntity();
			void setSelectedEntity(Entity entity);
			void onImGuiRender();
		private:
			void drawEntityNode(Entity entity);
			Shared<Scene> context;
			Entity selectionContext;
	};
}