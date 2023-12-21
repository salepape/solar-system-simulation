# Solar System Simulation

## Introduction

This project is the main one I realised after obtaining my double diploma in Computer Science. Its main objective was to learn about OpenGL and the modern rendering techniques. To do that, I decided to model and simulate the 3D Solar System. The project is still under development, and hopefully will make the simulation more and more realistic. 
Some comments within the codebase describe more precisely how it works (algorithmics, decisions taken and so on..)

## Features

The user can move within a reconstructed Solar System composed of its celestial bodies (planets, main dwarf planets and moons) moving along their orbits, as well as the main belts (asteroid and Kuiper ones), Milky Way in background. It is also possible to make celestial body names appear at the top of each of then by pausing the simulation. 

## Controls

To be able to discover the Solar System, the user should use the following controls:
* <kbd>Z</kbd>, <kbd>Q</kbd>, <kbd>S</kbd>, <kbd>D</kbd>, <kbd>left mouse button</kbd>, <kbd>right mouse button</kbd> to move the camera respectively forward, backward, left, right, up or down,
* <kbd>X</kbd> to increase camera speed when moving,
* <kbd>mouse scroll wheel</kbd> to zoom in/out on the scene,
* <kbd>up arrow</kbd> and <kbd>down arrow</kbd> to accelerate or decelerate the simulation,
* <kbd>space</kbd> to pause the simulation and make information appear,
* <kbd>Esc</kbd> to quit the simulation.

## Technologies

The project is developed under Visual Studio (VS) 17 and Windows 10 environment. It uses OpenGL 3.2 (core profile) API, C/C++ code + GLSL shaders, as well as a set of libraries:
* GLFW3 to intialise a window and input handling,
* Glad to get function pointers,
* GLM for maths,
* FreeType to render text characters,
* SOIL2 to load textures in different formats,
* Assimp to load models.

## Setup

You can either clone the repo to your computer or download the .zip file. Then:
* If you want to run an executable, drag one of them (stored in OpenGLProject/Output, according to the computer architecture/build type) to the main VS folder (OpenGLProject/OpenGLProject) before double-clicking on it,
* If you have Visual Studio installed, you can go to OpenGLProject/OpenGLProject and open OpenGLProject.sln, then click on the green arrow button to launch the simulation. 
Compilation and execution of the program should require about 5 seconds before starting. Enjoy!

## Resources

* Scientific data and some models has been retrieved from NASA website mainly (National Space Science Data Center),
* Textures have been downloaded from https://www.solarsystemscope.com/textures/, http://planetpixelemporium.com/planets.html and websites of passionate people,
* Some parts of OpenGL code have been inspired from https://learnopengl.com/ (from Joey De Vries) and https://youtu.be/W3gAzLwfIP0?feature=shared (from Yan Chernikov, aka TheCherno) mainly; http://www.opengl-tutorial.org/ and https://open.gl/ to a lesser extent.
