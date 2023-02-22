# Chapter 2: Miscellaneous Math

Most of graphics programming is just translating
math to code. The cleaner the math the cleaner the code.
Here will be an overview of some basic concepts and things
that should be known.

## Sets and Mappings

1. Cartesian Product of tweo sets
   This product return a set will all order pairs from the two
   sets for example is we have a set A with {1,2} and a Set B with {3,4}.
   The Cartesian product will be `C{(1,3),(1,4),(2,3),(2,4)}`.
2. Inverse function
   The inverse of a function is basically representing it with the y and x swapped.
   Usually we determine y from x. However, in the swap variant we determine x from y.

3. Quadratic equations
   The basic things we need to know are that the `Descriminant` is equal to `B^2 - 4 * C * A`
   and that if is it `0` we have only 1 solutions, if `>0` we have 2, and if `<0` we have 0.
   The solutions are obtained via: `(-B += sqrt(D))/2*A`, where `D` is the descriminant.
4. Angle representation
   We have 2 ways to determine angles. One is in radians which is basically in terms of `pi`, the other
   is in temrs of degress where they are between `0 and 360`. The conversion is very simple. From radians
   to degress `d = 180/pi * r`,and from degress to radians `r = pi/180 * d`.
