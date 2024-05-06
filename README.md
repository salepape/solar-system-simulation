# Solar System Simulation

## Introduction

Here is a simulation of the Solar System in 3D! This project mainly originates from a strong interest in the Universe that surrounds us, and Graphics programming. I familiarised myself with the OpenGL specification and some classic Rendering techniques. The development of this project is still ongoing...

## Technologies

The project is using Visual Studio 17 and Windows 10 environment. It has been implemented in C++17/GLSL, uses OpenGL 3.2 (Core profile) specification, as well as a set of libraries:
* Glad to get Core OpenGL function pointers,
* GLFW3 to initialise a window and handle user input,
* GLM for mathematical utility functions,
* SOIL2 to load textures in different formats,
* Assimp to load 3D models,
* FreeType to render 2D text characters.

## Features

Below are the main features implemented:
* Solar System simulation in 3D, composed of its spherical celestial bodies (planets, main dwarf planets and moons) moving along their circular orbits, as well as the main belts (asteroid and Kuiper ones), Milky Way in background,
* Camera/Controller/Input system using GLFW3,
* Model Loader for asteroids and Saturn rings using Assimp,
* 3D Mesh Renderer, including instanced rendering to draw the belts efficiently,
* 2D Text renderer using FreeType,
* Phong Illumination model done on the GPU via shaders, for all objects of the scene, with a directional light for the Sun, and point lights for the other celestial bodies.

Comments within the codebase describe more specifically how it works (algorithmics, decisions taken etc...)

## Setup

Project is available on Windows only, on OS architecture x32 and x64.

You can either clone the repository to your computer, or unzip the corresponding .zip folder after downloading it (it will be called solar-system-simulation-master). Then:
* If you just want to run the program, pick the executable <b>OpenGLProject.exe</b> you need according to your OS architecture (stored in <i>OpenGLProject/Output/Release/Win32 or x64/</i>), and drag it to the project main folder (called <i>OpenGLProject/OpenGLProject</i>) before double-clicking on it (otherwise it won't work, the program stating that it didn't find ASSIMP library),
* If you want to run the code, you need to have Visual Studio 17 installed, then go to the project main folder, open <b>OpenGLProject.sln</b> solution, choose the architecture/configuration you want, and click on the green arrow button to launch the simulation.

The execution of the program should require around 3 seconds in Release mode, and 5 seconds in Debug mode before starting. Enjoy!

P.S. If Avast antivirus gives you a warning that there is a threat while the simulation is starting, just ignore it: my program is safe to use :)

## Controls

To be able to explore the Solar System, the user should use the following controls:
* <kbd>Z</kbd>, <kbd>Q</kbd>, <kbd>S</kbd>, <kbd>D</kbd> (for an AZERTY keyboard), <kbd>left mouse button</kbd> and <kbd>right mouse button</kbd> to move forward, to the left, backward, to the right, up and down respectively,
* <kbd>X</kbd> to increase speed when moving,
* <kbd>mouse scroll wheel</kbd> to zoom in/out,
* <kbd>L</kbd> to make celestial body names appear above them,
* <kbd>Up arrow</kbd> and <kbd>down arrow</kbd> to speed up/slow down the simulation,
* <kbd>Space</kbd> to pause/unpause the simulation,
* <kbd>Tab</kbd> to switch the application to cursor mode (to resize the window easily for instance),
* <kbd>Esc</kbd> to quit the simulation.

P.S. Press <kbd>Win</kbd> to background the simulation while it runs.

## Resources

* Scientific data and some models has been retrieved from NASA website mainly (National Space Science Data Center),
* Textures have been downloaded from https://www.solarsystemscope.com/textures/, https://planetpixelemporium.com/planets.html and websites of passionate people,
* Some parts of OpenGL code have been inspired from https://learnopengl.com (from Joey De Vries) and https://www.youtube.com/user/TheChernoProject (from Yan Chernikov, aka TheCherno) mainly; http://www.opengl-tutorial.org and https://open.gl to a lesser extent.
