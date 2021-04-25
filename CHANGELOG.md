# Change Log
Change log for the Basil Game Engine project.

## 2021-04-25
### Added
* Log messages are now also sent to a log file.

### Changed
* Made the runloop accessible only on the engine side.


## 2021-04-24
### Added
* The available \_\_FUNCSIG__ defintion is now auto-deduced for wider compiler compatibility.
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