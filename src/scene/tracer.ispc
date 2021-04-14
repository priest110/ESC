struct vec3 
{
    uniform float v[4];
};

struct triangle 
{
    vec3 vertices[3];
    int primID;
    int geomID;
};


struct rayhit
{
    vec3 ori;
    vec3 dir;
    float t;
    float u;
    float v;
    int geomID;
    int primID;
};

uniform vec3 cross(uniform vec3 &v1, uniform vec3 &v2) {
  uniform vec3 dest;
  dest.v[0] = v1.v[1] * v2.v[2] - v1.v[2] * v2.v[1];
  dest.v[1] = v1.v[2] * v2.v[0] - v1.v[0] * v2.v[2];
  dest.v[2] = v1.v[0] * v2.v[1] - v1.v[1] * v2.v[0];
  return dest;
}

uniform float dot(uniform vec3& v0, uniform vec3& v1) {
  uniform float sum = 0;
  for(uniform int i = 0; i <3; i++)
    sum += v0.v[i] * v1.v[i];
  return sum;
}

uniform vec3 operator-(uniform vec3 &v0, uniform vec3 &v1) {
  uniform vec3 v;
  for(uniform int i = 0; i <3; i++)
    v.v[i] = v0.v[i] - v1.v[i];
  return v;
}




bool intersect_triangle(uniform vec3 orig, uniform vec3 dir, uniform vec3 vert0, uniform vec3 vert1, uniform vec3 vert2, float t, float u, float v) {
  uniform vec3 edge1, edge2, tvec, pvec, qvec;
  uniform double det, inv_det;

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

  
  inv_det = 1.f / det;

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

/* Calcula a interseção */



/* Verifica se há oclusão(algum obstáculo a tapar a luz) e cria sombra 
extern "C" bool occlusion(uniform struct scene* SceneMesh, uniform struct vec3 ori,
               uniform struct vec3 dir, float t) {
  float u, v;
  for (auto i = 0; i < SceneMesh.geometry.size(); i++) {
    for (auto f = 0; f < SceneMesh.geometry[i].face_index.size(); f++) {
      auto face = SceneMesh.geometry[i].face_index[f];
      if (intersect_triangle(
              ori, dir, SceneMesh.geometry[i].vertex[face[0]],
              SceneMesh.geometry[i].vertex[face[1]],
              SceneMesh.geometry[i].vertex[face[2]], t, u, v)) {
        return true;
      }
    }
  }
  return false;
}
*/

int main(){
    uniform triangle tri;
    tri.vertices[0].v[0] = 0.0;
    tri.vertices[0].v[1] = 0.0;
    tri.vertices[0].v[2] = 0.0;
    tri.vertices[1].v[0] = 1.0;
    tri.vertices[1].v[1] = 1.0;
    tri.vertices[1].v[2] = 1.0;
    tri.vertices[2].v[0] = 0.5;
    tri.vertices[2].v[1] = 0.5;
    tri.vertices[2].v[2] = 0.5;
    for(uniform int i = 0; i < 3; i++){
        print("\nV: % % %", tri.vertices[i].v[0], tri.vertices[i].v[1], tri.vertices[i].v[2]);
    }
    return 0;
}