# Hypergraphs

## Table of contents

- [The project](#the-project)
- [DISCLAIMER](#disclaimer)
- [Demo](#demo)
- [Installation guide](#installation-guide)
- [Controls](#controls)
- [Explanaton and 3D comparison](#explanation-and-3d-comparison)
- [Adding shapes](#adding-shapes)

<br>

## The project
This project aims to give the user a better understanding of a 4D environment.

To do so, a 4D hyperspace is projected (or "flattened") onto a 3D space.

The user can freely navigate in the 4 directions and rotate along the 6 possible rotations in 4D. 

<br>

## DISCLAIMER
I am not an expert. Some rotations, movements, projections etc might be wrong, i wouldn't know as I usually evolve in a 3D environment.

That said, everything seems coherent to me, based purely on how it looks while moving around.

Please open an issue if you find a problem.

<br>

## Demo
https://github.com/user-attachments/assets/e1a4e34f-f8c9-415a-b4b9-e3f94a235ebd

<br>

## Installation guide

### Windows
1. Click on the `hypergraphs_windows.zip` file above.
2. Click the `Download raw file` button on the top right.
3. You should now have a zip folder, extract it.
4. Now simply double click the `hypergraphs` executable.

If it doesn't work please open an issue, I'm unsure about how Windows works.

### Linux
So you use linux huh ? I guess you already know what you're doing :)

You'll have to install Raylib, sorry about that.
I might include raylib later, haven't looked into it yet.

You can follow ![these instructions](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux#install-on-gnu-linux) though, that should be easy.

Then download the repo, `cd`, and `make`.
 You should be able to run the program with `./hypergraphs`

<br>

## Controls
Press escape to quit

### Moving
W, A, S, D, LShift, Space for classic movement in 3D. Respectively: forwards, left, backwards, right, down, up

Q and E to move "in" and "out" (in 4D)

### Rotations
Mouse and mouse wheel to look around in 3D

Control + Mouse and mouse wheel to look around in 4D ***(this is the interesting part)***

<br>

## Explanation and 3D comparison
To give you a better understanding of what's going on, let's try to take everything one dimension down.
Imagine a 3D space, in which there is an infinite plane. There is a character moving freely in that 2D plane. Let's call him Mario.

Mario can move along 2 directions (front/back and up/down) and rotate around 1 point (look up / look down).

If we want to show Mario a cube, I believe the best way is to flatten (orthogonal projection) our cube onto the 2D plane.
Mario will then be able to move around the cube "shadow" freely.
We also give mario the ability to rotate aroud other axis, so he can effectively rotate his plane inside our 3D world. this way he'll be able to see the cube from every possible angle.

So if the cube is aligned in a certain way, the shadow of it will be a simple square, and Mario will see a square. He can move freely around this square and it stays a square. But now,if he decides to rotate his plane into our world, the shadow will deform and give him the impression of 2 squares, connected by the corners (the left face and the right face).

Take it back 1 dimension up.
At the start, the hypercube is aligned with our 3D space such that its shadow is just a simple cube. We can move around it freely. But we can also rotate our 3D space into the 4th dimension, and it becomes 2 cubes, connected by the corners (see the video above).
It's the same thing.

There are a lot of interesting possible shadows, I recommend spending some time moving around to understand what's going on !

<br>

## Adding shapes
If you're interested in adding more shapes, you'll need to program a bit. AI can probably make the process easier if you have no idea how, but the process should be simple as there are already two shapes that you can copy.

To add a shape you only modify the `shapes.c` file:

In the `init_shapes` function, copy what is done for the other 2 shapes and replace what is necessary.

Create a new function and program the shape as you want (the `make_pyramid` function is a good example)

### Now you have to compile it again

On linux just `make` it again.

On windows you have to `make` it too, but you have to use a terminal, probably WSL, I'm not sure, look into it... (sorry)
