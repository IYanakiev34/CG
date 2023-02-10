# Coordinate systems
We know that trasnformations can help transform an object either
scale,rotate,etc. We do this and then we still need to be at NDC coordinates.
Pass then to the rasterizer and gte an image. This is done in the vertex shader
however it is usually good to go through several different coordinate systems
before we reach NDC. Why, well some of the math is easier + some of them are
needed for intermediate results and later calculations. The 5 that we will
concern ourslef with are:

1. Local space (object space)
2. World space
3. View space (or Eye space)
4. Clip space
5. Screen space

Those are all a different state at whichi out vertices will be trasnformed
in before finally ending in up as fragments.

## The big picture
To transform coordinates from one space to the next we'll use several
transformation matrices. The most important being: **model, view and projection**.
Our vertex coordinates first start at **local space** and then are processed
to **world space**,**view coordinates**,**clip coordinates** and finally,
**screen coordinates**.

1. Local coordinates
These are the coordinates the object begins at. This means that their are local
to the object. everything from position,rotation and trasnlation is local to
the center of origin of the object at hand.
2. World space
This is the next step of the process. These coordinates are with respect to some
global point of origin. All object in the scene have both local and world space
coordinates.
3. View space
Next we transform these coordinates into view space. This is the coordinates
of the objects are in such a way as if seen from the camera.
4. Clip space
Clip coordinates convert the view coordinates in the -1 to 1 space. This will
clip out what we will not see and if we use projection to clip-space we can even
see the object in perspective.
5. Screen space
This is process called `viewport trasnform` that transforms coordinates in the 
-1 to 1 range to the coordinate range defined by the `glViewport`. The result is
then sent to the rasterizer to turn them into fragments.

### Local Space
This coordinate system is local to your object. Imagine you start a new blender
project with a cube to model. This cube has its own local space coordinates.
In our case the coordinates we used were between -0.5 and 0.5 with orign 0.0.

### World Space
All object will be positioned inside some simulation or game world. This world
has its own origin of (0,0,0). Imagine it like that you have a cube with its
own coordinates but now we need to place it in the world. Thus the world matrix
will scale,rotate, and translate it to the proper position so that it fits
the described scene of the programmer.

### View Space
This is what people call **camera space**. It is the process of transforming the
whole scene. Rotating,scaling,etc in order to position in with respect to the
**eyes** of the camera. Will cover in next chapter how to create a camera system

### Clip space
At the end of each vertex shader OpenGL expect the coordinates to be between
the range of -1 and 1. Everything ouraside of the range is clipped thus why
this space is called clipped space. To transform from a view space to clip
space we need a `projection matrix` that specifies the range of the coordinates
for example -1000 to 1000. The projection convert then coordinates into 
NDC (not directly, a step dcalled Perspective Division sits between them). So
for example if we have a point (1250,750,500) it will not be in the clip space
since the X component is outside of the range. Once we have the clip space
the final step is called **perspective division**. This is the step where we divide
each component of the vector by its homogeneous component w. This transforms
the 4D clip space into normalized 3D space for the NDC. The projection matrix
to transform view coordinates to clip ones usually takes two different forms.
Each has its own frustum. We can have either **orthographic** or **perspective**
projection matrix.

#### How the math works
Here is a good article on how the math works behind these two projection matrices:
http://www.songho.ca/opengl/gl_projectionmatrix.html

## Putting it all together
`Vclip = Mproj x Mview x Mmodel x Vlocal` You might notice that it is done in
reverse order this because we read matrix multiplication from right to left.



