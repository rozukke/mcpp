## Models

`obj-mc.cpp`

### Usage

Using Blender or any other 3d modeling software, convert your model to a .obj file.

Additionally, the models cannot use quads, so in Blender, triangulate the model using ctrl/command + t

Run `./obj-mc filename.obj scale`

> Scale is the max size of any dimension

### Examples

`./obj-mc blender-monkey.obj 50`

<img src="blender-monkey.png" alt="Blender Monkey" width="200"/>