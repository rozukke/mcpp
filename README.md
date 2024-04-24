![mcpp logo](/resources/mcpplogosmall.png)
# mcpp (Minecraft++)

**mcpp** (Minecraft++) is a library built to interface with Minecraft through [Spigot server](https://www.spigotmc.org/) 
running the [ELCI](https://github.com/rozukke/ELCI) plugin and using C++. It is currently limited to MacOS/Linux or Windows with WSL.

➡ For more details on the broad strokes of **mcpp**, refer to the [wiki](https://github.com/rozukke/mcpp/wiki/Index)!

## History
This library is based on [mcpi](https://github.com/martinohanlon/mcpi), which is a Python library with similar functionality. 

In addition to C++ support, this library implements several new commands supported by [ELCI](https://github.com/rozukke/ELCI):
- `getBlocks` to get a cuboid of blocks with modifiers in a performant manner,
- `getHeights` to get a 2D area of heights in a performant manner,
- `doCommand` to perform an in-game minecraft command which allows for additional functionality.

This C++ version was created for the Royal Melbourne Institute of Technology (RMIT) to supplement the COSC2084 
(Programming Studio 2) course.

## Installation
**The release is compiled using g++ and is intended for use with UNIX systems.**
### Windows
Since the release is intended for UNIX systems and not tested on Windows, the options to run locally are as follows:
- Build manually (there are included CMake build files)
- Run with WSL, in which case follow the Linux instructions below. You may need to specify a connection address manually in `MinecraftConnection()` if you are running Spigot with ELCI on Windows, which you can find using `nslookup "$(hostname).local"` on your WSL terminal. The default port for ELCI is `4711`.

### Mac and Linux
- Install `g++`, `make`, `cmake` and any other build prerequisites
- Clone this repository via `git clone https://github.com/rozukke/mcpp.git`
- Install by running `cmake .` and `sudo make install` inside the cloned directory.
- After doing this, the library should be accessible via a `#include <mcpp/mcpp.h>` directive. 
- When compiling code using the library, use the flag `-lmcpp` for Makefiles or `target_link_libraries(your_executable mcpp)` for CMake.


