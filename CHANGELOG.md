# Changelog
Changelog for the Basil Game Engine project.

## 2021-03-30
### Added
* Event handling to the ImGui layer.
* A KeyTyped event.
* A macro for binding events.
* Changelog file for documenting changes to the project.

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
* Created project and repository.

### Added
* Read me file.
* Licence file.