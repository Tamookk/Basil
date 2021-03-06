#include "Panels/PropertiesPanel.h"

#include "Scene/Component.h"
#include "Scripting/ScriptEngine.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_internal.h>

namespace Basil
{
	extern const std::filesystem::path assetPath;

	// Draw vec3 controls (for transform component)
	static void drawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		// Get the bold font
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

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
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		// Y
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		// Z
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f);
		ImGui::PopItemWidth();
		
		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
	}
	
	// Draw component of entity
	template <typename T, typename UIFunction>
	static void drawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		// Set tree node flags
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed 
			| ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		// Check if the entity has the component
		if (entity.hasComponent<T>())
		{
			// Get the component and the region available
			auto& component = entity.getComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			// Push frame padding style, get line height, add a seperator
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();

			// Create a tree node, pop the style
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();

			// Add component settings button to the same line as the component name
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("...", ImVec2{ lineHeight, lineHeight }))
				ImGui::OpenPopup("ComponentSettings");

			// Open component settings menu if button clicked
			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				// Remove component if this clicked
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			// Run the uiFunction function (component-specific code)
			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			// If remove component button was clicked, remove the component
			if (removeComponent)
				entity.removeComponent<T>();
		}
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
		}
		ImGui::End();
	}

	// Templated function for displaying "add component" entries
	template<typename T>
	void PropertiesPanel::displayAddComponentEntry(const std::string& entryName, Entity& entity)
	{
		// Only display entry for component T if the entity does not have it
		if (!entity.hasComponent<T>())
		{
			// If menu item of entry name is clicked
			if (ImGui::MenuItem(entryName.c_str()))
			{
				// Add component T and close the menu
				entity.addComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
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
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		// Draw the add component button
		ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		if (ImGui::Button("+"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			// Draw list of components that can be added to an entity
			displayAddComponentEntry<CameraComponent>("Camera", entity);
			displayAddComponentEntry<ScriptComponent>("Script", entity);
			displayAddComponentEntry<SpriteRendererComponent>("Sprite Renderer", entity);
			displayAddComponentEntry<CircleRendererComponent>("Circle Renderer", entity);
			displayAddComponentEntry<Rigidbody2DComponent>("Rigidbody 2D", entity);
			displayAddComponentEntry<BoxCollider2DComponent>("Box Collider 2D", entity);
			displayAddComponentEntry<CircleCollider2DComponent>("Circle Collider 2D", entity);

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		// Draw each component settings for entity
		drawComponent<TransformComponent>("Transform", entity, [](auto& component)
		{
			// Create drag floats for position, rotation, and scale
			drawVec3Control("Translation", component.translation);
			glm::vec3 rotation = glm::degrees(component.rotation);
			drawVec3Control("Rotation", rotation);
			component.rotation = glm::radians(rotation);
			drawVec3Control("Scale", component.scale, 1.0f);
		});

		drawComponent<CameraComponent>("Camera", entity, [](auto& component)
		{
			auto& camera = component.camera;

			// Create a checkbox for primary camera
			ImGui::Checkbox("Primary", &component.primary);

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
						camera.setCameraType((SceneCamera::CameraType)i);
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

				ImGui::Checkbox("Fixed Aspect Ratio", &component.fixedAspectRatio);
			}
		});

		drawComponent<ScriptComponent>("Script", entity, [](auto& component)
		{
				// Determine whether the script name exists
				bool scriptClassExists = ScriptEngine::entityClassExists(component.className);

				// Copy the name of the script into a buffer
				static char buffer[64];
				strcpy(buffer, component.className.c_str());

				// Push a red color if the script does not exist
				if (!scriptClassExists)
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.3f, 1.0f));

				// Set component class name to buffer text
				if (ImGui::InputText("Class", buffer, sizeof(buffer)))
					component.className = buffer;

				if (!scriptClassExists)
					ImGui::PopStyleColor();
		});

		drawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(component.color));

			// Texture drag and drop
			ImGui::Button("Texture", ImVec2(100.0f, 0.0f));
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::filesystem::path texturePath = std::filesystem::path(assetPath) / path;
					Shared<Texture2D> texture = Texture2D::create(texturePath.string());
					if (texture->isTextureLoaded())
						component.texture = texture;
					else
						LOG_WARN("Could not load texture {0}", texturePath.filename().string());
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::DragFloat("Tiling Factor", &component.tilingFactor, 0.1f, 0.0f, 100.0f);
		});

		drawComponent<CircleRendererComponent>("Circle Renderer", entity, [](auto& component)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(component.color));
			ImGui::DragFloat("Thickness", &component.thickness, 0.025f, 0.0f, 1.0f);
			ImGui::DragFloat("Fade", &component.fade, 0.00025f, 0.0f, 1.0f);
		});

		drawComponent<Rigidbody2DComponent>("Rigidbody 2D", entity, [](auto& component)
		{
				const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
				const char* currentBodyTypeString = bodyTypeStrings[(int)component.type];
				if (ImGui::BeginCombo("Type", currentBodyTypeString))
				{
					for (int i = 0; i < 3; i++)
					{
						bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
						if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
						{
							currentBodyTypeString = bodyTypeStrings[i];
							component.type = (Rigidbody2DComponent::BodyType)i;
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				ImGui::Checkbox("Fixed Rotation", &component.fixedRotation);
		});

		drawComponent<BoxCollider2DComponent>("Box Collider 2D", entity, [](auto& component)
		{
			ImGui::DragFloat2("Offset", glm::value_ptr(component.offset));
			ImGui::DragFloat2("Size", glm::value_ptr(component.size));
			ImGui::DragFloat("Density", &component.density, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Friction", &component.friction, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution", &component.restitution, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution Threshold", &component.restitutionThreshold, 0.01f, 0.0f);
		});

		drawComponent<CircleCollider2DComponent>("Circle Collider 2D", entity, [](auto& component)
		{
			ImGui::DragFloat2("Offset", glm::value_ptr(component.offset));
			ImGui::DragFloat("Radius", &component.radius, 0.1f, 0.5f, 10.0f);
			ImGui::DragFloat("Density", &component.density, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Friction", &component.friction, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution", &component.restitution, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution Threshold", &component.restitutionThreshold, 0.01f, 0.0f);
		});
	}
}