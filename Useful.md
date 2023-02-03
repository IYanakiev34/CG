# I'll leave here some useful things to search and explore

1. VBO,VAO,FBO
These are types of array objects that are usually good to be abstracted away
in cpp code through classes. These object are basically the information that
you will pass to the GPU in order to render a mesh. Having them abstracted away
is super good since you write code ones and can basically render any object. I
think this will basically be the heavier openGL code from thre it should be easy

2. Rendering pipeline
It is important to know there are 2 main shader types. A shader is a program
that runs on the GPU. The code looks similar to C++ but it is not. It is GLSL.
These program run for a specified object. So lets say you want to draw a circle.
You load the mesh data to the GPU and then bind a shader to this object.

### Shaders
A shader program has many different types of shaders. The 2 most useful ones
are:
1. Vertex shader
2. Fragment shader
Vertex shaders run once for each vertex. These shaders usually just clculate
the position on the screen for the pixels. These shaders draw the triangles on the
screen in the proper positions. Fragment shaders run once per pixel. These often
calculate stuff like color, reflections,light,etc. It is important to know
that they run in order so first vertex then fragment. Shaders can also
pass information between one another. This could be useful in many scenarios. Shaders
again are usually abstracted in cpp through a class Shader that will expose 
functionality such as loading a shader, creating a shader program, binding a shader
to be active.

### Useful resources
I think the most useful resource is a book called Game Programming in C++ by Sanjay Madhav
professor at USC. Chapters 3,5,6 I think are enough for the basics. 3 is just math
so vectors,matrices,cross product,dot product etc. 5 is openGl he explain it much
better than me. 6 is textures and more math. He explain how to porject stuff on
screen and the math that goes behind calculating the porper coordinates on screen
with respect to the camera position. He also talks about UV coordinates how to map
texture to object etc. It is a good resource also because he provides a free plugin
for unreal engine where you can basically download mesh data,animation data etc,
reformat it in json like style and use it in OpenGL. The formatting from what I've
seen is what we will be using. Which is very nice if we want to test code on more
complex objects not only spheres and cubes.



