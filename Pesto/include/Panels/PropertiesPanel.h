/*
 * This header file declares a panel for displaying the properties of entities within a scene.
 */
#pragma once

#include "Core/Core.h"
#include "Core/Log.h"
#include "Scene/Entity.h"
#include "Scene/Scene.h"

namespace Basil
{
	class PropertiesPanel
	{
	public:
		PropertiesPanel() = default;
		PropertiesPanel(const Shared<Scene>& context);
		void setContext(const Shared<Scene>& context);
		void onImGuiRender(Entity& selectionContext);
	private:
		void drawComponents(Entity entity);
		Shared<Scene> context;
		Entity selectionContext;
	};
}