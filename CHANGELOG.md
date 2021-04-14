# Change Log
Change log for the Basil Game Engine project.

## 2021-04-14
### Added
* Texture blending support.
* Ability to load RGB and RGBA images.
* Ability to read shader source code from files.

### Changed
* Changed directory structure of the Basil project.
* Updated includes to reflect directory structure change.
* Event Dispatcher no longer uses `std::function`, to improve performance.

### Fixed
* Some build warnings.


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