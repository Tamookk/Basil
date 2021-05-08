#include "Panels/SceneHierarchyPanel.h"

#include "Scene/Component.h"

#include <imgui.h>

namespace Basil
{
	// Constructor
	SceneHierarchyPanel::SceneHierarchyPanel(const Shared<Scene>& context)
	{
		setContext(context);
	}

	// Set the context
	void SceneHierarchyPanel::setContext(const Shared<Scene>& context)
	{
		this->context = context;
	}

	// On ImGui render
	void SceneHierarchyPanel::onImGuiRender()
	{
		// Create and render the ImGui panel
		ImGui::Begin("Scene Hierarchy");
		context->registry.each([&](auto entityID)
		{
			Entity entity{ entityID, context.get() };
			drawEntityNode(entity);
		});
		ImGui::End();
	}

	// Draw an entity node
	void SceneHierarchyPanel::drawEntityNode(Entity entity)
	{
		// Get the name of the entity
		auto tag = entity.getComponent<TagComponent>().tag;
		
		//
		ImGuiTreeNodeFlags flags = ((selectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			selectionContext = entity;
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}
	}
}