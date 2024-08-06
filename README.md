![mcpp logo](https://raw.githubusercontent.com/rozukke/mcpp/main/resources/mcpplogosmall.png)


# mcpp
[![made-with-cpp](https://img.shields.io/badge/Made%20with-C++_17-1f425f.svg)](https://cplusplus.com/) [![made-by-rozukke](https://img.shields.io/badge/Creator-rozukke-f497af.svg)](https://github.com/rozukke) 
[![GitHub license](https://img.shields.io/github/license/rozukke/mcpp.svg)](https://github.com/rozukke/mcpp/blob/main/LICENSE)
[![Build status](https://github.com/rozukke/mcpp/actions/workflows/ci.yml/badge.svg?branch=main&event=push)](https://github.com/rozukke/mcpp/actions/workflows/ci.yml)

**mcpp** (Minecraft++) is a library built to interface with Minecraft through [Spigot server](https://www.spigotmc.org/) 
running the [ELCI](https://github.com/rozukke/ELCI) plugin and using C++. It was primarily designed as a teaching tool, and is tested on MacOS/Linux or Windows with WSL.

## Documentation
➡ Access the docs for this repo at the [mcpp documentation site](https://rozukke.github.io/mcpp/).

➡ For more details on the broad strokes of **mcpp**, refer to the [wiki](https://github.com/rozukke/mcpp/wiki/Index)!

## History
This library is based on [mcpi](https://github.com/martinohanlon/mcpi), which is a Python library with similar functionality. 

In addition to C++ support, this library implements several new commands supported by [ELCI](https://github.com/rozukke/ELCI):
- `getBlocks` to get a cuboid of blocks with modifiers in a performant manner,
- `getHeights` to get a 2D area of heights in a performant manner,
- `doCommand` to perform an in-game minecraft command which allows for additional functionality.

This C++ version was created for the Royal Melbourne Institute of Technology (RMIT) to supplement the **COSC2084** 
(Programming Studio 2) course.

## Installation
### Windows
Since the release is intended for UNIX systems, the recommendation it to do as follows:
- Install WSL, and then follow the below instructions for Linux.
- Depending on your WSL version, you may need to specify a connection address manually in `MinecraftConnection()` if you are running Spigot (with ELCI) on Windows, which you can find using `nslookup "$(hostname).local"` on your WSL terminal. The default port for ELCI is `4711`. As such, it's recommended to also run the server under WSL.

### MacOS and Linux
- Install `g++`, `make`, `cmake` and any other build prerequisites
- Run the following commands:
```shell
git clone https://github.com/rozukke/mcpp.git && cd mcpp
cmake -B build && cd build
sudo make install
```
- After doing this, the library should be accessible via a `#include <mcpp/mcpp.h>` directive. 
- When compiling code using the library, use the flag `-lmcpp` for Makefiles or `target_link_libraries(your_executable mcpp)` for CMake.

## Contributors

<a href="https://github.com/rozukke/mcpp/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=rozukke/mcpp" />
</a>

## License
**The following applies to all relevant code and resource files:**

Copyright 2024 Artemis Rosman and contributors

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.


