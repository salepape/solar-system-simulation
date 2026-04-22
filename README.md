# Solar System Simulation

## :video_game: Introduction

Here is a simulation of the Solar System in 3D! This project mainly originates from a strong interest in the Universe and Graphics programming. Developing this project led me to dive into the OpenGL specification and classic Rendering techniques. The development of this project is still ongoing.

![SimulationStartingPoint](https://github.com/salepape/solar-system-simulation/blob/master/README%20Gifs/SimulationStartingPoint.gif)

## :gear: Technologies

### Environment Specifications

This project is being developed under an environment with the following specifications:
* ASUS UX461U with Intel(R) Core(TM) i7-8550U CPU and Intel(R) UHD Graphics 620 GPU
* Windows 10 OS with x64 architecture
* Visual Studio Community 22 IDE
* LLVM Clang C++ compiler
* C17/C++17/GLSL programming languages
* OpenGL 4.6 (Core Profile) specification.

### Third-Party Libraries

Here is the list of all the open-source libraries used for this project:
* [Glad](https://github.com/dav1dde/glad) to get Core OpenGL function pointers to the correct implementation version
* [GLFW](https://github.com/glfw) to initialise both the OpenGL context/application window and handle user input
* [GLM](https://github.com/g-truc/glm) for mathematical utility functions
* [SOIL2](https://github.com/SpartanJ/soil2) to load DDS textures in different fashion (i.e. bitmaps and cubemaps)
* [Assimp](https://github.com/assimp) to load 3D models (i.e. composed of sub-meshes and materials)
* [FreeType](https://github.com/freetype) to render 2D glyphs (i.e. ASCII characters).

Finally, [RenderDoc](https://github.com/baldurk/renderdoc) has also been used to improve the rendering performance/frame rate in some places.

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
* :mag: Mesh blending taking into account the distance of each scene entity to the player camera
* :rocket: 3D Mesh Renderer with instanced rendering to draw the belts in a more performant way
* :page_facing_up: 2D Text renderer to display celestial body names
* :flashlight: Blinn-Phong Illumination model running on GPU via GLSL shaders, with a Point Light for Sun contribution.

Comments in the codebase describe implementation details (technical decisions, documentation, etc.)

## :hammer: Setup

> [!WARNING]
> The simulation is available only for Windows OS, on a x86 or x64 architecture.

You can either clone the Git repository to your computer via Git commands/your favourite Source Control IDE, or unzip the corresponding .zip folder after downloading it (it will be called <i>solar-system-simulation-master</i>).

The execution of the program should require around 4 seconds in Release mode, and 8 seconds in Debug mode before starting. Enjoy!

### For end-users

If you want to run the executable, go to <i>OpenGLProject/Output/Release/[OS architecture]</i> (<i>Win32</i> for x86, or <i>x64/</i> for x64), and double-click on <b>OpenGLProject.exe</b>

An Assimp DLL needs to be located in the same folder as the executable. If, for some reason, you don't, double-check you got the latest version of the remote Git Repository. You can also run the batch file yourself from a command line, or copy the DLL manually located in <i>OpenGLProject/Tools/Libraries/[OS Architecture]/Assimp</i>.

> [!NOTE]
> If your antivirus is flagging a threat just after running an executable from the File Explorer, just ignore it: the program is safe to use :) You can also try to run it in Admin mode.

### For developers

If you want to build the executable, go to <i>OpenGLProject/OpenGLProject</i>, and double-click on <b>OpenGLProject.sln</b>. Choose the OS architecture/configuration you need, and click on the green arrow button. 

> [!WARNING]
> Not having the same Visual Studio environment installed (as specified [here](#environment-specifications)) can lead to build errors. You can set this up from Visual Studio Installer or download all needed elements from [the official website](https://visualstudio.microsoft.com/downloads).
> Furthermore, all used libraires need to be compatible with your OS/computer specifications.

Here are the required Visual Studio elements to build the Solution:
* IDE version: Visual Studio 2022 (running it in later versions should be fine if Solution is upgraded)
* programming language: worload "Desktop Development in C++"
* compiler: individual components "C++ Clang Compiler for Windows (19.1.5)" and "MSBuild support for LLVM (clang-cl) toolset" (you can still switch back to Microsoft MSVC Compiler in the Project Settings if you prefer)

You might also need to right-click on the Solution from inside the Visual Studio IDE and click:
* "Load Project" if unloaded
* "Set it as Startup project" if Solution Configuration is empty.

The batch script <b>CopyDLLPostBuild.bat</b> is automatically run after the project has successfully built to add an Assimp DDL to the Project folder.

## :joystick: Controls

To be able to explore the Solar System, the user should use the following controls:
* <kbd>Z</kbd>, <kbd>Q</kbd>, <kbd>S</kbd>, <kbd>D</kbd> (for an AZERTY keyboard) to move forward, left, backward, and right respectively,
* <kbd>left mouse button</kbd> and <kbd>right mouse button</kbd> to move up and down
* <kbd>mouse scroll wheel</kbd> to zoom in/out
* <kbd>Shift</kbd> to increase speed when moving
* <kbd>R</kbd> (like Reset) to teleport the user to the initial location (i.e. just above the Sun)
* <kbd>L</kbd> (like Legend) to make celestial body names appear above the meshes
* <kbd>H</kbd> (like Headlamp) to turn on/off user's headlight, to better explore regions with poor lighting
* <kbd>Up arrow</kbd> and <kbd>down arrow</kbd> to speed up/slow down the simulation
* <kbd>Space</kbd> to pause/unpause the simulation
* <kbd>Tab</kbd> to switch the application to cursor mode (allowing you to resize the window, background the simulation, etc.)
* <kbd>Esc</kbd> to quit the simulation.

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

Finally, I am very grateful to the following people/sources: they have been real catalysers for my understanding of OpenGL and the world of Graphics.
* Joey De Vries for his tutorial website [Learn OpenGL](https://learnopengl.com)
* Yan Chernikov (aka TheCherno) for his tutorial videos [here](https://www.youtube.com/user/TheChernoProject)
* Rémy Malgouyres for his course "Programmation 3D avec OpenGL"
* the website [opengl-tutorial](http://www.opengl-tutorial.org)
* the website [open.gl](https://open.gl)
* the [official Khronos documentation](https://registry.khronos.org/OpenGL-Refpages/)
* the [OpenGL Wiki](https://wikis.khronos.org/opengl/Main_Page)
* on-site SIGGRAPH conferences
* other websites, as well as many Stack Overflow/Reddit threads, videos and books!