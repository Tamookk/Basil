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
#### Project Setup
1. From the `scripts` directory inside the project directory, open a command prompt and run `Setup.bat`. This will install all dependencies and generate all necessary project files for building this project within Visual Studio 2022.
2. If you do not have the Vulkan SDK installed already, then this script will have to be run again after restarting your computer.
3. Once done, simply open `Basil.sln` and build away!

For an example of how to add C# scripts to your projects, see `Pesto/SandboxProject/Assets/Scripts`. In this folder is a Premake Lua script for generating a Visual Studio 2022 C# project, a batch file for doing this automatically, and the C# scripts themselves.

#### Building Mono Libraries
Debug and Build x64 Mono libraries for Windows have been included in the repository, as they are required for C# scripting support. I do plan on keeping these updated, however the following instructions can be followed for rebuilding them if necessary.

1. Clone Mono with `git clone https://github.com/mono/mono` into a folder separate from the one containing Basil. I recommend then checking out the latest release commit.
2. Navigate to the `msvc` directory within the cloned Mono repository, then open `mono.sln` in Visual Studio.
3. Build `libmono-static` in both Debug and Release configurations, using the x64 architecture. Make a note of the directory that these files are built to - they should be in `mono/msvc/build/sgen/x64/lib/`.
4. The `libmono-static-sgen.lib` files in the `Debug` and `Release` folders need to be copied to the `Debug` and `Release` folders in `Basil/Basil/vendor/mono/lib/`.
5. Following that, build `libmono-dynamic` in any configuration - this is just to get the header files.
6. Copy the `jit`, `metadata`, and `util` folders from `mono/msvc/include/mono` into `Basil/Basil/vendor/mono/include/mono`.
7. We are now done with the Mono repository!

Windows .NET 4.5 libraries from the [Mono release](https://www.mono-project.com/download/stable/) are contained within the working directory, `Pesto\mono\lib\`. I do not recommend updating these!

For more information on incorporating Mono into a game engine, please check out this guide written by Peter Nilsson: [Mono Embedding for Game Engines](https://peter1745.github.io/introduction.html).

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
* [Mono](https://github.com/mono/mono) for C# scripting support.
