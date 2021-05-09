#include "Panels/PropertiesPanel.h"

#include "Scene/Component.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace Basil
{
	// Constructor
	PropertiesPanel::PropertiesPanel(const Shared<Scene>& context)
	{
		setContext(context);
	}

	// Set the context
	void PropertiesPanel::setContext(const Shared<Scene>& context)
	{
		this->context = context;
	}

	// On ImGui render
	void PropertiesPanel::onImGuiRender(Entity& selectionContext)
	{
		// Create and render the properties panel
		ImGui::Begin("Properties");
		if (selectionContext)
		{
			drawComponents(selectionContext);
		}
		ImGui::End();
	}

	// Draw the components for a given entity
	void PropertiesPanel::drawComponents(Entity entity)
	{
		if (entity.hasComponent<TagComponent>())
		{
			// Get the tag
			auto& tag = entity.getComponent<TagComponent>().tag;

			// Create and initialise a string buffer
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());

			// Create a text input field and assign its contents to the tag component's tag
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		if (entity.hasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform Component"))
			{
				auto& position = entity.getComponent<TransformComponent>().position;
				auto& rotation = entity.getComponent<TransformComponent>().rotation;
				auto& scale = entity.getComponent<TransformComponent>().scale;
				ImGui::DragFloat3("Position", glm::value_ptr(position), 0.5f);
				ImGui::DragFloat("Rotation", &rotation, 1.0f);
				ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.5f);
				ImGui::TreePop();
			}
		}
	}
}