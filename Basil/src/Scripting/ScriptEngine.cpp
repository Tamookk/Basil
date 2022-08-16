#include "pch.h"
#include "Scripting/ScriptEngine.h"

namespace Basil
{
	namespace Utils
	{
		// Read bytes from provided file path (TODO: move to Util folder perhaps?)
		char* readBytes(const std::filesystem::path& filepath, uint32_t* outSize)
		{
			// Open the file
			std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

			// If file opening failed, return a null pointer
			if (!stream)
				return nullptr;

			// Get the file size
			std::streampos end = stream.tellg();
			stream.seekg(0, std::ios::beg);
			uint32_t size = end - stream.tellg();

			// If there is nothing in the file, return a null pointer
			if (size == 0)
				return nullptr;

			// Initialize char array
			char* buffer = new char[size];

			// Read bytes into char array
			stream.read((char*)buffer, size);
			stream.close();

			// Set size and return char array pointer
			*outSize = size;
			return buffer;
		}

		// Load a C# assembly from the provided file path
		MonoAssembly* loadMonoAssembly(const std::filesystem::path& assemblyPath)
		{
			// Get file data from assembly path
			uint32_t fileSize = 0;
			char* fileData = readBytes(assemblyPath, &fileSize);

			// Open a Mono Image from the file data
			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

			// If an error occured, return a nullptr and log the error message
			if (status != MONO_IMAGE_OK)
			{
				const char* errorMessage = mono_image_strerror(status);
				LOG_ERROR(errorMessage);
				return nullptr;
			}

			// Load the assembly
			std::string pathString = assemblyPath.string();
			MonoAssembly* assembly = mono_assembly_load_from_full(image, pathString.c_str(), &status, 0);

			// Close the image
			mono_image_close(image);

			// Delete file data and return the assembly
			delete[] fileData;
			return assembly;
		}

		// Print C# assembly types
		void printAssemblyTypes(MonoAssembly* assembly)
		{
			// Get image from assembly
			MonoImage* image = mono_assembly_get_image(assembly);

			// Get table info and number of types from image
			const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
			int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

			// For each type
			for (int32_t i = 0; i < numTypes; i++)
			{
				// Decode type
				uint32_t cols[MONO_TYPEDEF_SIZE];
				mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

				// Get type namespace and name
				const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
				const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

				// Print namespace and name
				LOG_TRACE("{}.{}", nameSpace, name);
			}
		}
	}

	struct ScriptEngineData
	{
		MonoDomain* rootDomain = nullptr;
		MonoDomain* appDomain = nullptr;

		MonoAssembly* coreAssembly = nullptr;
		MonoImage* coreAssemblyImage = nullptr;
		
		MonoAssembly* appAssembly = nullptr;
		MonoImage* appAssemblyImage = nullptr;

		Scene* sceneContext = nullptr;

		ScriptClass entityClass;

		std::unordered_map<std::string, Shared<ScriptClass>> entityClasses;
		std::unordered_map<UUID, Shared<ScriptInstance>> entityInstances;
	};

	static ScriptEngineData* scriptEngineData = nullptr;

	ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore)
	{
		this->classNamespace = classNamespace;
		this->className = className;

		this->monoClass = mono_class_from_name(isCore ? scriptEngineData->coreAssemblyImage : scriptEngineData->appAssemblyImage,
			classNamespace.c_str(), className.c_str());
	}

	MonoObject* ScriptClass::instantiate()
	{
		return ScriptEngine::instantiateClass(monoClass);
	}

	MonoMethod* ScriptClass::getMethod(const std::string& name, int parameterCount)
	{
		return mono_class_get_method_from_name(monoClass, name.c_str(), parameterCount);
	}

	MonoObject* ScriptClass::invokeMethod(MonoObject* instance, MonoMethod* method, void** params)
	{
		return mono_runtime_invoke(method, instance, params, nullptr);
	}

	// Script instance constructor
	ScriptInstance::ScriptInstance(Shared<ScriptClass> scriptClass, Entity entity)
	{
		// Set script class variable
		this->scriptClass = scriptClass;

		// Get instance, constructor, and create & update methods from the script
		instance = scriptClass->instantiate();
		constructor = scriptEngineData->entityClass.getMethod(".ctor", 1);
		onCreateMethod = scriptClass->getMethod("OnCreate", 0);
		onUpdateMethod = scriptClass->getMethod("OnUpdate", 1);

		// Call the constructor
		{
			UUID entityID = entity.getUUID();
			void* param = &entityID;
			scriptClass->invokeMethod(instance, constructor, &param);
		}
	}
	
	// Invoke the script's on create method
	void ScriptInstance::invokeOnCreate()
	{
		if (onCreateMethod)
			scriptClass->invokeMethod(instance, onCreateMethod);
	}

	// Invoke the script's on update method
	void ScriptInstance::invokeOnUpdate(float timeStep)
	{
		if (onUpdateMethod)
		{
			void* param = &timeStep;
			scriptClass->invokeMethod(instance, onUpdateMethod, &param);
		}
	}

	// Initialize the script engine
	void ScriptEngine::init()
	{
		// Create a new script engine data object
		scriptEngineData = new ScriptEngineData();

		// Initialize Mono
		initMono();

		// Load assembly and classes
		loadAssembly("Resources/Scripts/Basil-ScriptCore.dll");
		loadAppAssembly("SandboxProject/Assets/Scripts/Binaries/Sandbox.dll");
		loadAssemblyClasses();

		// Register components and functions
		ScriptGlue::registerComponents();
		ScriptGlue::registerFunctions();

		scriptEngineData->entityClass = ScriptClass("Basil", "Entity", true);

		/* Example code
		MonoObject* instance = scriptEngineData->entityClass.instantiate();

		// Call function
		MonoMethod* printMessageFunc = scriptEngineData->entityClass.getMethod("PrintMessage", 0);
		scriptEngineData->entityClass.invokeMethod(instance, printMessageFunc);

		// Call functions with param
		MonoMethod* printIntFunc = scriptEngineData->entityClass.getMethod("PrintInt", 1);
		int value = 4;
		void* param = &value;
		scriptEngineData->entityClass.invokeMethod(instance, printIntFunc, &param);

		MonoMethod* printIntsFunc = scriptEngineData->entityClass.getMethod("PrintInts", 2);
		int value1 = 5;
		int value2 = 6;
		void* params[2] = { &value1, &value2 };
		scriptEngineData->entityClass.invokeMethod(instance, printIntsFunc, params);

		MonoMethod* printCustomMessageFunc = scriptEngineData->entityClass.getMethod("PrintCustomMessage", 1);
		MonoString* monoString = mono_string_new(scriptEngineData->appDomain, "Wow, C#!");
		void* messageParam = monoString;
		scriptEngineData->entityClass.invokeMethod(instance, printCustomMessageFunc, &messageParam);
		*/
	}

	// Shut down the script engine
	void ScriptEngine::shutdown()
	{
		// Shut down Mono and delete the script engine data object
		shutdownMono();
		delete scriptEngineData;
	}

	// Load assembly (usually core one)
	void ScriptEngine::loadAssembly(const std::filesystem::path& filePath)
	{
		// Create an app domain
		scriptEngineData->appDomain = mono_domain_create_appdomain("BasilAppDomain", nullptr);
		mono_domain_set(scriptEngineData->appDomain, true);

		scriptEngineData->coreAssembly = Utils::loadMonoAssembly(filePath);
		scriptEngineData->coreAssemblyImage = mono_assembly_get_image(scriptEngineData->coreAssembly);
	}

	// Load app assembly
	void ScriptEngine::loadAppAssembly(const std::filesystem::path& filePath)
	{
		// Set app assembly and app assembly image
		scriptEngineData->appAssembly = Utils::loadMonoAssembly(filePath);
		scriptEngineData->appAssemblyImage = mono_assembly_get_image(scriptEngineData->appAssembly);
	}

	// On runtime start
	void ScriptEngine::onRuntimeStart(Scene* scene)
	{
		// Set script engine data scene context
		scriptEngineData->sceneContext = scene;
	}

	// On runtime stop
	void ScriptEngine::onRuntimeStop()
	{
		// Clear scene context and entity instances
		scriptEngineData->sceneContext = nullptr;
		scriptEngineData->entityInstances.clear();
	}

	// Return whether entity class exists
	bool ScriptEngine::entityClassExists(const std::string& fullClassName)
	{
		return scriptEngineData->entityClasses.find(fullClassName) != scriptEngineData->entityClasses.end();
	}

	// Run on create 
	void ScriptEngine::onCreateEntity(Entity entity)
	{
		// Get Script Component
		const auto& sc = entity.getComponent<ScriptComponent>();

		// If entity class exists
		if (ScriptEngine::entityClassExists(sc.className))
		{
			// Create script instance
			Shared<ScriptInstance> instance = makeShared<ScriptInstance>(scriptEngineData->entityClasses[sc.className], entity);

			// Update entity instances map and invoke on create method
			scriptEngineData->entityInstances[entity.getUUID()] = instance;
			instance->invokeOnCreate();
		}
	}

	// Run on update
	void ScriptEngine::onUpdateEntity(Entity entity, Timestep timeStep)
	{
		// Get UUID of entity and assert it's script component exists in the map
		UUID entityUUID = entity.getUUID();
		ASSERT(scriptEngineData->entityInstances.find(entityUUID) != scriptEngineData->entityInstances.end(), "");

		// Invoke on update method for the script component
		Shared<ScriptInstance> instance = scriptEngineData->entityInstances[entityUUID];
		instance->invokeOnUpdate(timeStep);
	}

	// Return scene context
	Scene* ScriptEngine::getSceneContext()
	{
		return scriptEngineData->sceneContext;
	}

	// Return map of entity classes
	std::unordered_map<std::string, Shared<ScriptClass>> ScriptEngine::getEntityClasses()
	{
		return scriptEngineData->entityClasses;
	}

	// Return core assembly image
	_MonoImage* ScriptEngine::getCoreAssemblyImage()
	{
		return scriptEngineData->coreAssemblyImage;
	}

	// Initialize Mono
	void ScriptEngine::initMono()
	{
		// Set the Mono assemblies path relative to the working directory (i.e., inside Pesto)
		mono_set_assemblies_path("mono/lib");

		// Initialize JIT
		MonoDomain* rootDomain = mono_jit_init("BasilJITRuntime");
		ASSERT(rootDomain, "");

		// Store the root domain pointer
		scriptEngineData->rootDomain = rootDomain;
	}

	// Shut down Mono
	void ScriptEngine::shutdownMono()
	{
		// Clear pointers to app and root domains
		scriptEngineData->appDomain = nullptr;
		scriptEngineData->rootDomain = nullptr;
	}

	MonoObject* ScriptEngine::instantiateClass(MonoClass* monoClass)
	{
		MonoObject* instance = mono_object_new(scriptEngineData->appDomain, monoClass);
		mono_runtime_object_init(instance);
		return instance;
	}

	// Load assembly classes from Mono assembly
	void ScriptEngine::loadAssemblyClasses()
	{
		// Clear entity classes map
		scriptEngineData->entityClasses.clear();

		// Get table info
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(scriptEngineData->appAssemblyImage, MONO_TABLE_TYPEDEF);

		// Get number of types and entity class by name
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);
		MonoClass* entityClass = mono_class_from_name(scriptEngineData->coreAssemblyImage, "Basil", "Entity");

		// For each type
		for (int32_t i = 0; i < numTypes; i++)
		{
			// Decode row
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			// Get namespace and name
			const char* nameSpace = mono_metadata_string_heap(scriptEngineData->appAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(scriptEngineData->appAssemblyImage, cols[MONO_TYPEDEF_NAME]);
			std::string fullName;

			// If namespace exists, append name to it to make fullName
			if (strlen(nameSpace) != 0)
				fullName = fmt::format("{}.{}", nameSpace, name);
			// Otherwise fullName is just name
			else
				fullName = name;

			// Get Mono class from the name
			MonoClass* monoClass = mono_class_from_name(scriptEngineData->appAssemblyImage, nameSpace, name);

			// If the Mono class is the entity class, continue
			if (monoClass == entityClass)
				continue;

			// If Mono class is a subclass of the entity class, add it to the map
			bool isEntity = mono_class_is_subclass_of(monoClass, entityClass, false);
			if (isEntity)
				scriptEngineData->entityClasses[fullName] = makeShared<ScriptClass>(nameSpace, name);
		}
	}
}
