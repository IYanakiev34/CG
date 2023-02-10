# OpenGL notes
Section 1 will be from a windows to a triangle explaining the pipeline
of OpenGL. This whole document will be based on the `learnopengl.com` series
online. I try to make it shorter and the main reaosn I write it is so that I
can learn it better. I try to make the summaries good but their website is
much better I beleive than what I have to offer.

## From window to a trinagle
OpenGL can be split up into 2 major parts
1. Convert 3D coordinates to 2D (in your screen)
2. Determine the color of each pixel on the screen
We have teh following steps:
1. Vertex Shader
2. Shape Assembly
    - Could be GL_POINTS,GL_TRIANGLES,GL_LINE_STRIP,etc. We tell OpenGL how we want to process these points in the shape assembly.
3. Geometry Shader (optional) this shader might create new geometry (add/remove vertices)
4. Test and blending (more on that later)
5. Fragment shader (performed once for each pixel)
6. Rasterization
Important note it that before the fragment shader runs, clipping is performed.
Thus we won't render anything that is not on the screen improving performance.

### Vertex Shader
Main perpose of the vertex shader is to take in some 3D coordianates and transform
them into another type of 3D (or 2D) coordinates. This is where for example
the view projection matrix should be used.

### Fragment shader
The main purpose of this shader is to calculate the color of each pixel. This
shader will do most of the work and will most likely need a lot of infromation
in order to perform all the needed computations. Usually contains data about
the 3D scene (lights,colors,color of light,shadows etc).

### Alpha test + blending
Will get to those later but basically determines if the object is in-front of or
in the back of something and blends the object accordingly.

## Vertex input
We need coordinates in 3D space (even if we have 2D game for OpenGL we treate it
as 3D, we can simply put the Z-axis as 0). It doesn't simply transform the coordinates
you give it from 3D to 2D. They have to be **normalized**.  The screen of your 
computer/phone or whatever operates on **Normalized Device Cooridantes**. This
coordinate space if from -1 to 1 on all axis (meaning **x and y**). Usually
we will pass in coordinates that are not in this format and transform them inside
the shader in order to get the position of each vertex. However, for the sake
of drawing triangle we can do something like this:

```C++
    float vertices[] = {
        -0.5f,-0.5f,0.0f, // left vertex
        0.5f,-0.5f,0.0f, // right vertex
        0.f,0.5f,0.f, // top vertex
```
Since the `z-component` of the triangle is 0 we make it look like the depth
of it is the same thus it makes it look like we are in 2D not 3D.

### VBO object
We manage data in these objects. These objects send batch data to the GPU
which **is very very fast**. These object have unique OpenGL **id** and are
generated via:
```C++
    unsigned int VBO; // or GLuint vbo
    glGenBuffers(1,&VBO);
```
After we generate it we need to bind it in order to make it active.
```C++
    glBindBuffers(GL_ARRAY_BUFFER,VBO);
```
After we created the buffer and binded it we need to actually send the data to
the GPU.
```C++
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
```
This puts data in to buffer that is currently bound. We have 3 types of draws:
1. GL_STATIC_DRAW - meaning data is set only once and used many times
2. GL_STREAM_DRAW - data is set once and used by the GPU at most **a few** times.
3. GL_DYNAMIC_DRAW - the data is changed a lot and used many times.
Since our case the triangle position will not change static draw can be used.
If we have buffer that would change a lot we can opt in to use **GL_DYNAMIC_DRAW**

## Shaders
In modern OpenGL we need at least 2 shaders: **vertex** and **fragment**. We need
to load those into memory,compile each shader and then create a shader program.
### Example of vertex shader (simplest one)
```GLSL
    #version 330 core // version of openGL

    layout (location = 0) in vec3 aPos; // the location in the buffer

    void main()
    {
        gl_Position = vec4(aPos,1.f)
    }
```
gl_Position is the output position of the vertices, since we already pass NDC
cooridantes we only need to create the gl_Position for each vertex. The `gl_Position`
behind the scenes is **vec4** so we can just add one more **1** to the end. This
is the homogenous coordinate, very important concept.

### Compiling a Shader
We need to place the shader in a C-string
```C++
    const char *vertexShaderSource = "#version 330 core .... rest of shader code";

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
    glCompileShader(vertexShader);
```
You probably want to check if the shader has compiled correctly. You can do that via:

```C++
    int success;
    char infoLog[512];
    glGenShaderiv(vertexShader,GL_COMPILE_STATUS,&success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader,512,NULL,&success);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_ERROR\n";
    }
```

### Fragment shaders
```GLSL
    #version 330 core
    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(1,0.5,0.2,1);
    }
```

The again the same procedure to compile it

```C++
    unsigned int fragmentShader;
    const char *shaderSource = "... shaderr source"; 
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&shaderSouce,NULL);
    glCompileShader(fragmentShader);
```
Finally we need a shader program. This program gets all the little
shaders and make the actual final shader program.

```C++
    unsigned int shaderProg;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);
```
We can again check if the program has linked correctly via:

```C++
    glgetProgramiv(shaderProgram,GL_LINK_STATUS,&success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram,512,NULL,infoLog);
    }
```
Finally we can use the program + we need to delete the shader programs
that we have linked since we do not need them anymore.
```C++
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
```
Nice so now we have passed the data and made the shader however finally we need
to instruct the GPU how to interpret the data that we have passed it. We can
do that with specific vertex arrtributes.

```C++
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),nullptr);
    glEnableVertexAttribArray(0);
```
We specify that the first part of the vertex will be the position. So we say
1. 0 - index of attribute
2. 3 - how many elements we will pass in
3. GL_FLOAT - The type of the elements.
4. GL_FALSE - This means we do not want to normalize them
5. size - The size of the data that we have there 
6. nullptr - this is the offset of where the data begins since we are at the 0th index we do not have offset we can leave it as nullptr.

We also want to enable the vertex attribute since by default they are not enabled.
This process can be very tidious if we have a lof of vertex array pointers. It
would be nice if we could have an object that we could just bind and release
and remember the state of all this. This is why we have **VAO** objects.

#### Vertex Array object
It can be bound just like any VBO. The subsequent calls to the atributes
will be stored in the VAO. This has the advantage that we only need to make
these calls once when we configure the VBO and then switching between different
object is as easy as switching the currently bound VAO object.

```C++
    unsigned int VAO;
    glGenVertexArrays(1,&VAO);

    glBindVertexArray(VAO);
```
This creates and bind the VAO. So the process finally comes together. What do 
we need?
1. Create the VAO
2. Bind the VAO
3. Create,bind, and buffer data into the VBO's that we will use
4. Create the vertex attrib pointer and specify how the data is layed out in memory
5. Get the shaders
6. Compile the shaders
7. Link the shaders into one shader program
8. Make the current shader program active
9. Draw something on the screen

```C++
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES,0,3);
```

1. GL_TRINAGLES = What we want to draw
2. 0 - Starting index of the vertex
3. 3 - vertices we wish to draw

## EBO (Element buffer object or IBO also known)
Consider we wish to draw a rectangle. What you will find out is that we have
duplication of vertices. What if we could avoid sending duplicate data to the GPU.
One thing that we could do is create another array that specifes which vertices
should each triangle use. This is also know as **ELEMENT BUFFER OBJECT**

```C++
    unsigned int EBO;
    glGenBuffer(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
```
Finally we have to remove the `glDrawArrays` call.

```C++
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glDrawElements(GL_TRINAGLES,6,GL_UNSIGNED_INT,0);
```
1. GL_TRIANGLES - The mode we wish to draw in
2. 6 - The amount of vertices we will draw
3. GL_UNSIGNED_INT - The type of the indices
4. 0 - If not using EBO pass indices now we dont need it

We can verify that we have 2 triangle if we render not in FILL mode
but in LINE mode.

```C++
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    // reset to normal mode when you need to
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
```

