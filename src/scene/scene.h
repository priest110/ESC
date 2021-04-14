#pragma once

#include "../math/vec.h"
#include <vector>

namespace tracer {
struct scene {

  struct Material {
    tracer::vec3<float> ka;
    tracer::vec3<float> kd;
    tracer::vec3<float> ks;
    tracer::vec3<float> ke;
    float Ns;
    bool lightsource{false};
  };

  struct Geometry {
    std::vector<tracer::vec3<float> > vertex; // Vértices da figura geométrica
    std::vector<tracer::vec3<float> > normals;
    std::vector<tracer::vec4<float> > vertex_color;
    std::vector<tracer::vec2<float> > uv;
    std::vector<tracer::vec3<unsigned int> > face_index; // Índices dos vértices de cada triângulo que compõe a figura geométrica
    std::vector<Material> face_material;
    Material object_material;
    unsigned int geomID;
  };

  std::vector<Geometry> geometry; // Parede frontal, paredes laterais, chão, paredes, paralelepípedo, cubo e teto
  std::vector<size_t> light_sources; // Só existe uma, no teto
};



} // namespace tracer
