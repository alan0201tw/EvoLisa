# EvoLisa

![reference](/assets/mona-200.bmp)

Using polygons with 4 vertices

Batch1 : 11000 iterations
![result](/gif/evolisa_v4.gif)

Batch2 : 42000 iterations
![result](/gif/evolisa_v4_b2.gif)

Using polygons with 6 vertices

15000 iterations
![result](/gif/evolisa_v6.gif)

Using polygons with 8 vertices

15500 iterations
![result](/gif/evolisa_v8.gif)

This little project is inspired by [this](https://users.cg.tuwien.ac.at/zsolnai/gfx/mona_lisa_parallel_genetic_algorithm/) post about the EvoLisa problem, which is mentioned in this youtube video : [Painting the Mona Lisa...With Triangles!](https://www.youtube.com/watch?v=JmVQJg-glYA).

The original post for this problem by Roger Alsing can be found [here](https://rogerjohansson.blog/2008/12/07/genetic-programming-evolution-of-mona-lisa/)

Also, please notice that the result can vary a lot based on the quality of initial conditions, which in this case are random. Above listed results are some of the better ones.

## Implementation Detail

* Graphics
    * OpenGL : glad, glfw
    * Image IO : stb_image, stb_image_write
* Math : HandmadeMath
* Build system : premake

