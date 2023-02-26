# Balls.

A 2D Physics simulation featuring balls of the dimensional variety, using OpenGL and C++.

![image](https://user-images.githubusercontent.com/57671477/220913819-f5fe70cc-ab5e-4442-9a1e-782caf6b5dfa.png)

This project is an educational delve into physics simulation, currently with circles, but being built to make adding different collider shapes and collision resolvers trivial.

Aswell as physics, the project will require work on the renderer (particularly with batching), creating custom allocators to manage memory and implementations of spatial partitioning.

Current Features:

- a (simple) batch renderer.
- CircleVCircle collisions.
- physics objects can be added and removed with impunity.
- curently supports implicit euler and verlet integration.

Planned:

- generic solvers that can be swapped in and out.
- addition of and ability to choose, spatial partioning methods.
- more collider types (already set up for this addition).
- improvements to batch renderer to allow hands-free flushing when swaping shaders.
- possibly a component based system.

[Demo 1](https://www.youtube.com/watch?v=aFJUTtJ7iYU)

[Demo 2](https://www.youtube.com/watch?v=AKmfHvteXos)

[Demo 3](https://www.youtube.com/watch?v=ac58XjFSvl4)
