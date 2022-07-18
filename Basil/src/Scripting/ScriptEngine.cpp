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

		ScriptClass entityClass;
	};

	static ScriptEngineData* scriptEngineData = nullptr;

	// Initialize the script engine
	void ScriptEngine::init()
	{
		// Create a new script engine data object
		scriptEngineData = new ScriptEngineData();

		// Initialize Mono
		initMono();

		loadAssembly("Resources/Scripts/Basil-ScriptCore.dll");

		ScriptGlue::registerFunctions();


		scriptEngineData->entityClass = ScriptClass("Basil", "Entity");

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
	}

	// Shut down the script engine
	void ScriptEngine::shutdown()
	{
		// Shut down Mono and delete the script engine data object
		shutdownMono();
		delete scriptEngineData;
	}

	void ScriptEngine::loadAssembly(const std::filesystem::path& filePath)
	{
		// Create an app domain
		scriptEngineData->appDomain = mono_domain_create_appdomain("BasilAppDomain", nullptr);
		mono_domain_set(scriptEngineData->appDomain, true);

		scriptEngineData->coreAssembly = Utils::loadMonoAssembly(filePath);
		scriptEngineData->coreAssemblyImage = mono_assembly_get_image(scriptEngineData->coreAssembly);
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

	ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className)
	{
		this->classNamespace = classNamespace;
		this->className = className;

		this->monoClass = mono_class_from_name(scriptEngineData->coreAssemblyImage, classNamespace.c_str(), className.c_str());
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
}
