# Lights and Shadows

### Ambient Reflections

This type of light is modelled simply
by a coefficient called `Ia`

### Diffues Reflections

This about different surfaces. For example a wooden
surface will not emmit as much light as a mirror one.
The diffuse component models the `roughness` of the material
and how light bounces. This is modelled via an angle between
the **light source** and the **normal vector**. The angle could
be obtained by getting the dot product between the two vector
assuming we have **normalized** them. We perform one more thing
which is clamping it between 0 and the angle. Since if we have
negative cosine we just want to have 0 not negative values.So for now our light
look like this: `I = Ia + Id` where `Id` is the diffuse component.

### Specular Reflections

Shiny objects have highlights. These highlights depend on the angle in which
we view and object, thus in the specular reflections we should account
for this angle. The model for `Is` becomes the following: `Is = max(0,R.V)^p`,
where `R` is the perfect reflection of the light verctor and V is the vector to the
camera location. `P` is the **Phong Exponent**. So finally the complete light intensity
will look like this: `I = Ia + Id + Is`.

Important note about the specular component is that we need to have a positive cosine for the
angle between the light source and the normal vector otherwise there will be no specular
reflection thus we should return 0.

#### How to compute the perfect reflection vector.

Well first we need to get 2 times the angle between the light source and the normal since this will be the angle we need
for the reflection vector: `2 * (N.L)` again provided that `N.L > 0`. We then need to multiply this by `N` and subtract the
light source vector. Finally we have `2*(N.L)*N-L`. This will give the reflection vector. In GLSL we can use the function
call `reflect()` in order to obtain it.

All of these component compose the `Phong Illumination Model`.

## Phong Illumination Model

How do we incorporate RGB color. Well each light could have a specific ambient,diffure and specular
intensity. These will be constants. Each material will also have these coefficients. So:

1. `Ia = Ia*Ka`
2. `Id = Id*Kd*max(0,N.L)`
3. `Is = Is*Ks*max(0,R.L)^p`

### Multiple light sources

Well we just need to average and sum for each light source in order to obtain the proper color.

1. `Ia  = Ia*ka` (We dont want to add them but rather average them)
2. `Id = Kd*Sum(1 to n)[Idi*max(0,N.Li)]`
3. `Is = Ks*Sum(1 to n)[Isi*max(0,Ri*V)^p]`
   Finally we need to sum up all the intensities to achive the final one:
   `I = Ia*Ka + Sum(1 to n)[Idi*max(0,N*Li)] + Sum(1 to n)[Isi*max(0,Ri*V)^p]`. If the object
   surface emits light we need to also add it via a constant `Ie`.

## Blinn-Phong Illumination Model

A benefit of this model is that it is less expensive. It is the default fixed-pipeline OpenGL. Only the
sepcular component is different. Blinn noticed that the Reflection vector times the Vision Vector
has to be recalculated every time. He proposed an optimization which is the following.

We compute an `H` vector which is the avergae of `L and V`. `H = (L * V)/||L + V||`. In this case
`H` could be cached if we have the light source and the view source positioned at infinity. So `H` will
stay fixed this will benefit lower end hardware and be faster.

## Local Models

Both the `Phong` and `Blinn-Phong` models are local models. The can become semi-global when
used in raytracing, however in fixed OpenGL pipeline they are local. Within raytracing smooth
object such as sphere we can apply these models directly for each pixel, but how to we do this
on triangular meshes?

### Shading Models

1. Flat shading
   Illumination computed per face.
2. Gouraud shading
   Illumination per vertex.
3. Phong Shading
   Illumination computed per pixel.

### Flat Shading

1. Fast to compute
2. Good for showing input mesh
3. not realistic
   We only need one normal per face and all pixels to this triangle will get the same color
   based on this normal.

## Barycentric Coordinates

These are used for the other types of models since we need to interpolate inside the triangle.
We need to be able to express a point in a triangle as a convex combination of its vertices.
This is then used to interpolate at vertces over the triangle.

The basic equation is the follwing: `P = u*U + v*V + w*W, where u+v+w = 1 and 0 <= u,v,w <=1`.
This works since we could express for example the position of vertex U as just u = 1 and the
rest are 0s. This allows us to interpolate the face of the triangle and get each point in it.

### Gouraud Shading

1. Quite fast to compute.
2. Good enough for diffuse reflections.
3. Not realistic for highlights.

We need a normal per vertex. Or computed by averaging (more in lecture on meshes)
We can then eavluate for example `Blinn-Phong` model at vertices.

### Phong Shading

1. A bit slower to compute since it is for each pixel
2. Good for diffuse reflections
3. Great for specular highlights

We need one normal per pixel. The proceed to interpolate them over faces. (Must renormalize)

## Shading in Shaders

1. Flat shading
   - Need a `flat` normal per face
   - Vertex shader
2. Gouraud Shading
   - Need normal per vertex
   - Compute intensity per vertex in vertex shader
   - Intensities arrive interpolated over faces in fragment shader
3. Phong shading
   - Need normal per pixel
   - Get normal per vertex in vertex shader
   - Use interpolated normal in fragment shader

## Debgging shading using normal maps and Gooch Shading

1. Normal Map
   - replace color/intensity with normalized normal coordinates
   - `I = 0.5 * N  +  (0.5,0.5,0.5)`
   - Useful for debugging to see if normal are correct
2. Gooch Shading
   - Use `c = max(0,N*L)` to blend between two colors.
   - `I = c*Iwar + (1-c)*Icold` (mix() in GLSL)
3. Toon Shading
   - Detect silhouttes (how?)
   - Render them in black
