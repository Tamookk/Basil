# Change Log
Change log for the Basil Game Engine project.

## 2021-09-18
### Added
* Box2D submodule for physics.
* An example 2D Physics scene.
* Initial physics work - a rigidbody and box collider framework.
* Logging for GLM matrices, vectors, and quaternions.
* A check when loading textures to see if texture data was actually loaded.
* A try-catch block when deserializing scenes for parser exceptions.
* A check for the file extension when opening scene files.

### Changed
* Setup script now updates git submodules.


## 2021-09-11
### Added
* Play/stop button to the editor.
* `imgui.ini` file, so the Pesto editor layout is consistent across compiles/installs of Basil.

### Changed
* Default texture magnification filter changed to linear.

### Fixed
* Texture index being interpolated in shaders.


## 2021-08-21
### Added
* Drag + drop textures to the Sprite Renderer Component.
* A tiling factor to the Sprite Renderer Component.


## 2021-07-29
### Added
* Drag + drop to the Content Browser panel.


## 2021-07-16
### Added
* Content Browser panel icons.

### Changed
* Layout of the Content Browser panel.


## 2021-07-15
### Added
* A Python script to install Premake (instead of storing the exe in the repository).
* Post-build commands for copying Vulkan dlls to the output directory.
* Initial Content Browser panel work.

### Changed
* Made some changes to the setup scripts:
  * VulkanSDK directory is now created if it doesn't exist.
  * Setup is in different classes.
  * A progress bar has been added to the unzip function.
  * The UserAgent requirement has been removed.
  * Asks to use pip to install Python prerequisites to not break Python installs that don't use pip.

### Fixed
* Made fixes to Sandbox to ensure it compiles and runs on a fresh checkout.
* Fixed error in `Texture.glsl` that would cause the program to crash when using textures. 

### Removed
* Unused variable `time` in `ImGuiLayer.h/cpp`.
* Non-working URL from the URL list in `SetupVulkan.py`.


## 2021-05-20
### Added
* Python scripts to retrieve dependencies.
* Uniform buffers.
* Command line arguments.
* shaderc and SPIRV-Cross submodules.

### Changed
* Now using new shader system.
* Made Premake scripts better.


## 2021-05-18
### Added
* Selecting entities in the viewport.

### Fixed
* Bug where program would crash if you changed the gizmo type while using it.
* OpenGLVertexArray for integer attributes.
* Wrong viewport bounds when viewport tab bar is visible.


## 2021-05-17
### Added
* Added API to clear texture attachments.


## 2021-05-15
### Added
* GL_RED_INTEGER as a framebuffer format.

### Changed
* Refactored framebuffers to allow for multiple render targets.


## 2021-05-13
### Added
* Transformation gizmos to Pesto using ImGuizmo.
* An editor camera for Pesto.


## 2021-05-12
### Added
* yaml-cpp submodule for scene serialization.
* Scene serialization.
* Open and save file dialogs


## 2021-05-11
### Added
* UI to add and remove entities and components.

### Changed
* Changed the look of Pesto's UI.


## 2021-05-10
### Added
* Sprite Render Component UI to properties panel in Pesto.

### Changed
* Changed look of Transform Component UI in properties panel.


## 2021-05-09
### Added
* A properties panel in Pesto.


## 2021-05-08
### Added
* Camera component.
* SceneCamera class.
* Camera resizing support.
* Initial native scripting support.
* A scene hierarchy panel in Pesto.

### Changed
* Changed `BIND_EVENT` in `Core.h` from `std::bind` to a lambda.
* Changed ScriptableEntity functions to be virtual.

### Fixed
* `ERROR` was being used instead of `LOG_ERROR` in `OpenGLShader.cpp`.


## 2021-05-07
### Added
* Initial ECS work.
* Entity class.


## 2021-05-06
### Added
* Licence for stb.
* EnTT for an entity component system.

### Changed
* Submodule project files are now generated in Basil/vendor instead of Basil/vendor/\[submodule\].
* The OpenGL framebuffer will no longer resize if the window size is less than or equal to 0 in any dimension.


## 2021-05-05
### Added
* A virtual destructor to the Framebuffer class.

### Changed
* Changed how the Input class works; platform-specific input files are no longer chosen at run-time.

### Fixed
* ImGui events are now processed properly depending on the viewport in focus.
* Flickering when the viewport in Pesto was resized.


## 2021-05-04
### Added
* Pesto, the Basil Game Engine Editor.
* A viewport panel for viewing the scene in Pesto.

### Changed
* Updated `README.md`.


## 2021-05-03
### Added
* Basic Framebuffer class.


## 2021-05-02
### Added
* Initial dockspace work.


## 2021-05-01
### Added
* Transforms to the batch renderer.
* Draw call splitting and stats.
* Debug break definition for cross-platform compatibility.

### Changed
* Replaced `Renderer2D::drawQuad` vertex setup code with a for loop.
* `data.QuadVertexBufferBase` in Renderer2D is now deleted on shutdown.
* Renderer2D now uses the `tintColor` argument when drawing a textured quad.
* Branching is used instead in `Texture.glsl` instead of indexing the texture sampler.
* Performed Instrumentor optimisation to remove \_\_cdecl and replace " with ' in the scope name at compile time. 

### Fixed
* Issue where you could not render more than `Renderer2DData::maxTextureSlots` different textures.
* Incorrect cases in `BufferElement::getComponentCount`.
* Build warning in Renderer2D.
* Issue where an empty vertex array would be drawn if an empty scene is being rendered.
* Issue where ImGui sometimes stops events from reaching the rest of the layers.


## 2021-04-30
### Added
* Texture support to batch rendering.

### Fixed
* Batching now works.


## 2021-04-25
### Added
* Log messages are now also sent to a log file.
* A Transform struct for setting and storing object transformations.
* Textures can now be tinted different colours.
* Basic batching.

### Changed
* Made the runloop accessible only on the engine side.
* The transform matrix only does a rotation if rotation doesn't equal 0 to save processing time.


## 2021-04-24
### Added
* The available \_\_FUNCSIG__ definition is now auto-deduced for wider compiler compatibility.
* OpenGL messages are now logged in debug mode.
* Implemented KeyCode and MouseCode enums.
* Mutex to instrumentor.
* Ensured that beginning an instrumentation session will end any that already exists.
* Compile-time check of window and input type.

### Changed
* Moved ExampleLayer in Sandbox into its own files.


## 2021-04-23
### Added
* Added instrumentation code over Basil codebase.

### Removed
* Removed ImGui demo window.


## 2021-04-22
### Added
* Renderer is now shutdown to free memory allocated when it is initialised.
* A file reading check to the OpenGL shader implementation.
* A timer for basic profiling.
* Added Debug folder to Basil project.
* Added basic instrumentor.

### Changed
* 2D renderer now uses a single shader for textured and non-textured quads.
* Changed some raw pointers to smart pointers.
* Changed empty constructors/destructors to default ones.
* Changed how profiling times are displayed.
* Moved Timer class to Debug folder.

### Removed
* DLL traces (BASIL_API macros).
* Traces of OpenGL in main renderer and Sandbox files.


## 2021-04-21
### Added
* Textures to the 2D renderer.
* More genets.


## 2021-04-20
### Added
* Transforms to the 2D renderer.

### Changed
* 2D renderer no longer relies on OpenGL.


## 2021-04-19
### Added
* A 2D layer in Sandbox for testing 2D rendering.
* A basic 2D renderer.

### Changed
* Moved some rendering code from Sandbox to the 2D renderer.


## 2021-04-18
### Changed
* Shaders are now deleted after their associated shader program is created.
* Changed some raw pointers to smart pointers.

## Fixed
* A layer's onDetach function is now called before it is deleted.
* Errors in Sandbox caused by changing some raw pointers to smart pointers.

### Removed
* Visual Studio solution and project files.


## 2021-04-16
### Changed
* Changed directory structure of the Basil project.
* Updated includes to reflect directory structure change.
* Altered some files to be more consistent with the rest.
* Changed code to remove compiler warnings.

### Removed
* Unused Test class/files.


## 2021-04-15
### Added
* An orthographic camera controller.
* Proper window resize event handling.


## 2021-04-14
### Added
* Texture blending support.
* Ability to load RGB and RGBA images.
* Ability to read shader source code from files.
* ShaderLibrary class for adding, loading, and retrieving shaders.

### Changed
* Changed directory structure of the Basil project.
* Updated includes to reflect directory structure change.
* Event Dispatcher no longer uses `std::function`, to improve performance.

### Fixed
* Some build warnings.
* Some errors in shader file loading.


## 2021-04-13
### Added
* Initial texture support using stb_image.
* Genet.


## 2021-04-11
### Added
* Shader abstraction.
* More uniform functions.
* Unique and Shared definitions.

### Changed
* Refactored smart pointers to Basil types.


## 2021-04-09
### Added
* Transforms to the renderer.
* A square.


## 2021-04-08
### Added
* A function to handle key pressed events in Sandbox.
* A class for keeping track of time.

### Changed
* Moved sandbox code to the Sandbox program.

### Fixed
* Scene data's view-projection matrix now updates properly when a scene begins.


## 2021-04-07
### Added
* Orthographic camera.


## 2021-04-06
### Added
* Classes for specifying the layout of buffers.
* Vertex array abstraction.
* Renderer flow and submission.

### Changed
* Glad/GLFW includes now all use angle brackets.


## 2021-04-05
### Added
* Vertex and index buffer abstractions.
* An OpenGL-specific implementation of vertex and index buffers.
* A basic renderer.


## 2021-04-04
### Added
* A generic graphics context.
* An OpenGL graphics context.
* Triangles. Well, a single triangle.
* A basic shader.


## 2021-04-02
### Changed
* Basil now builds as a static library instead of a dll.

### Fixed
* Premake files are now properly configured, eliminating most build warnings.
* Eliminated the rest of the build warnings with some code clean up.


## 2021-04-01
### Added
* Some Dear ImGui test code to Sandbox.

### Changed
* Swapped to the docking branch of Dear ImGui.
* The layer stack now uses an integer instead of an iterator to keep track of the amount of layers it has.

### Fixed
* Dear ImGui now actually works! Updated the ImGuiLayer to properly use Dear ImGui.

### Removed
* Some files no longer needed for Dear ImGui implementation.
* Herobrine.


## 2021-03-31
### Added
* Input polling.
* Definitions for key and mouse button codes.
* Maths (glm).

### Changed
* Edited `premake5.lua` so the Basil dll will copy into Sandbox's folder after it's built, even if the Sandbox folder doesn't exist yet.
* Moved GLFW includes from header files to cpp files for headers included in `Basil.h` so that GLFW is not included in Sandbox/client applications.


## 2021-03-30
### Added
* Event handling to the ImGui layer.
* A KeyTyped event.
* A macro for binding events.
* Change log file for documenting changes to the project.

### Changed
* Edited `premake5.lua`:
  * to use solution folders to group project dependencies together.
  * to change some project generation options.

### Fixed
* Fixed a typo in `ImGuiOpenGLRenderer.cpp`.


## 2021-03-29
### Added
* Dear ImGui layer.

### Fixed
* A bug causing the application to crash whenever the client would attempt to log a mouse move event.


## 2021-03-28
### Added
* GLAD


## 2021-03-22
### Added
* Layers and the layer stack
* Dispatching of events.


## 2021-03-21
### Added
* Window class.
* GLFW.
* A precompiled header.
* An event system.


## 2021-03-20
### Changed
* Moved to using premake for generating project/solution files.


## 2021-03-19
### Added
* Read me file.
* Licence file.
* Created project and repository.