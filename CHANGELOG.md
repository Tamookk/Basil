# Change Log
Change log for the Basil Game Engine project.

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