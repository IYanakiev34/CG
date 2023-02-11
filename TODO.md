# I will list some useful things to be done

- [ ] Familiarize with Qt documentation since we will use this framework. This involves understading how to create widgets and also very nice math classes like Vector3D and Vector2D.
- [ ] Figure out how to model the relationship between VAO,VBO,IBO
- [ ] Create a class for each of the: VAO,VBO,IBO
- [ ] Learn to use the Qt wrapper functions such as QOpenGLShaderProgram. This class can automatically compile and link shaders no need to write on our own.

Once we have VAO,VBO,and IBO we can essentially render any mesh given its vertices,
indices of vertecies. The rest is the shader program and a bit of computational
algorithms which are not too bad, so we need to make sure that these objects
have solid and correct implementation. Other useful object to encapsulate

1. Ray -> object that will represent a ray. Should have direction, position and length
2. Camera -> should have position orientation
3. Mesh -> represneted by VAO,VBO,IBO and later texture + materials
4. Material -> store the properties of a material such as transparency, roughness etc
5. Light -> many different type but a basic one will suffice might become a base class
6. Actor -> general object to represent an actor in the scene
7. Scene -> contains all the actors
8. Texture -> holds a texuture resource
These will most likely be some of the main classes that we will need to implement
throughout the course.


<br></br>
From what I've seen in the qt documentation we should have all the needed
math objects such as:
1. Vector3
2. Vector2
3. Transform
4. Mat3
5. Mat4
6. Quaternion


## TODO Assignment 1
1. Figure out what means to unitize a mesh (probably normalize it but not sure)
2. VAO and BAO should be basically the QOpenGLBuffer and QOpenGLVertexArrayObject
I need to decouple some stuff because too much functionality in the mainview class.
Ideally we will have some mesh object that will have all the data needed for it.
It will have its VAO and VBOs. The we can have Actor which will have a mesh
and a shader. Thus we can draw it since each actor will have position,scale,
and rotation. However, I am not too sure on how to decouple everything so it
works + I need to understand more about the lifecycle of the shader programs
and when to delete them and not same for VAO and VBO. Since once the shader
is in the GPU we can techniaclly delete the shaders and just use the program.
