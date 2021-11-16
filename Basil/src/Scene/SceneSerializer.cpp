#include "pch.h"

#include <fstream>

#include "Scene/Component.h"
#include "Scene/Entity.h"
#include "Scene/SceneSerializer.h"

#include <yaml-cpp/yaml.h>

namespace YAML
{
	// glm::vec2 conversion
	template <>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	// glm::vec3 conversion
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	// glm::vec4 conversion
	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Basil
{
	// glm::vec2 operator
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	// glm::vec3 operator
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	// glm::vec4 operator
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	// Rigidbody 2D type to string
	static std::string Rigidbody2DTypeToString(Rigidbody2DComponent::BodyType type)
	{
		switch (type)
		{
			case Rigidbody2DComponent::BodyType::Static:	return "Static";
			case Rigidbody2DComponent::BodyType::Dynamic:	return "Dynamic";
			case Rigidbody2DComponent::BodyType::Kinematic: return "Kinematic";
		}

		ASSERT(false, "");
		return {};
	}

	// Rigidbody 2D type from string
	static Rigidbody2DComponent::BodyType Rigidbody2DTypeFromString(const std::string& typeString)
	{
		if (typeString == "Static")		return Rigidbody2DComponent::BodyType::Static;
		if (typeString == "Dynamic")	return Rigidbody2DComponent::BodyType::Dynamic;
		if (typeString == "Kinematic")	return Rigidbody2DComponent::BodyType::Kinematic;

		ASSERT(false, "");
		return Rigidbody2DComponent::BodyType::Static;
	}

	// Constructor
	SceneSerializer::SceneSerializer(const Shared<Scene>& scene)
	{
		this->scene = scene;
	}

	// Serialize an entity and its components
	static void serializeEntity(YAML::Emitter& out, Entity entity)
	{
		ASSERT(entity.hasComponent<IDComponent>(), "");

		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << entity.getUUID();	// TODO: entity ID

		// Tag component
		if (entity.hasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;

			auto& tag = entity.getComponent<TagComponent>().tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap;
		}

		// Transform component
		if (entity.hasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			auto& tc = entity.getComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tc.translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.scale;

			out << YAML::EndMap;
		}

		// Camera component
		if (entity.hasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			auto& cameraComponent = entity.getComponent<CameraComponent>();
			auto& camera = cameraComponent.camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap;
			out << YAML::Key << "CameraType" << YAML::Value << (int)camera.getCameraType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.getPerspectiveVerticalFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.getPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.getPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.getOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.getOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.getOrthographicFarClip();
			out << YAML::EndMap;

			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.fixedAspectRatio;

			out << YAML::EndMap;
		}

		// Sprite Renderer component
		if (entity.hasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap;

			auto& spriteRendererComponent = entity.getComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.color;

			out << YAML::EndMap;
		}

		// Circle renderer component
		if (entity.hasComponent<CircleRendererComponent>())
		{
			out << YAML::Key << "CircleRendererComponent";
			out << YAML::BeginMap;

			auto& circleRendererComponent = entity.getComponent<CircleRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << circleRendererComponent.color;
			out << YAML::Key << "Thickness" << YAML::Value << circleRendererComponent.thickness;
			out << YAML::Key << "Fade" << YAML::Value << circleRendererComponent.fade;

			out << YAML::EndMap;
		}

		// Rigidbody 2D component
		if (entity.hasComponent<Rigidbody2DComponent>())
		{
			out << YAML::Key << "Rigidbody2DComponent";
			out << YAML::BeginMap;

			auto& rigidbody2DComponent = entity.getComponent<Rigidbody2DComponent>();
			out << YAML::Key << "Type" << YAML::Value << Rigidbody2DTypeToString(rigidbody2DComponent.type);
			out << YAML::Key << "FixedRotation" << YAML::Value << rigidbody2DComponent.fixedRotation;

			out << YAML::EndMap;
		}

		// Box Collider 2D component
		if (entity.hasComponent<BoxCollider2DComponent>())
		{
			out << YAML::Key << "BoxCollider2DComponent";
			out << YAML::BeginMap;

			auto& boxCollider2DComponent = entity.getComponent<BoxCollider2DComponent>();
			out << YAML::Key << "Offset" << boxCollider2DComponent.offset;
			out << YAML::Key << "Size" << boxCollider2DComponent.size;
			out << YAML::Key << "Density" << boxCollider2DComponent.density;
			out << YAML::Key << "Friction" << boxCollider2DComponent.friction;
			out << YAML::Key << "Restitution" << boxCollider2DComponent.restitution;
			out << YAML::Key << "RestitutionThreshold" << boxCollider2DComponent.restitutionThreshold;

			out << YAML::EndMap;
		}

		// Circle Collider 2D Component
		if (entity.hasComponent<CircleCollider2DComponent>())
		{
			out << YAML::Key << "CircleCollider2DComponent";
			out << YAML::BeginMap;

			auto& circleCollider2DComponent = entity.getComponent<CircleCollider2DComponent>();
			out << YAML::Key << "Offset" << circleCollider2DComponent.offset;
			out << YAML::Key << "Radius" << circleCollider2DComponent.radius;
			out << YAML::Key << "Density" << circleCollider2DComponent.density;
			out << YAML::Key << "Friction" << circleCollider2DComponent.friction;
			out << YAML::Key << "Restitution" << circleCollider2DComponent.restitution;
			out << YAML::Key << "RestitutionThreshold" << circleCollider2DComponent.restitutionThreshold;

			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	// Serialize
	void SceneSerializer::serialize(const std::string& filePath)
	{
		// Begin writing YAML
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";

		// Write entities
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		scene->registry.each([&](auto entityID)
		{
			Entity entity = { entityID, scene.get() };
			if (!entity)
				return;

			serializeEntity(out, entity);
		});

		// Finish writing YAML
		out << YAML::EndSeq;
		out << YAML::EndMap;

		// Write YAML to file
		std::ofstream fout(filePath);
		fout << out.c_str();
	}

	// Serialize during runtime - not implemented
	void SceneSerializer::serializeRuntime(const std::string& filePath)
	{
		ASSERT(false, "Function not implemented.");
	}

	// Deserialize scene
	bool SceneSerializer::deserialize(const std::string& filePath)
	{
		// Open file
		YAML::Node data;
		
		try
		{
			data = YAML::LoadFile(filePath);
		}
		catch (YAML::ParserException e)
		{
			return false;
		}

		if (!data["Scene"])
			return false;

		// Get scene name
		std::string sceneName = data["Scene"].as<std::string>();
		LOG_TRACE("Deserializing scene '{0}'", sceneName);

		// Parse data
		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>();

				// Tag component
				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as < std::string>();
				LOG_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

				// Create entity from tag
				Entity deserializedEntity = scene->createEntityWithUUID(uuid, name);

				// Transform component
				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					// Entities always have transform components
					auto& tc = deserializedEntity.getComponent<TransformComponent>();
					tc.translation = transformComponent["Translation"].as<glm::vec3>();
					tc.rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.scale = transformComponent["Scale"].as<glm::vec3>();
				}

				// Camera component
				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserializedEntity.addComponent<CameraComponent>();
					auto& cameraProps = cameraComponent["Camera"];

					cc.camera.setCameraType((SceneCamera::CameraType)cameraProps["CameraType"].as<int>());

					cc.camera.setPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
					cc.camera.setPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					cc.camera.setPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

					cc.camera.setOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.camera.setOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					cc.camera.setOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

					cc.primary = cameraComponent["Primary"].as<bool>();
					cc.fixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				// Sprite Renderer component
				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					auto& src = deserializedEntity.addComponent<SpriteRendererComponent>();
					src.color = spriteRendererComponent["Color"].as<glm::vec4>();
				}

				// Circle renderer component
				auto circleRendererComponent = entity["CircleRendererComponent"];
				if (circleRendererComponent)
				{
					auto& src = deserializedEntity.addComponent<CircleRendererComponent>();
					src.color = circleRendererComponent["Color"].as<glm::vec4>();
					src.thickness = circleRendererComponent["Thickness"].as<float>();
					src.fade = circleRendererComponent["Fade"].as<float>();
				}

				// Rigidbody 2D component
				auto rb2dComponent = entity["Rigidbody2DComponent"];
				if (rb2dComponent)
				{
					auto& rb2d = deserializedEntity.addComponent<Rigidbody2DComponent>();
					rb2d.type = Rigidbody2DTypeFromString(rb2dComponent["Type"].as<std::string>());
					rb2d.fixedRotation = rb2dComponent["FixedRotation"].as<bool>();
				}

				// Box Collider 2D component
				auto bc2dComponent = entity["BoxCollider2DComponent"];
				if (bc2dComponent)
				{
					auto& bc2d = deserializedEntity.addComponent<BoxCollider2DComponent>();
					bc2d.offset = bc2dComponent["Offset"].as<glm::vec2>();
					bc2d.size = bc2dComponent["Size"].as<glm::vec2>();
					bc2d.density = bc2dComponent["Density"].as<float>();
					bc2d.friction = bc2dComponent["Friction"].as<float>();
					bc2d.restitution = bc2dComponent["Restitution"].as<float>();
					bc2d.restitutionThreshold = bc2dComponent["RestitutionThreshold"].as<float>();
				}

				// Circle Collider 2D Component
				auto cc2dComponent = entity["CircleCollider2DComponent"];
				if (cc2dComponent)
				{
					auto& cc2d = deserializedEntity.addComponent<CircleCollider2DComponent>();
					cc2d.offset = cc2dComponent["Offset"].as<glm::vec2>();
					cc2d.radius = cc2dComponent["Radius"].as<float>();
					cc2d.density = cc2dComponent["Density"].as<float>();
					cc2d.friction = cc2dComponent["Friction"].as<float>();
					cc2d.restitution = cc2dComponent["Restitution"].as<float>();
					cc2d.restitutionThreshold = cc2dComponent["RestitutionThreshold"].as<float>();
				}
			}
		}

		return true;
	}

	// Deserialize scene during runtime - not implement
	bool SceneSerializer::deserializeRuntime(const std::string& filePath)
	{
		ASSERT(false, "Function not implemented.");
		return false;
	}
}