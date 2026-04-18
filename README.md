# Solar System Simulation

## :video_game: Introduction

Here is a simulation of the Solar System in 3D! This project mainly originates from a strong interest in the Universe and Graphics programming. I familiarised myself with the OpenGL specification and classic Rendering techniques. The development of this project is still ongoing...

![SimulationStartingPoint](https://github.com/salepape/solar-system-simulation/blob/master/README%20Gifs/SimulationStartingPoint.gif)

## :gear: Technologies

The project is developed under a Windows 10 environment using Visual Studio 22. It is implemented in C17/C++17/GLSL, uses OpenGL 4.6 (Core Profile) specification as well as the following set of libraries:
* [Glad](https://github.com/dav1dde/glad) to get Core OpenGL function pointers to the correct implementation version,
* [GLFW](https://github.com/glfw/glfw) to initialise the aaplication window and handle user input,
* [GLM](https://github.com/g-truc/glm) for mathematical utility functions,
* [SOIL2](https://github.com/SpartanJ/soil2) to load DDS textures in different fashion (i.e. bitmaps and cubemaps),
* [Assimp](https://github.com/assimp/assimp) to load 3D models (i.e. sub-meshes and materials),
* [FreeType](https://github.com/freetype) to render 2D glyph characters.

Finally, [RenderDoc](https://github.com/baldurk/renderdoc) has also been used to improve the performance in some places.

## :telescope: Content 

The simulation contains the following celestial bodies:
* the Sun,
* the 8 planets of the Solar System,
* 10 dwarf planets (some of them not unanimously considered as such by the scientific community),
* 6 ring systems (the ones in the visible/infrared spectrum),
* 30 spherical moons,
* 2 belts (Asteroid and Kuiper ones, plus the Scattered Disc),
* Milky Way in background.

> [!NOTE]
> For the most distant celestial bodies, statistical data can be hard to find, so texture maps are fictionnal, and chosen according to the photometry analysis presented in Astrophysical research papers. 

## :bricks: Features

The actual 3D Solar System simulation is decoupled from the object-oriented engine that runs it. Below are its main features:
* :ringed_planet: Celestial Bodies with their Moons, in motion along circular orbits, with adjustable simulation speed,
* :movie_camera: Perspective Camera Controller & Input System for an intuitive exploration,
* :globe_with_meridians: Meshes computed in code from scratch, or loaded from file for asteroid/ring system 3D Models,
* :mag: Mesh blending taking into account the distance of each scene entity to the player camera,
* :rocket: 3D Mesh Renderer with instanced rendering to draw the belts in a more performant way,
* :page_facing_up: 2D Text renderer to display celestial body names,
* :flashlight: Blinn-Phong Illumination model running on GPU via GLSL shaders, with a Point Light for Sun contribution.

Comments within the codebase describe more specifically implementation details (documentation, technical decisions, etc.)

## :hammer: Setup

> [!WARNING]
> The simulation is available only for Windows OS, on a x86 or x64 architecture.

You can either clone the Git repository to your computer, or unzip the corresponding .zip folder after downloading it (it will be called solar-system-simulation-master). Then:
* If you want to run the executable, double-click on the executable <b>OpenGLProject.exe</b> you need according to your OS architecture (stored in <i>OpenGLProject/Output/Release/Win32</i> for x86, or <i>x64/</i> for x64).
* If you want to build the executable, go to the project main folder, open <b>OpenGLProject.sln</b> Visual Studio 22 solution, choose the architecture/configuration you need, and click on the green arrow button.

> [!NOTE]
> In any of these cases, an executable is run, and needs the Assimp DLL to be located in the same folder.
> * In the former scenario, the batch script <b>CopyDLLPostBuild.bat</b> is automatically run after the project has successfully built,
> * In the latter scenario, the DLL is provided next to the executable. If, for some reason, no DLL is there, double-check you got the latest version of the remote repository. You can also run the batch file yourself, or copy the DLL manually from <i>OpenGLProject/Tools/Libraries/ArchitectureName/Assimp</i>

The execution of the program should require around 4 seconds in Release mode, and 8 seconds in Debug mode before starting. Enjoy!

> [!NOTE]
> If your antivirus is flagging a threat just after running an executable from the File Explorer, just ignore it: the program is safe to use :)

## :joystick: Controls

To be able to explore the Solar System, the user should use the following controls:
* <kbd>Z</kbd>, <kbd>Q</kbd>, <kbd>S</kbd>, <kbd>D</kbd> (for an AZERTY keyboard) to move forward, left, backward, and right respectively,
* <kbd>left mouse button</kbd> and <kbd>right mouse button</kbd> to move up and down, 
* <kbd>mouse scroll wheel</kbd> to zoom in/out,
* <kbd>Shift</kbd> to increase speed when moving,
* <kbd>R</kbd> (like Reset) to teleport the user to the initial location (i.e. just above the Sun),
* <kbd>L</kbd> (like Legend) to make celestial body names appear above the meshes,
* <kbd>H</kbd> (like Headlamp) to turn on/off user's headlight, to better explore regions with poor lighting,
* <kbd>Up arrow</kbd> and <kbd>down arrow</kbd> to speed up/slow down the simulation,
* <kbd>Space</kbd> to pause/unpause the simulation,
* <kbd>Tab</kbd> to switch the application to cursor mode (allowing you to resize the window, background the simulation, etc.)
* <kbd>Esc</kbd> to quit the simulation.

## :gem: Resources

Scientific data and several models has been retrieved from the NASA website (in particular the [National Space Science Data Center](https://science.nasa.gov/solar-system/)).

Celestial body textures have been downloaded from the websites:
* [Solar System Scope](https://www.solarsystemscope.com/textures),
* [JHT's Planetary Pixel Emporium](https://planetpixelemporium.com/planets.html),
* [Jet Propulsion Laboratory](https://www.jpl.nasa.gov) data,
* [3D Asteroid Catalogue](https://3d-asteroids.space),
* websites of passionate people such as [Planet Texture Maps Wiki](https://planet-texture-maps.fandom.com/wiki).

My understanding of OpenGL and the world of programming has mainly been thanks to these people and websites:
* Joey De Vries for his tutorial website [Learn OpenGL](https://learnopengl.com),
* Yan Chernikov (aka TheCherno) for his tutorial videos [here](https://www.youtube.com/user/TheChernoProject),
* the website [opengl-tutorial](http://www.opengl-tutorial.org),
* the website [open.gl](https://open.gl),
* online documentation, many books and Stack Overflow/Reddit threads!