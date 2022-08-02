#include "pch.h"
#include "Core/Input.h"
#include "Core/KeyCodes.h"
#include "Scene/Entity.h"
#include "Scripting/ScriptEngine.h"
#include "Scripting/ScriptGlue.h"

#include <box2d/b2_body.h>
#include <mono/metadata/object.h>

namespace Basil
{
	static std::unordered_map<MonoType*, std::function<bool(Entity)>> entityHasComponentFuncs;

#define ADD_INTERNAL_CALL(Name) mono_add_internal_call("Basil.InternalCalls::" #Name, Name)

	// Logging from C# - cout string followed by parameter
	static void NativeLog(MonoString* string, int parameter)
	{
		char* cStr = mono_string_to_utf8(string);
		std::string message(cStr);
		mono_free(cStr);
		std::cout << message << ", " << parameter << std::endl;
	}

	// Logging vectors
	static void NativeLog_Vector(glm::vec3* parameter, glm::vec3* outResult)
	{
		// glm::vec3 shouldn't be needed but macros in Log.h are being stupid
		LOG_WARN("Value: {0}", glm::to_string(*parameter));
		*outResult = glm::normalize(*parameter);
	}

	// Logging vector dot products
	static float NativeLog_VectorDot(glm::vec3* parameter)
	{
		LOG_WARN("Value: {0}", glm::to_string(*parameter));
		return glm::dot(*parameter, *parameter);
	}

	// Checking whether an entity has a component
	static bool Entity_HasComponent(UUID entityId, MonoReflectionType* componentType)
	{
		// Get scene, assert it exists
		Scene* scene = ScriptEngine::getSceneContext();
		ASSERT(scene, "");
		
		// Get entity, assert it exists
		Entity entity = scene->getEntityByUUID(entityId);
		ASSERT(entity, "");

		// Get managed type, assert entity has it
		MonoType* managedType = mono_reflection_type_get_type(componentType);
		ASSERT(entityHasComponentFuncs.find(managedType) != entityHasComponentFuncs.end(), "");

		// Return component
		return entityHasComponentFuncs.at(managedType)(entity);
	}

	// Get translation from transform component
	static void TransformComponent_GetTranslation(UUID entityID, glm::vec3* outTranslation)
	{
		// Get scene and entity, assert they exist
		Scene* scene = ScriptEngine::getSceneContext();
		ASSERT(scene, "");
		Entity entity = scene->getEntityByUUID(entityID);
		ASSERT(entity, "");

		// Return translation
		*outTranslation = entity.getComponent<TransformComponent>().translation;
	}

	// Set translation of transform component
	static void TransformComponent_SetTranslation(UUID entityID, glm::vec3* translation)
	{
		// Get scene and entity, assert they exist
		Scene* scene = ScriptEngine::getSceneContext();
		ASSERT(scene, "");
		Entity entity = scene->getEntityByUUID(entityID);
		ASSERT(entity, "");

		entity.getComponent<TransformComponent>().translation = *translation;
	}

	// Apply linear impulse to a rigidbody
	static void Rigidbody2DComponent_ApplyLinearImpulse(UUID entityID, glm::vec2* impulse, glm::vec2* point, bool wake)
	{
		// Assert entity exists
		Scene* scene = ScriptEngine::getSceneContext();
		ASSERT(scene, "");
		Entity entity = scene->getEntityByUUID(entityID);
		ASSERT(entity, "");

		// Get runtime body and apply impulse to it
		auto& rb2d = entity.getComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.runtimeBody;
		body->ApplyLinearImpulse(b2Vec2(impulse->x, impulse->y), b2Vec2(point->x, point->y), wake);
	}

	// Apply linear impulse to a rigidbody to its center
	static void Rigidbody2DComponent_ApplyLinearImpulseToCenter(UUID entityID, glm::vec2* impulse, bool wake)
	{
		// Assert entity exists
		Scene* scene = ScriptEngine::getSceneContext();
		ASSERT(scene, "");
		Entity entity = scene->getEntityByUUID(entityID);
		ASSERT(entity, "");

		auto& rb2d = entity.getComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.runtimeBody;
		body->ApplyLinearImpulseToCenter(b2Vec2(impulse->x, impulse->y), wake);
	}

	// Return if key is down
	static bool Input_IsKeyDown(KeyCode keyCode)
	{
		return Input::isKeyPressed(keyCode);
	}

	// Register multiple components for C#
	template<typename... Component>
	static void registerComponent()
	{
		([]()
		{
			// Get type name
			std::string_view typeName = typeid(Component).name();

			// Get struct name
			size_t pos = typeName.find_last_of(':');
			std::string_view structName = typeName.substr(pos + 1);

			// Make managed type name
			std::string managedTypeName = fmt::format("Basil.{}", structName);

			// Get type from name
			MonoType* managedType = mono_reflection_type_from_name(managedTypeName.data(), ScriptEngine::getCoreAssemblyImage());
			if (!managedType)
			{
				LOG_ERROR("Could not find component type {}.", managedTypeName);
			}

			// Add component to map
			entityHasComponentFuncs[managedType] = [](Entity entity) {return entity.hasComponent<Component>(); };
		}(), ...);
	}

	// Register component group for C#
	template<typename... Component>
	void registerComponent(ComponentGroup<Component...>)
	{
		registerComponent<Component...>();
	}

	// Register components for C#
	void ScriptGlue::registerComponents()
	{
		// Register all components!
		registerComponent(AllComponents());
	}

	// Register functions for C#
	void ScriptGlue::registerFunctions()
	{
		ADD_INTERNAL_CALL(NativeLog);
		ADD_INTERNAL_CALL(NativeLog_Vector);
		ADD_INTERNAL_CALL(NativeLog_VectorDot);

		ADD_INTERNAL_CALL(Entity_HasComponent);
		ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		ADD_INTERNAL_CALL(TransformComponent_SetTranslation);

		ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulse);
		ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulseToCenter);

		ADD_INTERNAL_CALL(Input_IsKeyDown);
	}
}