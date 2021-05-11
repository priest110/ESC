#pragma once

#include "../math/vec.h"

namespace tracer {

struct ray {
  vec3<float> origin; // origem do raio
  vec3<float> dir; // direção do raio
  vec3<float> inv_dir; // inverso da direção do raio
  int sinal[3];

  ray(vec3<float> origin, vec3<float> dir) : origin(origin), dir(dir) {
    inv_dir = 1.f / dir;
    sinal[0] = (inv_dir.x < 0);
    sinal[1] = (inv_dir.y < 0);
    sinal[2] = (inv_dir.z < 0);      
  }
};

struct camera {
public:
  camera(vec3<float> lookfrom, vec3<float> lookat, vec3<float> vup, float vfov,
         float aspect) {
    // vfov is top to bottom in degrees
    float theta = vfov * M_PI / 180; // Ângulo em rad do que a câamra capta(de cima a baixo)
    float half_height = tan(theta / 2); // Metade da altura do campo de visão da câmara  
    float half_width = aspect * half_height; // Metade do comprimento do campo de visão da câmara
    origin = lookfrom;
    w = normalize(lookfrom - lookat);
    u = normalize(cross(vup, w));
    v = cross(w, u);
    lower_left_corner = origin - u * half_width - v * half_height - w;
    horizontal = u * (2.0f * half_width);
    vertical = v * (2.0f * half_height);
  }

  ray get_ray(float s, float t) {
    return ray(origin, normalize(lower_left_corner + horizontal * s +
                                 vertical * t - origin));
  }

  vec3<float> origin; // Origem da câmara(onde está o olho)
  vec3<float> lower_left_corner; // Canto esquerda das dimensões do campo de visão da câmara
  vec3<float> horizontal; // Altura do campo de visão da câmara
  vec3<float> vertical; // Largura do campo de visão da câmara
  vec3<float> u, v, w; // Vetores unitários de x, y, z
};

} // namespace tracer