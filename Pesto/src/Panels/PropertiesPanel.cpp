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
		// Tag component
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

		// Transform component
		if (entity.hasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				// Create drag floats for position, rotation, and scale
				auto& position = entity.getComponent<TransformComponent>().position;
				auto& rotation = entity.getComponent<TransformComponent>().rotation;
				auto& scale = entity.getComponent<TransformComponent>().scale;
				ImGui::DragFloat3("Position", glm::value_ptr(position), 0.5f);
				ImGui::DragFloat("Rotation", &rotation, 1.0f);
				ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.5f);
				ImGui::TreePop();
			}
		}

		// Camera component
		if (entity.hasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				// Get camera component and camera
				auto& cameraComponent = entity.getComponent<CameraComponent>();
				auto& camera = cameraComponent.camera;

				// Create a checkbox for primary camera
				ImGui::Checkbox("Primary", &cameraComponent.primary);

				// Specifiy camera types and get the current camera type
				const char* cameraTypes[] = { "Perspective", "Orthographic" };
				const char* currentCameraTypeString = cameraTypes[(int)camera.getCameraType()];

				// Combobox for selecting camera type
				if (ImGui::BeginCombo("Camera Type", currentCameraTypeString))
				{
					// Iterate through camera types
					for (int i = 0; i < 2; i++)
					{
						// If the camera type selected changes, reflect this change
						bool isSelected = currentCameraTypeString == cameraTypes[i];
						if (ImGui::Selectable(cameraTypes[i], isSelected))
						{
							currentCameraTypeString = cameraTypes[i];
							cameraComponent.camera.setCameraType((SceneCamera::CameraType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				// If the camera type selected is perspective
				if (camera.getCameraType() == SceneCamera::CameraType::Perspective)
				{
					float verticalFOV = camera.getPerspectiveVerticalFOV();
					if (ImGui::DragFloat("FOV", &verticalFOV))
						camera.setPerspectiveVerticalFOV(verticalFOV);

					float persNear = camera.getPerspectiveNearClip();
					if (ImGui::DragFloat("Near Clip", &persNear))
						camera.setPerspectiveNearClip(persNear);

					float persFar = camera.getPerspectiveFarClip();
					if (ImGui::DragFloat("Far Clip", &persFar))
						camera.setPerspectiveFarClip(persFar);
				}

				// If the camera type selected is orthographic
				if (camera.getCameraType() == SceneCamera::CameraType::Orthographic)
				{
					float orthoSize = camera.getOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize))
						camera.setOrthographicSize(orthoSize);

					float orthoNear = camera.getOrthographicNearClip();
					if (ImGui::DragFloat("Near Clip", &orthoNear))
						camera.setOrthographicNearClip(orthoNear);

					float orthoFar = camera.getOrthographicFarClip();
					if (ImGui::DragFloat("Far Clip", &orthoFar))
						camera.setOrthographicFarClip(orthoFar);

					ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.fixedAspectRatio);
				}

				ImGui::TreePop();
			}
		}

		if (entity.hasComponent<SpriteRenderComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(SpriteRenderComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Render"))
			{
				auto& color = entity.getComponent<SpriteRenderComponent>().color;
				ImGui::ColorEdit4("Color", glm::value_ptr(color));
				ImGui::TreePop();
			}
		}
	}
}