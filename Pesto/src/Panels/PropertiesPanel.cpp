#include "Panels/PropertiesPanel.h"

#include "Scene/Component.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_internal.h>

namespace Basil
{
	// Draw vec3 controls (for transform component)
	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		// X
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		// Y
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		// Z
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f);
		ImGui::PopItemWidth();
		
		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
	}
	
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

			// A button to add a component
			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("Add Component");

			if(ImGui::BeginPopup("Add Component"))
			{
				if (ImGui::MenuItem("Camera"))
				{
					selectionContext.addComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Sprite Render"))
				{
					selectionContext.addComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
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

		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

		// Transform component
		if (entity.hasComponent<TransformComponent>())
		{
			bool open = ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treeNodeFlags, "Transform");
			if (open)
			{
				// Create drag floats for position, rotation, and scale
				auto& translation = entity.getComponent<TransformComponent>().translation;
				auto& rotation = entity.getComponent<TransformComponent>().rotation;
				auto& scale = entity.getComponent<TransformComponent>().scale;
				DrawVec3Control("Translation", translation);
				DrawVec3Control("Rotation", rotation);
				DrawVec3Control("Scale", scale, 1.0f);
				ImGui::TreePop();
			}
		}

		// Camera component
		if (entity.hasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), treeNodeFlags, "Camera"))
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

		if (entity.hasComponent<SpriteRendererComponent>())
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 0, 0 });
			bool open = ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), treeNodeFlags, "Sprite Render");
			ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
			if (ImGui::Button("+", ImVec2{ 20, 20 }))
			{
				ImGui::OpenPopup("Component Settings");
			}
			ImGui::PopStyleVar();

			bool removeComponent = false;
			if (ImGui::BeginPopup("Component Settings"))
			{
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;
				ImGui::EndPopup();
			}

			if (open)
			{
				auto& color = entity.getComponent<SpriteRendererComponent>().color;
				ImGui::ColorEdit4("Color", glm::value_ptr(color));
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.removeComponent<SpriteRendererComponent>();
		}
	}
}