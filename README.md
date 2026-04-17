# Solar System Simulation

## :video_game: Introduction

Here is a simulation of the Solar System in 3D! This project mainly originates from a strong interest in the Universe and Graphics programming. I familiarised myself with the OpenGL specification and classic Rendering techniques. The development of this project is still ongoing...

## :gear: Technologies

The project is developed under a Windows 10 environment using Visual Studio 22. It is implemented in C17/C++17/GLSL, uses OpenGL 4.6 (Core Profile) specification as well as the following set of libraries:
* Glad to get Core OpenGL function pointers to the correct implementation version,
* GLFW3 to initialise a window and handle user input,
* GLM for mathematical utility functions,
* SOIL2 to load textures in different formats,
* Assimp to load 3D models (sub-meshes and materials),
* FreeType to render 2D glyph characters.
Finally, RenderDoc has also been used to improve the performance in some places.

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
> For most distant celestial bodies, information can be hard to find, so texture maps are fictionnal, and chosen according to the photometry analysis presented in Astrophysical research papers. 

## :bricks: Features

The actual 3D Solar System simulation is decoupled from the object-oriented engine that runs it. Below are the main features implemented so far:
* :ringed_planet: Celestial bodies in motion according along circular orbits, with adjustable simulation speed,
* :movie_camera: Perspective Camera/Controller & Input System for an intuitive exploration, including an accelerator, a magnifier and a headlamp (using GLFW3),
* :globe_with_meridians: Geometric meshes computations, including a cubemap for the Skybox, and a 3D model loader for asteroids/ring systems (using Assimp),
* :mag: Mesh blending taking into account the distance of each scene entity to the camera,
* :rocket: 3D Mesh Renderer with instanced rendering to draw the belts in a more performant way,
* :page_facing_up: 2D Text renderer to display celestial body names,
* :flashlight: Blinn-Phong Illumination model running on the GPU via GLSL shaders, for all scene entities, with a point light approximating Sun contribution.

Comments within the codebase describe more specifically how it works (algorithmic clarifications, stances, technical decision explanations, etc.)

## :hammer: Setup

> [!WARNING] 
> The simulation is available only for Windows OS, on a x86 or x64 architecture.

You can either clone the Git repository to your computer, or unzip the corresponding .zip folder after downloading it (it will be called solar-system-simulation-master). Then:
* If you just want to run the program as an end user, double-click on the executable <b>OpenGLProject.exe</b> you need according to your OS architecture (stored in <i>OpenGLProject/Output/Release/Win32</i> for x86, or <i>x64/</i> for x64).
* If you want to run the code, you need to have Visual Studio 22 installed, then go to the project main folder, open <b>OpenGLProject.sln</b> Visua Studio solution, choose the architecture/configuration you want, and click on the green arrow button to launch the simulation.

> [!NOTE]
> In any of these cases, an executable is run, and needs the Assimp DLL to be located in the same folder.
> * In the first scenario, the batch script <b>CopyDLLPostBuild.bat</b> is automatically run after the project has built,
> * In the second scenario, the DLL is already provided. For some reason, if no DLL is there, you can ensure you got the latest version of the project, run the batch file yourself, or copy the DLL manually from <i>OpenGLProject/Tools/Libraries/ArchitectureName/Assimp</i>

The execution of the program should require around 4 seconds in Release mode, and 8 seconds in Debug mode before starting. Enjoy!

> [!NOTE]
> If your antivirus is flagging a threat just after running an executable from the File Explorer, just ignore it: my program is safe to use :)

## :joystick: Controls

To be able to explore the Solar System, the user should use the following controls:
* <kbd>Z</kbd>, <kbd>Q</kbd>, <kbd>S</kbd>, <kbd>D</kbd> (for an AZERTY keyboard), <kbd>left mouse button</kbd> and <kbd>right mouse button</kbd> to move forward, to the left, backward, to the right, up and down respectively,
* <kbd>mouse scroll wheel</kbd> to zoom in/out,
* <kbd>Shift</kbd> to increase speed when moving,
* <kbd>R</kbd> (like Reset) to teleport the user to the initial location (i.e. just above the Sun),
* <kbd>L</kbd> (like Legend) to make celestial body names appear above them,
* <kbd>H</kbd> (like Headlamp) to turn on/off user's headlight, to better explore regions with poor lighting,

* <kbd>Up arrow</kbd> and <kbd>down arrow</kbd> to speed up/slow down the simulation,
* <kbd>Space</kbd> to pause/unpause the simulation,
* <kbd>Tab</kbd> to switch the application to cursor mode (to resize the window easily, or background the simulation for instance),
* <kbd>Esc</kbd> to quit the simulation.

## :gem: Resources

Scientific data and several models has been retrieved from the NASA website (National Space Science Data Center).

Celestial body textures have been downloaded from:
* https://www.solarsystemscope.com/textures,
* https://planetpixelemporium.com/planets.html,
* https://www.jpl.nasa.gov,
* https://3d-asteroids.space,
* websites of passionate people such as https://planet-texture-maps.fandom.com/wiki.

My understanding of OpenGL and the world of programming has mainly been based on resources from:
* https://learnopengl.com (from Joey De Vries),
* https://www.youtube.com/user/TheChernoProject (from Yan Chernikov, aka TheCherno),
* http://www.opengl-tutorial.org,
* https://open.gl to a lesser extent,
* many Reddit threads!