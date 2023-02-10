# Shaders + GLSL
Shaders are isolated programs that run on the GPU. They are isolated
meaning no communication between them. Only in terms of inputs and outputs
they can coomunicate.

## GLSL
This is the language of the shader programs. They consist of 4 basic things.
1. Inputs
2. Outputs
3. Uniforms
4. main function
When talking about vertex shader each input is basically a vertex attribute
(except the uniforms). Based on the hardware we might be limited by how many
attributes we can attach. OpenGL guarantees 16 4-component attributes.
We can query how many using: `GL_MAX_VERTEX_ATTRIBS`.
```C++
    int attNum;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS,&attNum);
    std::cout << "Number of max attributes: " << attNum << "\n";
```
### Types in GLSL
Basic ones like: int,float,double,uint,bool. We also have containers
such a vectors and matrices which are very important.

1. `vecn` : vector of n floats
2. `bvecn`: vector of n booleans
3. `ivecn`: vector of n integers
4. `uvecn`: vector of n unsigned integers
5. `dvecn`: vector of n doubles
Some interesting properties of GLSL is that we allow for swizzling meaning
we can do something like this:

```GLSL
    vec4 out;
    out.xxyz +=3;
```
We can access component like this and also acces one component multiple times.

### Ins and Outs
We can pass input to shaders (vertex shaders) using their atrtibutes. We can also
have `out` variables. Whenever the `in` variable has the same name as the `out`
variable it can be used from the previous shader. For example we can do that
to use information in the fragment shader from the vertex shader.

### Special variables
The fragment shader needs a `vec4` out for the color. Otherwise we will not output
any color.

### Examples
Example of vertex shader
```GLSL
    #version 330 core
    layout (location = 0) in vec3 aPos;

    out vec4 vertexColor;

    void main()
    {
        gl_Position = vec4(aPos,1.f);
        vertexColor = vec4(0.5,0,0,1);
    }
```
```GLSL
    #version 330 core
    in vec4 vertexColor;
    out vec4 fragColor;

    void main()
    {
        fragColor = vertexColor;
    }
```
In this example we have successfully passed information from the vertex shader
to the fragment shader sinc in the vertex shader we have out variable with a color
and in the fragment shader we have in variable of the same type and name.

## Uniforms
Uniforms are different since they are once again a way to pass information to the shader
however they are not vertex attributes. They are set by the C++ code though. 
Uniforms are:
1. Global : the whole `shader program` can use them. Meaning if we have vertex,fragment, and geometry shader all of the can use a uniform.
2. Keep their value : uniforms will keep their value until being reset or updated.
Even though uniforms are global they need to be defined in each shader if we wish
to use them their. They need to have the same name and type otherwise it will be
treated as a different uniform variable.
### Example
```GLSL
    #version 330 core
    out vec4 fragColor;

    uniform vec4 myColor;
    void main()
    {
        fragColor = myColor;
    }
```
From C++ we can do the following to set the uniform:
```C++
    float timeValue = glfwGetTime();
    float greenValue = (sin(time) /2.0f) + 0.5;
    int vertexColorLocation = glGetUniformLocation(shaderProgram,"myColor");
    glUseProgram(shaderProgram);
    glUniform4f(vertexColorLocation,0.0f,greenValue,0.0f,0.0f);
```

## Adding color to vertex attributes
Lets assume now each vertex consists of 6 floats:
1. 3 for position
2. 3 for color
How can we achieve this, pass infromation to the vertex shader which will
in tern pass information to the fragment shader.

```C++
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 6 * sizeof(float),nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE, 6 * sizeof(float),(void *) 3 * sizeof(float));
    glEnableVertexAttribArray(0);
```
```GLSL
    #vesion 330 core
    // Vertex shader
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;

    out vec3 ourColor;

    void main()
    {
        gl_Position = vec4(aPos,1);
        outColor = aColor;
    }

    #version 330 core
    // Fragment shader code

    in vec3 ourColor;
    out vec4 fragColor;

    void main
    {
        fragColor = vec4(ourColor,1);
    }
```
Here we can see we use the 2 vertex attribute pointer that we have made in the
VBO object. We also see how we pass information between the shaders.
The reason that on the first call we have `nullptr` and on the second we see:
`(void *) 3*sizeof(float)` is because this specified the **offset** meaning how
many elements do we need to skip in order to get to the specific property. In
both cases we can see we have a **stride** of: `6 * sizeof(float)` since we have
to move 6 floats in order to get to the next vertex with this specific attribute.
A good idea in OpenGL is to abstract the process of a shader in its own class.
I will mock out the interface here:

```C++
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int d_id;

    Shader(char const *vertexPath, char const *fragmentPath);
    void use();
    void setBool(std::string const &name, bool value) const;
    void setInt(std::string const &name, int value) const;
    ..
    ..
    ..

};

#endif

```



