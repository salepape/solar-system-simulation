# Solar System Simulation

## Introduction

This project is the main one after obtaining my double diploma in Computer Science. Some comments within the code describes more precisely how it works (algorithmic, decisions taken and so on). The project is still under development in order to learn new OpenGL concepts and make the simulation even more realistic. 

## Installation

Just download the .zip project folder or clone the repo to your computer. Then click solar-system-simulation, OpenGLProject, Visual Studio project (OpenGLProject.sln). To finish, click on the green arrow button to launch the simulation. Compilation and execution may dure about some seconds according to your computer.

## Features

The user can move within a reconstructed Solar System composed of its celestial bodies (planets, main dwarf planets and moons) moving along their orbits, as well as the main belts (asteroid and Kuiper ones). It is also possible to make names of celestial bodies appear on the top of each. 

## Controls

To be able to discover the Solar System, the user should use the following basic controls :
* <kbd>Z</kbd>, <kbd>Q</kbd>, <kbd>S</kbd>, <kbd>D</kbd>, <kbd>left mouse button</kbd>, <kbd>right mouse button</kbd> to move the camera respectively forward, backward, left, right, up or down;
* <kbd>X</kbd> to increase camera speed when moving;
* <kbd>mouse scroll wheel</kbd> to zoom in / out on the scene;
* <kbd>up arrow</kbd> and <kbd>down arrow</kbd> to accelerate or decelerate the simulation;
* <kbd>space</kbd> to pause the simulation and make information appear;
* <kbd>Esc</kbd> to quit the simulation.

## Technologies

The project is developed under Visual Studio 17 with Windows 10. It uses :
* OpenGL 3.2 (core profile) : C / C++ code + HLSL shaders;
* GLFW3 library;
* Glad;
* Assimp library; 
* FreeType library;
* GLM library;
* SOIL2 library.

## Resources

* Scientific data have been retrieved from NASA website mainly (National Space Science Data Center);
* Textures have been downloaded from https://www.solarsystemscope.com/textures/, http://planetpixelemporium.com/planets.html and websites of passionate people;
* Some parts of OpenGL code have been inspired from https://learnopengl.com/, http://www.opengl-tutorial.org/ and https://open.gl/ mainly.
