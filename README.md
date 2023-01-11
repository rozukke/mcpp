![mcpp logo](/resources/mcpplogosmall.png)
# mcpp (Minecraft++)

mcpp (Minecraft++) is a library built to interface with Minecraft through [Spigot server](https://www.spigotmc.org/) 
running the [ELCI](https://github.com/rozukke/ELCI) plugin and using C++. It is currently limited to MacOS/Linux or Windows with WSL.

## History
This library is based on [mcpi](https://github.com/martinohanlon/mcpi), which is a Python library with similar functionality. 

In addition to C++ support, this library implements several new commands supported by [ELCI](https://github.com/rozukke/ELCI):
- `getBlocksWithData` to get a cuboid of blocks with data in a performant manner,
- `getHeights` to get a 2D area of heights in a performant manner,
- `doCommand` to perform an in-game minecraft command which allows for additional functionality.

This C++ version was created for the Royal Melbourne Institute of Technology (RMIT) to supplement the COSC2084 
(Programming Studio 2) course.

## Installation
**PENDING RELEASE**

Download and build the library yourself (this repository contains some configuration for and was developed using 
JetBrains CLion), or download the most recent release and run `install.sh` on your Linux install, whereupon 
the library should now be accessible in most IDEs through a global `#include <mcpp/mcpp.h>` directive. **The release is
compiled using g++**.
