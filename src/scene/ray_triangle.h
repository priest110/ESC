#pragma once
#include "../math/vec.h"
#include <limits>

namespace tracer {
/* the original jgt code */
bool intersect_triangle(vec3<float> orig, vec3<float> dir, vec3<float> vert0,
                        vec3<float> vert1, vec3<float> vert2, float &t,
                        float &u, float &v) {
  vec3<float> edge1, edge2, tvec, pvec, qvec;
  double det, inv_det;

  /* find vectors for two edges sharing vert0 */
  edge1 = vert1 - vert0;
  edge2 = vert2 - vert0;

  /* begin calculating determinant - also used to calculate U parameter */
  pvec = cross(dir, edge2);

  /* if determinant is near zero, ray lies in plane of triangle */
  det = dot(edge1, pvec);
  
  /* Até aqui verificamos se edege1 e pvec têm produto escalar = 0 */

  /* epsilon is the difference between 1.0 and the next representable value for float, ou seja, 0.0000000...1 */
  if (det > -1e-6 &&
      det < 1e-6)
    return false;

  
  inv_det = 1.0f / det;

  /* calculate distance from vert0 to ray origin */
  tvec = orig - vert0;

  /* calculate U parameter and test bounds */
  float u2 = dot(tvec, pvec) * inv_det;
  if (u2 < 1e-6 || u2 > 1.0f)
    return false;

  /* prepare to test V parameter */
  qvec = cross(tvec, edge1);
 
  /* calculate V parameter and test bounds */
  float v2 = dot(dir, qvec) * inv_det;
  if (v2 < 1e-6 || u2 + v2 > 1.0f)
    return false;

  /* calculate t, ray intersects triangle */
  float t2 = dot(edge2, qvec) * inv_det;
  if (t2 < 1e-6)
    return false;

  if (t2 >= t)
    return false;

  t = t2;
  u = u2;
  v = v2;
  return true;
}

} // namespace tracer

