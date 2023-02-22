# Chapter: Introduction

### Areas

Major areas of applicability fo computer graphics include

1. Image Processing
2. Scientific Visualization
3. Modelling
4. 3D Rendering
5. User Interaction
   The reason we have this is because computer graphics is one of the earliest
   places where IO was explored.
6. Virtual Reality

### Applications

1. Video Games
2. Cartoons
3. Visual Effects
4. ANimated films
5. CAD/CAM
6. Simulation

### Graphics APIs

Several graphics APIs exist. Every graphics program need to do 2 related pices of work.
First we need to be able to draw on a windows. So the API should be able to open a window
and perform operations on it. Second, we need to be able to collect user input in order
to manipulate the graphics. Java for example encapsulated this in their packages and
usersd could write portable code on the JVM, however it might be inefficient and outdated
by now. Second approach is to use something like: `OpenGl`, `Vulkan`, or `Direct3D`. This
will tie you to a specific language, in this case C++ and will make it harder to write portable
code. For small applications it could be abstracted by some 3rd party library to encapsulate
user-interface code.

## Graphics Pipeline

Every computer has powerful 3D/2D graphics pipeline. in essense this is epcialized software and hardware that is
optimized for processing 3D triangles with shared vertices and map them into the 2D space on screen in a realistic
and order manner from back-to-front. This was once tha hardest issue but now is easily solved with `z-buffer` which
uses special memory buffer to solve this issue in a brute-force manner.

Geometric manipulation could be entirely abstracted away by 4x4 matrices. These matrices operate with 3D
cooridnates and 1 extra component for the `homogeneous` space. This is one of the biggest intellectual
hurdles to get accross once starting computer graphics.

It is very important that for most real-time applications we need to have smooth frame rate. Thus we often
represent objects in lowerr quallity in order to achieve interactability. Since object could be shown
from far and from close it is a good idea to have multiple `LODs` (Level of detail) version of the object
where they could be shown at specific levels.

## Efficiency

In modern computer CPU speed is not the problem, however memory access speed is. We need
to werite code for memoery access time not for speed of the CPU. That doesnt mean to use
slow algorithms but try to optimize cache locality in order to make the job for retrieving memory
easier.

## Desiging and Coding Graphcis Programs

When designing a graphics program it is a good idea to abstract away
different components such as:

1. Vector2
2. Vector3
3. HVector - 4 component vector
4. Matrix4
5. Matrix3
6. RBG
7. Image
8. Ray
   You see whenever we have a crucial noun we need to complement it with a class
   that suports its functionality. This will allow for easy maintance and extensibility of the code.
