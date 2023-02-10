# Transformations
Matrices allow to create transformation of an object. Both in local
and global space. Matrices allows us to move between spaces like
loca,global,clip,etc.

## Vectors
A vector has a direction and a magnitude. Addition/Subtraction is pretty easy
also the scalar operation. Finding a vector from another two:
1. If e have a vector w and v the vector from v to w is w - v. So basically end goal - start goal is start to end.

### Useful vector operation
1. Length - this is just squaring each component summing it up and then taking the square root of it all
2. dot product - the dot product is just multiplying each component by each and then summing it up.
Interesting property of the dot product is that it is equal to the multiplication
of the vectors lengths times the cosine of the angle between them. Thus we can go
very easily back and forth between vectors and angles.
3. Cross Product
Cross product works only in 3D space it takes two non parallel vectors and
produces an orthogonal vector of them. This is very useful for example
if you want to find the **normal** of a vector or even the right direction
of a vector. Imagine you have a player and you want to find the right vector.
Well just take the forward and the up vector make a cross product and you
have the right vector.
Formula: [ax,ay,az] X [bx,by,bz] = [ay.bz - az.by,az.bx - ax.bz,ax.by - ay.bx]

## Matrices
Addition and subtraction is done on per element basis meaning we need matrices
of the same dimension to do these operations on. THe operations are communicative
and associative. The scalar product is just multiplying each element of the matrix
by the scalar value.

### Matrix Multiplication
You can perform this only if some rules are met first:
1. The number of columns of the left matrix needs to be the same as the rows in the right one
2. Matrix multiplication is not communicative meaning A.B != B.A
When we mutiply matrices they will be in the form: n x m times m x p the result
will have the shape of (n,p). We can even multiply a vector with a matrix
we have to be careful is we choose row vector of column vector.

#### Scaling
In order to scale a vector we can use a scale matrix. This is a 4x4 matrix
that has homogeneous space the vector is also 4 component for the homogeneous
component. Example:
```C++
    /* [S1, 0, 0, 0] [x]
     * [0, S2, 0, 0] [y]
     * [0, 0, S3, 0] [z]
     * [0, 0, 0, 1 ] [1]
     *
     */
```
We can easily see why this will work. We multiply 4x4 by 4x1 which will result
in a 4x1 vector. The first component will be equal to. S1.x + 0.0 ... so
we bassically scale X by a factor of S1 same for the other. We maintain the
homogenous component at 1 so it doesnt change.

#### Translation
```C++
    /* [1,0,0,T1] [x]
     * [0,1,0,T2] [y]
     * [0,0,1,T3] [z]
     * [0,0,0,1]  [1]
     */
```
We again see 4x4 times 4x1 resulting in a 4x1 vector. We see now why we need
the homogenoues component at 1: The resulting vectors first component will
be equal to: `x*1 + 0*0 + 0*0 + T1*1` which is: `x + T1`. So after doing
that for each component we have successfully translated it into world space.

### Homogeneous component
This component allows us to translate 3D vectors without it it would not be possible.
To get the 3D vector from the 4D vector we just divide each component by the
homogenous component (which is usually one). If it is 0 then this vector
is called direction vector and cannot be trasnlated into space (see the math above
to check why)

#### Rotation
A rotation can be represented with an angle either in degress of radians where
`angle in degrees = angle in radians * (180/PI)` and 
`angle in radians = angle in degrees * (PI/180)`.
Rotation in 2D are easy we just need an angle between two vectors that our rotation.
However in 3D we need an angle and a rotation axis since we can rotation about
any of the 3 axis. I will now show the 3 Rotation matrices over the 3 axis
since for each axis we have a different rotation matrix. I would recommned the
Khan Academy videos if you like to know how to generate them and not memorize
them.
```C++
    // X axis
    /* [1,0,0,0]
     * [0,cosPhy,-sinPhy,0]
     * [0,sinPhy,cosPhy,0]
     * [0,0,0,1]
     */
    
    // Y axis
     /* [cosPhy,0,sinPhy,0]
      * [0,1,0,0]
      * [-sinPhy,0,cosPhy,0]
      * [0,0,0,1]
      */
    // Z axis
      /* [cosPhy,-sinPhy,0,0]
       * [sinPhy,cosPhy,0,0]
       * [0,0,1,0]
       * [0,0,0,1]
       */
```
In order to rotate around an arbitrary axis we just combine all 3 of them.
First the X then the Y then the Z. Howver, this quickly causes a problem known
as Gimbal lock. To remove this problem we can use Quaternions. They are fairly
complex math object but we only need to concern ourselves with a few more basic
equations. They can be seen in the book: Game Programming in C++ I beleive chapter 5.

### Combining Matrices
Since we can multiply matrices in order to scale,translate and rotate an object
we just need to multiply the corresponding matrices, however this needs to be
done in a very specific order otherwise we will end up with a wrong representation
of the vector. The order we do it in is: T.R.S where T is translate,R is rotate,S
is scale. 

## Good playlist for linear algebra
https://www.youtube.com/playlist?list=PLZHQObOWTQDPD3MizzM2xVFitgF8hE_ab
