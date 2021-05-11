#include "Panels/SceneHierarchyPanel.h"

#include "Scene/Component.h"

#include <glm/gtc/type_ptr.hpp>
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

	// Return the selection context
	Entity& SceneHierarchyPanel::getSelectionContext()
	{
		return selectionContext;
	}

	// On ImGui render
	void SceneHierarchyPanel::onImGuiRender()
	{
		// Create and render the scene hierarchy panel
		ImGui::Begin("Scene Hierarchy");
		context->registry.each([&](auto entityID)
		{
			Entity entity{ entityID, context.get() };
			drawEntityNode(entity);
		});

		// Clear selection when clicking on a blank space
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			selectionContext = {};

		// Let user create entity when right clicking on a blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				context->createEntity("Empty Entity");

			ImGui::EndPopup();
		}

		ImGui::End();
	}

	// Draw an entity node
	void SceneHierarchyPanel::drawEntityNode(Entity entity)
	{
		// Get the name of the entity
		auto tag = entity.getComponent<TagComponent>().tag;
		
		// Set flags
		ImGuiTreeNodeFlags flags = ((selectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			selectionContext = entity;
		}

		// Allow user to delete entity if right clicking on one
		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

		// Delete the entity
		if (entityDeleted)
		{
			context->destroyEntity(entity);
			if (selectionContext == entity)
				selectionContext = {};
		}
	}
}