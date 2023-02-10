# Textures
Textures are usually 2D images that are placed on top of meshes. This gives
the illusion of detail without adding extra vertices to the object. In order
to map textures to triangles we need to tell each vertex which texture coordinate
it should have. After that fragmentation interpolation occurs and does the rest of
the work for us. Texture coordinates range from 0 to 1 in both the x and y axis.

## Texture wrapping
If we pass coordinates outside of the (0,1) region OpenGL will ignore
the integer part of the thing we pass in. For example if we pass in 10.4.
We will use 0.4. We have 4 options for textures:

1. `GL_REPEAT`: Defualt behavior of textures it will repeat the texture image.
2. `GL_MIRRORED_REPEAT`: Same as `GL_REPEAT` but mirros the image on each repeat.
3. `GL_CLAMP_TO_EDGE`: Clamp cooridnates resulting in higher coordinates to become clamped to the edge, which leads to stretched patterns.
4. `GL_CLAMP_TO_BORDER`: Coordinates outside of the range are given user specified border color.

Each of the mentioned options can be set per texture axis (s,t,r) r is used for
3D textures. Example of this is shown:
```C++
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);
```
The first argument sepcifies the **texture target** in our case a 2D one.
The second parameter requirs us to say what do we want to set and for which
axis do we want to set it up. The last argument is the texture wrapping mode.
If we pass for example **GL_CLAMP_TO_BORDER** we also need to sepcify a border
color in order to do that we need to use a different function:
```C++
    float borderColor[] = {1.0f,1.0f,0.f,1.f };
    glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,borderColor);
```

## Texture filtering
Texture coordinates are independent of the resolution, thus we can pass in any
float to opengl. Then OpenGL has to figure out which texture pixel (also known
as **texel**) to map to this coordinate. OpenGL has several options available
but we will discuss the following:
1. `GL_NEAREST` - Default mode, OpenGL will select the closest texel to the coordinate
2. `GL_LINEAR` - Takes the approximate color from filtering the nearest neighbours.
GL_NEAREST will result in more of a block pattern where we can clearly distinguish
between edges and pixels. GL_LINEAR will produce something smoother with less
borders and roughness to the texture. Texture filtering can be set for both
magnifying and minifying operations. Thus when we look from afar we might want to
have GL_NEAREST but when we upscale we will want GL_LINEAR. This can be done as
follows:

```C++
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
```

### Mipmaps
Imagine a scene where we have the same texture hundreds of times. However, in
some cases the object may be very far away, OpenGL will have trouble sampling
a high resolution texture. Thus we need a smaller one. We can solve this problem
using mipmaps that OpenGL will generate for us. This will save memory and computation.
Each mipmap will be twice as small as the previous one. OpenGL will also automaticaly
select the best mipmap needed for the specific object. This could be done for 
an example texture as follows:
```C++
    glGenerateMipmap(texture);
```
We can also determine for which mimpap how we want to process it using the
GL_NEAREEST or GL_LINEAR filtering. This can be done on all levels of the mipmap.
Example:
```C++
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
```
Example of going through the process of generating a texture, binding it and generating
a mipmap for it:
```C++
    unsigned int tex;
    glGenTexture(GL_TEXTURE_2D,&tex);
    glBindTexture(GL_TEXTURE_2D,texture);
    // assuming we have loaded the texture in the CPU
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
    glGenerateMipmap(GL_TEXTURE_2D); // generate mipmap for the currently bound texture
```
1. GL_TEXTURE_2D - what type of texture
2. 0 - mipmap level for which we want to generate texture. We can go manually over them for now leave it at 0
3. GL_RBG - what format we want to store the texture
4. width, height - The dimensions of the image
5. 7 and 8 - specify the format of the image in our case GL_RGB + GL_UNSIGNED_BYTE for the data type
6. data - the data pointer

glTexImage2D means that the currently bound texture object will now have a texture assigned to it.
The mimap call will automatically generate all the required levels of mipmaps needed
for the texture. After we generate the texture we can free the memory on the CPU
since it is no longer needed. The whole process can be seen as follows:

```C++
    unsigned int texture;
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D,texture);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    int width,height;
    unsigned char *data = // load data;

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,GL_RGB,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture\n";
    }

    free_image(data);
```
In order to apply texture to the triangle we need to update the vertex array.
We need to add 2 float more for the texture coordinates. Which then means that
we need to update the vertex attributes since now the stride is 8.

```C++
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE, 8 * sizeof(float),(void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);
```
```GLSL
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    layout (location = 2) in vec2 aTex;

    out vec3 ourColor;
    out vec2 texCoord;

    void main()
    {
        gl_Position = vec4(aPos,1);
        outColor = aColor;
        texCoord = aTex;
    }

    #version 330 core
    out vec4 fragColor;

    in vec3 ourColor;
    in vec2 texCoord;

    uniform sampler2D outTexture;
    void main()
    {
        fragColor = texture(outTexture,texCoord);
    }
```
The sampler can automatically sample a texture. GLSL has a function texture()
which takes as a parameter a texture and a texture coordinate to sample for color.
The output color is the texel, then filtered and interpolated. In order
to draw the triangle with the texture we simply do:
```C++
    glBindTexture(GL_TEXTURE_2D,texture);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
```
The reason we didnt need to set the uniform sampler is because if we have only 
one texture in the shader it can automatically infer to sample from it however
if we have more we need to set it in order to know which to sample from.
OpenGL has something called **texture units** if we wish to perform an operation
on a specific unit we need to set it active before we bind it.
```C++
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,texture);
```
OpenGL should have at minimum 16 texture units we can sepcify one for example
`GL_TEXTURE8` or even `GL_TEXTURE0 + 8` this can be very useful while iterating
over units. However we need as much samplers as texture so we would need
something like:
```GLSL
    uniform sampler2D texture1;
    uniform sampler2D texture2;
    //
    //
    //
    void main()
    {
        fragColor = mix(texure(texture1,texCoord),texture(texture2,texCoord),0.2);
    }

```
For example here the final output is combination of 2 textures using the GLSL
built-in mix function. It takes 2 arguments and linearly interpolates between them
based on its third argument. Since we have 0.2 we will return 80% of the first input
mixed with 20% of the second input.




