# Basil
Basil Game Engine

A game engine using the OpenGL API based on the tutorial found [here](https://www.youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT). This engine is not intended to ever be 100% perfect and ready for actually developing video games. This is more of a fun project for me, and a way to showcase some of my skills and knowledge.

This project was created and developed in Visual Studio Community 2019.

This project includes a game engine (Basil), set to be built as a dll, and a test application (Sandbox), for testing the game engine as I develop it. Currently, this engine only supports Windows. I'm hoping that one day I can expand it to work on other platforms, however.

## Getting Started
Visual Studio 2019 is recommended.

Clone the repository with `git clone --recursive https://github.com/Tamookk/Basil`.

### Windows Instructions
From the project directory, open a command prompt and run `vendor\premake\premake5.exe vs2019`. This will generate all necessary project files for building this project within Visual Studio 2019. Once done, simply open `Basil.sln` and build away!

**Please note** that the .sln and .vcxproj files included in the repository are not enough on their own to successfully build the project within VS2019. Project files *must* be generated using premake first.

## Third-Party Dependencies
A list of third-party software used in this project:
* [spdlog](https://github.com/gabime/spdlog) for logging
* [premake](https://github.com/premake/premake-core) for generating project files
* [GLFW](https://github.com/glfw/glfw) for OpenGL development
* [glad](https://github.com/Dav1dde/glad) for accessing modern OpenGL functions and extensions
* [Dear ImGui](https://github.com/ocornut/imgui) for GUIs
* [glm](https://github.com/g-truc/glm) for mathematics
* [stb](https://github.com/nothings/stb) (specifically stb_image) for image loading
