/*
struct vec3 
{
    uniform float v[4];
};

export struct triangle 
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

uniform vec3 cross(uniform vec3& v1, uniform vec3& v2) {
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

uniform vec3 operator-(uniform vec3& v0, uniform vec3& v1) {
  uniform vec3 v;
  for(uniform int i = 0; i <3; i++)
    v.v[i] = v0.v[i] - v1.v[i];
  return v;
}


uniform bool intersect_triangle(uniform vec3& orig, uniform vec3& dir, varying vec3& vert0, varying vec3& vert1, varying vec3& vert2, uniform float t, uniform float u, uniform float v) {
  uniform vec3 edge1, edge2, tvec, pvec, qvec;
  uniform double det, inv_det;

  edge1 = vert1 - vert0;
  edge2 = vert2 - vert0;
  
  pvec = cross(dir, edge2);
  
  det = dot(edge1, pvec);
 
  if (det > -1e-6 &&
      det < 1e-6)
    return false;

  inv_det = 1.f / det;

  tvec = orig - vert0;

  uniform float u2 = dot(tvec, pvec) * inv_det;
  if (u2 < 1e-6 || u2 > 1.0f)
    return false;

  qvec = cross(tvec, edge1);
 
  uniform float v2 = dot(dir, qvec) * inv_det;
  if (v2 < 1e-6 || u2 + v2 > 1.0f)
    return false;

  uniform float t2 = dot(edge2, qvec) * inv_det;
  if (t2 < 1e-6)
    return false;

  if (t2 >= t)
    return false;

  t = t2;
  u = u2;
  v = v2;
  return true;
  
} 
export void nada(varying triangle* uniform tris){
  return;
}
*/
/* Calcula a interseção */
/*
export uniform bool intersect(uniform int tam, varying triangle* uniform tris, uniform vec3 &ori,
               uniform vec3 &dir, uniform float &t, uniform float &u, uniform float &v,
               uniform size_t &geomID, uniform size_t &primID) {
  for(uniform int i = 0; i < tam; i++){
    foreach(j = 0 ... 8){
      if(intersect_triangle(ori, dir, tris[i].vertices[0], tris[i].vertices[1], tris[i].vertices[2], t, u, v)){
        geomID = tris[i].geomID;
        geomID = tris[i].primID;
    }
    }
  }
  return (geomID != -1 && primID != -1);
}
*/

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
