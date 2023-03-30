# Balls.

A 2D Physics simulation featuring balls of the dimensional variety, using OpenGL and C++.

![image](https://user-images.githubusercontent.com/57671477/228808149-4036aa24-d8c4-4419-a84b-aebdbe0511d0.png)

This project is an educational delve into physics simulation, currently with circles, but being built to make adding different collider shapes and collision resolvers trivial.

Aswell as physics, the project will require work on the renderer (particularly with batching), efficent allocation of memory (including keeping hot data dense) and implementations of spatial partitioning.

Current Features:

- a (simple) batch renderer.
- CircleVCircle collisions.
- physics objects can be added and removed with impunity.
- curently supports verlet integration.
- reuse of physics objects via generational IDs.

Being worked on:

- other types of colliders (AABB and capsule being worked on), and efficent runtime branching for each case.
- improvements to batch renderer to allow hands-free flushing when swaping shaders.
- further work on a more generalist component based system, keeping componets dense in memory.
- implementations of spacial partitioning, along with debug visualisers to show how they work (previous attempt scrapped, worked but was far from optimal).

Goals:

- a lightweight api that will allow me to make simple physics based games with it (e.g. plinko)

Current Implementation:

[Demo](https://www.youtube.com/watch?v=aLWAK1GmBm4)

Old implementations (slower, heavy use of polymorphism):

[Demo 1](https://www.youtube.com/watch?v=aFJUTtJ7iYU)

[Demo 2](https://www.youtube.com/watch?v=AKmfHvteXos)

[Demo 3](https://www.youtube.com/watch?v=ac58XjFSvl4)
