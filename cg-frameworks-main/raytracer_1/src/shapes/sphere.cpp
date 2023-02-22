#include "sphere.h"

#include <cmath>

using namespace std;

Hit Sphere::intersect(Ray const &ray)
{
  /****************************************************
   * RT1.1: INTERSECTION CALCULATION
   *
   * Given: ray, position, r
   * Sought: intersects? if true: *t
   *
   * Insert calculation of ray/sphere intersection here.
   *
   * You have the sphere's center (position) and radius (r) as well as
   * the ray's origin (ray.O) and direction (ray.D).
   *
   * If the ray does not intersect the sphere, return false.
   * Otherwise, return true and place the distance of the
   * intersection point from the ray origin in *t (see example).
   ****************************************************/
  Vector U = ray.O - position;
  double a = ray.D.dot(ray.D);
  double b = 2 * ray.D.dot(U);
  double c = U.dot(U) - r * r;
  double des = b * b - 4 * a * c;

  if (des < 0)
    return Hit::NO_HIT();

  double t1, t2;
  t1 = (-b + sqrt(des)) / 2 * a;
  t2 = (-b - sqrt(des)) / 2 * a;

  if (t1 < 0 and t2 < 0)
    return Hit::NO_HIT();

  double res = 0;
  if (t1 < 0)
    res = t2;
  else if (t2 < 0)
    res = t1;
  else
    res = t1 < t2 ? t1 : t2;

  /****************************************************
   * RT1.2: NORMAL CALCULATION
   *
   * Given: t, position, r
   * Sought: N
   *
   * Insert calculation of the sphere's normal at the intersection point.
   ****************************************************/

  Point hitPoint = ray.at(res);
  Vector N = (hitPoint - position).normalized();

  return Hit(res, N);
}

Sphere::Sphere(Point const &pos, double radius) : position(pos), r(radius) {}
