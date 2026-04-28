# Solar System Simulation

## :video_game: Introduction

Here is a simulation of the Solar System in 3D! This project mainly originates from a strong interest in the Universe and Graphics programming. Developing it led me to dive into the OpenGL specification and classic Rendering techniques. The development of this project is still ongoing.

![SimulationStartingPoint](https://github.com/salepape/solar-system-simulation/blob/master/README%20Gifs/SimulationStartingPoint.gif)

## :hammer: Setup

> [!WARNING]
> The simulation is only available on Windows x86 (<i>[OS architecture] = Win32</i>) or x64 (<i>[OS architecture] = x64</i>), and for GPUs compatible with OpenGL version 4.0.
> To check the latter, open 'dxdiag' then 'Display' section, and look for the GPU model and DirectX 12 max feature level supported: if the max OpenGL version supported is lower, either the GPU or installed driver is considered legacy, and program will abort.

> [!NOTE]
> If your antivirus is flagging a threat just after running an executable from the Windows File Explorer, just ignore it: the program is safe to use :) You can also try to run it in Admin mode.

First, scroll up this GitHub page and locate the green "Code" button, and click on it.

### For end-users

Download the repository by clicking the "Download ZIP" button, and unzip the folder once completed (it should be called <i>solar-system-simulation-master</i>).
To run the executable, go to <i>Output/Release/[OS architecture]</i>, and double-click on `OpenGLProject.exe` (should start after 4 seconds in Release mode).

An Assimp DLL should be located in the same folder as the executable to run. If not, double-check you got the latest version of the remote Git Repository.

### For developers

Clone the repository by clicking the icon labelled "copy URL to clipboard", then open Git Bash from the folder you want to set up the project in, and run the command `git clone [URL]`. You can also use a Source Control IDE to proceed.
To build the executable, go to <i>OpenGLProject</i>, and double-click on `OpenGLProject.sln`. Choose the OS architecture/configuration you need, click on the green arrow button (building time around 80 seconds).

> [!WARNING]
> Not having the same Visual Studio environment installed (as specified [here](#environment-specifications)) can lead to build errors. You can set this up from Visual Studio Installer, or download all needed elements from [the official website](https://visualstudio.microsoft.com/downloads).

Here are the required Visual Studio elements to build the Solution:
* IDE: Visual Studio 2022 or later (the latter requiring a Solution upgrade)
* programming language: workload <b>Desktop Development in C++</b> (should include the latest SDK available, i.e. the individual component <b>Windows 11 SDK (10.0.26100.7705)</b> or later)
* compiler: individual components <b>C++ Clang Compiler for Windows (19.1.5)</b> and <b>MSBuild support for LLVM (clang-cl) toolset</b> (you can still switch back to Microsoft MSVC Compiler in the Project Settings if you prefer).

You might then need to right-click on the Solution from inside the Visual Studio IDE and click:
* <b>Load Project</b> if unloaded
* <b>Set it as Startup project</b> if Solution Configuration is empty.

An Assimp DLL should automatically be added to the Project folder after it has successfully built. If not, you can:
* copy the DLL manually located in <i>Tools/Libraries/[OS Architecture]/Assimp</i>
* execute the batch file `CopyDLLPostBuild.bat` from a cmd: `copy [path to DLL]\[assimp].dll [path to .exe]`.

## :joystick: Controls

To be able to explore the Solar System, the user should use the following controls:
* <kbd>Z</kbd>, <kbd>Q</kbd>, <kbd>S</kbd>, <kbd>D</kbd> (for an AZERTY keyboard) to move forward, left, backward, and right respectively,
* <kbd>left mouse button</kbd>, <kbd>right mouse button</kbd> to move up and down, <kbd>mouse scroll wheel</kbd> to zoom in and out
* <kbd>Shift</kbd> to move faster
* <kbd>R</kbd> (like Reset) to teleport the user to the initial location (i.e. just above the Sun)
* <kbd>L</kbd> (like Legend) to display names of each celestial body
* <kbd>H</kbd> (like Headlamp) to turn on/off user's headlight, to better explore regions with poor lighting
* <kbd>Up arrow</kbd> and <kbd>down arrow</kbd> to speed up/slow down the simulation
* <kbd>Space</kbd> to pause/unpause the simulation
* <kbd>Tab</kbd> to switch the application to cursor mode (allowing you to resize the window, background the simulation, etc.)
* <kbd>Esc</kbd> to quit the simulation.

## :telescope: Content 

The simulation contains the following celestial bodies:
* the Sun
* the 8 planets of the Solar System
* 10 dwarf planets
* 6 ring systems (the ones in the visible/infrared spectrum)
* 30 moons
* 2 belts (Asteroid and Kuiper ones) plus the Scattered Disc
* the Milky Way in background.

> [!NOTE]
> For the most distant celestial bodies, statistical data can be hard to find, so texture maps are fictionnal, and chosen according to the photometry analysis presented in Astrophysical research papers. 

## :bricks: Features

The actual simulation is decoupled from the object-oriented engine that runs it. Below are its main features:
* :ringed_planet: Celestial Bodies with their moons, in motion along circular orbits, with adjustable simulation speed
* :movie_camera: Perspective Camera Controller & Input System for an intuitive exploration
* :globe_with_meridians: Meshes computed in code from scratch, or loaded from file for asteroid/ring system 3D Models
* :mag: Mesh blending taking into account the distance of each Scene Entity to the Perspective Camera
* :rocket: 3D Mesh Renderer with instanced rendering to draw the belts in a more performant way
* :page_facing_up: Glyph Loader rendered on 2D quads to display the names of Celestial Bodies
* :flashlight: Blinn-Phong Illumination model running on GPU via GLSL shaders, with a Point Light for Sun contribution.

Comments in the codebase describe implementation details (technical decisions, documentation, etc.)

## :gear: Technologies

### Environment Specifications

This project is being developed under an environment with the following specifications:
* Intel(R) Core(TM) i7-8550U CPU
* Intel(R) UHD Graphics 620 GPU (DirectX 12-compatible)
* Windows 10 OS with x64 architecture
* Visual Studio Community 2022 IDE version 17
* LLVM Clang C++ compiler version 19.1.5
* C17/C++17/GLSL programming languages
* OpenGL 4.0 (Core Profile) specification.

### Third-Party Libraries

Here is the list of all the open-source libraries used for this project:
* [Glad](https://github.com/dav1dde/glad) to get Core OpenGL function pointers to the correct implementation version
* [GLFW](https://github.com/glfw) to initialise both the OpenGL context/application window and handle user input
* [GLM](https://github.com/g-truc/glm) for mathematical utility functions
* [SOIL2](https://github.com/SpartanJ/soil2) to load DDS textures in different fashion (i.e. bitmaps and cubemaps)
* [Assimp](https://github.com/assimp) DLL to load 3D models (i.e. composed of sub-meshes and materials)
* [FreeType](https://github.com/freetype) to render 2D glyphs (i.e. ASCII characters).

Finally, [RenderDoc](https://github.com/baldurk/renderdoc) has been used to improve the rendering performance/frame rate in some places.

## :gem: Resources

Scientific data have been retrieved from:
* the NASA website, in particular [NASA Science](https://science.nasa.gov) and the [National Space Science Data Center](https://science.nasa.gov/solar-system/)
* sources from passionate people such as [The Solar System Wiki](https://thesolarsystem.fandom.com)

Celestial body textures have been downloaded from the websites below:
* [Solar System Scope](https://www.solarsystemscope.com/textures)
* [JHT's Planetary Pixel Emporium](https://planetpixelemporium.com/planets.html)
* [Jet Propulsion Laboratory](https://www.jpl.nasa.gov) data
* [3D Asteroid Catalogue](https://3d-asteroids.space)
* sources from passionate people such as [Planet Texture Maps Wiki](https://planet-texture-maps.fandom.com/wiki).

Finally, I am very grateful to the following people/sources, real catalysers for my understanding of OpenGL.
* Joey De Vries for his tutorial website [Learn OpenGL](https://learnopengl.com)
* Yan Chernikov (aka TheCherno) for his tutorial videos [here](https://www.youtube.com/user/TheChernoProject)
* Nicol Bolas for his numerous interventions and [his website](https://nicolbolas.github.io)
* Rémy Malgouyres for his course "Programmation 3D avec OpenGL"
* the website [opengl-tutorial](http://www.opengl-tutorial.org)
* the website [open.gl](https://open.gl)
* the [official Khronos documentation](https://registry.khronos.org/OpenGL-Refpages/)
* the [OpenGL Wiki](https://wikis.khronos.org/opengl/Main_Page)
* on-site SIGGRAPH conferences
* other websites, as well as many Stack Overflow/Reddit threads, videos and books!