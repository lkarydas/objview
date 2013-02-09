objview

A 3D viewer for wavefront (.obj) files using OpenGL without immediate mode and GLUT.

# Overview

This project is in a very early stage.

The [GLM library](http://user.xmission.com/~nate/tutors.html) written by Nate Robins is used for loading the .obj files. 

The original library has been extented so that it does not use deprecated OpenGL code: the vertices and normals are loaded into VBOs and rendered using GLSL shaders.

For now the library extension only supports models with single or multiple materials. There is no support for textures yet.
conflicts I renamed the original
Part of the [code](http://www.cs.unm.edu/~angel/BOOK/) provided as supporting material for "Interactive Computer Graphics" by Edward Angel is used to facilitate shader loading and compiling as well as mathematical utilities. In the future, I plan to replace this with [OpenGL Mathematics Library](http://glm.g-truc.net/). In order to avoid potential name conflicts I renamed the original files glm.h and glm.c to libobj.h and libobj.cpp.

<img src="http://i.imgur.com/FLz0ZGP.png?1" alt="objview screenshot">

# How To Compile

Type
<pre>
  make
</pre>
