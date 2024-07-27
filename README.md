objview
==================

A 3D viewer for wavefront (.obj) files using OpenGL (no immediate mode) and GLUT.

# Overview

This project is at an early stage.

The [GLM library](http://user.xmission.com/~nate/tutors.html) written by Nate Robins is used for loading the .obj files. 

The original library has been extented so that it does not use deprecated OpenGL code: the vertices and normals are loaded into VBOs and rendered using GLSL shaders.

For now the library extension only supports models with single or multiple materials. There is no support for textures yet.
Part of the [code](http://www.cs.unm.edu/~angel/BOOK/) provided as supporting material for "Interactive Computer Graphics" by Edward Angel was initialy used to facilitate shader loading and compiling as well as mathematical utilities. This library has been replaced with [OpenGL Mathematics Library](http://glm.g-truc.net/). In order to avoid potential name conflicts I renamed the original files glm.h and glm.c to libobj.h and libobj.cpp.

<img src="http://i.imgur.com/dopsneA.png" alt="Screenshot1" border="6" height="350" width="420">
<img src="http://i.imgur.com/Acc6cw2.png" alt="Screenshot2" border="6" height="350" width="420">

# Dependencies

Install dependencies from Debina/Ubuntu packages:

```shell
sudo apt install libglew-dev libglut-dev libglm-dev
```

# How To Compile

```shell
make
```
