
struct uniform_vec3 
{
   uniform float v[3];
};

struct varying_vec3 
{
   varying float v[3];
};

struct triangle 
{
    uniform varying_vec3 vertices[3];
    varying int primID;
    varying int geomID;
};


struct ray
{
    uniform_vec3 ori;
    uniform_vec3 dir;
    varying float t;
    varying float u;
    varying float v;
    varying int geomID;
    varying int primID;
    varying bool valido;
};


uniform varying_vec3 vv_cross(uniform varying_vec3& v1, uniform varying_vec3& v2) {
  uniform varying_vec3 dest;
  dest.v[0] = v1.v[1] * v2.v[2] - v1.v[2] * v2.v[1];
  dest.v[1] = v1.v[2] * v2.v[0] - v1.v[0] * v2.v[2];
  dest.v[2] = v1.v[0] * v2.v[1] - v1.v[1] * v2.v[0];
  return dest;
}

uniform varying_vec3 vu_cross(uniform varying_vec3& v1, uniform uniform_vec3& v2) {
  uniform varying_vec3 dest;
  dest.v[0] = v1.v[1] * v2.v[2] - v1.v[2] * v2.v[1];
  dest.v[1] = v1.v[2] * v2.v[0] - v1.v[0] * v2.v[2];
  dest.v[2] = v1.v[0] * v2.v[1] - v1.v[1] * v2.v[0];
  return dest;
}

uniform varying_vec3 uv_cross(uniform uniform_vec3& v1, uniform varying_vec3& v2) {
  uniform varying_vec3 dest;
  dest.v[0] = v1.v[1] * v2.v[2] - v1.v[2] * v2.v[1];
  dest.v[1] = v1.v[2] * v2.v[0] - v1.v[0] * v2.v[2];
  dest.v[2] = v1.v[0] * v2.v[1] - v1.v[1] * v2.v[0];
  return dest;
}

varying float vv_dot(uniform varying_vec3& v0, uniform varying_vec3& v1) {
  varying float sum = 0;
  for(uniform int i = 0; i <3; i++)
    sum += v0.v[i] * v1.v[i];
  return sum;
}

varying float vu_dot(uniform varying_vec3& v0, uniform uniform_vec3& v1) {
  varying float sum = 0;
  for(uniform int i = 0; i <3; i++)
    sum += v0.v[i] * v1.v[i];
  return sum;
}

varying float uv_dot(uniform uniform_vec3& v0, uniform varying_vec3& v1) {
  varying float sum = 0;
  for(uniform int i = 0; i <3; i++)
    sum += v0.v[i] * v1.v[i];
  return sum;
}

uniform varying_vec3 operator-(uniform varying_vec3& v0, uniform varying_vec3& v1) {
  uniform varying_vec3 v;
  for(uniform int i = 0; i <3; i++)
    v.v[i] = v0.v[i] - v1.v[i];
  return v;
}

uniform uniform_vec3 operator-(uniform uniform_vec3& v0, uniform uniform_vec3& v1) {
  uniform uniform_vec3 v;
  for(uniform int i = 0; i <3; i++)
    v.v[i] = v0.v[i] - v1.v[i];
  return v;
}

uniform varying_vec3 operator-(uniform uniform_vec3& v0, uniform varying_vec3& v1) {
  uniform varying_vec3 v;
  for(uniform int i = 0; i <3; i++)
    v.v[i] = v0.v[i] - v1.v[i];
  return v;
}

uniform varying_vec3 operator-(uniform varying_vec3& v0, uniform uniform_vec3& v1) {
  uniform varying_vec3 v;
  for(uniform int i = 0; i <3; i++)
    v.v[i] = v0.v[i] - v1.v[i];
  return v;
}

uniform bool intersect_triangle(uniform varying_vec3& vert0, uniform varying_vec3& vert1, uniform varying_vec3& vert2, uniform ray& r) {
  uniform varying_vec3 edge1, edge2, tvec, pvec, qvec;
  varying double det, inv_det;
  bool valido = true;

  edge1 = vert1 - vert0;
  edge2 = vert2 - vert0;
  
  pvec = uv_cross(r.dir, edge2);

  det = vv_dot(edge1, pvec);
  r.valido = !(det > -1e-6 && det < 1e-6);
  if(all(!r.valido))
    return false;

  inv_det = 1.f / det;

  tvec = r.ori - vert0;

  float u2 = vv_dot(tvec, pvec) * inv_det;

  r.valido = r.valido && !(u2 < 1e-6 || u2 > 1.0f);
  if(all(!r.valido))
    return false;

  qvec = vv_cross(tvec, edge1);
  float v2 = uv_dot(r.dir, qvec) * inv_det;
  r.valido = r.valido && !(v2 < 1e-6 || u2 + v2 > 1.0f);
  if (all(!r.valido))
    return false;

  float t2 = vv_dot(edge2, qvec) * inv_det;
  r.valido = r.valido && (!(t2 < 1e-6) || !(t2 >= r.t));
  if (all(!r.valido))
    return false;
  r.t = t2;
  r.u = u2;
  r.v = v2;
  return true;
} 

/* Calcula a interseção */
export uniform bool ispc_intersect(uniform triangle& tri, uniform ray& r) {
    r.geomID = -1; r.primID = -1;
    r.valido = false;
    r.v = 0; r.u = 0;
    if(intersect_triangle(tri.vertices[0], tri.vertices[1], tri.vertices[2], r)){
      r.geomID = tri.geomID;
      r.primID = tri.primID;
    }
    return any(r.geomID != -1 && r.primID != -1);
}

/* Verifica se há oclusão(algum obstáculo a tapar a luz) e cria sombra */
export uniform bool ispc_occlusion(uniform triangle& tri, uniform ray& r, uniform int h, uniform int w) {
  r.valido = false;
  return intersect_triangle(tri.vertices[0], tri.vertices[1], tri.vertices[2], r);
}
