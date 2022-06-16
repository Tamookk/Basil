# Basil
Basil Game Engine

A game engine using the OpenGL API based on the tutorial found [here](https://www.youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT). This engine is not intended to ever be 100% perfect and ready for actually developing video games. This is more of a fun project for me, and a way to showcase some of my skills and knowledge.

This project was created and developed in Visual Studio Community 2022.

This project includes:
* A game engine (Basil)
* A test application (Sandbox), for testing the game engine as I develop it
* A game editor (Pesto), for developing games using Basil

Currently, this engine only supports Windows. I'm hoping that one day I can expand it to work on other platforms, however.

## Getting Started
Visual Studio 2022 is recommended.

Clone the repository with `git clone --recursive https://github.com/Tamookk/Basil`.

This project requires [Python 3](https://www.python.org/) for setup.

### Windows Instructions
1. From the `scripts` directory inside the project directory, open a command prompt and run `Setup.bat`. This will install all dependencies and generate all necessary project files for building this project within Visual Studio 2022.
2. If you do not have the Vulkan SDK installed already, then this script will have to be run again after restarting your computer.
3. Once done, simply open `Basil.sln` and build away!

## Third-Party Dependencies
A list of third-party software used in this project:
* [spdlog](https://github.com/gabime/spdlog) for logging
* [Premake](https://github.com/premake/premake-core) for generating project files
* [GLFW](https://github.com/glfw/glfw) for OpenGL development
* [glad](https://github.com/Dav1dde/glad) for accessing modern OpenGL functions and extensions
* [Dear ImGui](https://github.com/ocornut/imgui) for GUIs
* [ImGuizmo](https://github.com/TheCherno/ImGuizmo) for transformation gizmos
* [GLM](https://github.com/g-truc/glm) for mathematics
* [stb](https://github.com/nothings/stb) (specifically stb_image) for image loading
* [EnTT](https://github.com/skypjack/entt) for an entity component system
* [yaml-cpp](https://github.com/jbeder/yaml-cpp) for scene serialization
* [SPIRV-Cross](https://github.com/KhronosGroup/SPIRV-Cross) for converting SPIR-V to other shader languages
* [Shaderc](https://github.com/google/shaderc) for shader compilation.
* [Box2D](https://github.com/erincatto/box2d) for physics.
