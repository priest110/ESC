#include <chrono>
#include <cstring>
#include <fstream>
#include <future>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <thread>
#include <deque>
#include <type_traits>

#include "math/vec.h"
#include "scene/camera.h"
#include "scene/ray_triangle.h"
#include "scene/scene.h"
#include "scene/bvh.h"
#include "scene/sceneloader.h"

/* Calcula a interseção */
bool intersect(const tracer::scene &SceneMesh, const tracer::vec3<float> &ori,
               const tracer::vec3<float> &dir, float &t, float &u, float &v,
               size_t &geomID, size_t &primID) {
  for (auto i = 0; i < SceneMesh.geometry.size(); i++) {
    for (auto f = 0; f < SceneMesh.geometry[i].face_index.size(); f++) {
      auto face = SceneMesh.geometry[i].face_index[f];
      if (tracer::intersect_triangle(
              ori, dir, SceneMesh.geometry[i].vertex[face[0]],
              SceneMesh.geometry[i].vertex[face[1]],
              SceneMesh.geometry[i].vertex[face[2]], t, u, v)) {
        geomID = i; // figura geométrica intercetada
        primID = f; // face(triângulo) intercetada
      }
    }
  }
  return (geomID != -1 && primID != -1);
}


/* Verifica se há oclusão(algum obstáculo a tapar a luz) e cria sombra */
bool occlusion(const tracer::scene &SceneMesh, const tracer::vec3<float> &ori,
               const tracer::vec3<float> &dir, float &t) {
  float u, v;
  for (auto i = 0; i < SceneMesh.geometry.size(); i++) {
    for (auto f = 0; f < SceneMesh.geometry[i].face_index.size(); f++) {
      auto face = SceneMesh.geometry[i].face_index[f];
      if (tracer::intersect_triangle(
              ori, dir, SceneMesh.geometry[i].vertex[face[0]],
              SceneMesh.geometry[i].vertex[face[1]],
              SceneMesh.geometry[i].vertex[face[2]], t, u, v)) {
        return true;
      }
    }
  }
  return false;
}

struct block{
  int l;
  int c_start;
  int c_end;

  block(int l, int c_start, int c_end) : l(l), c_start(c_start), c_end(c_end){};
};

std::mutex mtx;

int main(int argc, char *argv[]) {
  std::string modelname = "";
  std::string outputname = "output.ppm";
  bool hasEye{false}, hasLook{false};
  tracer::vec3<float> eye(0, 1, 3), look(0, 1, 0);
  tracer::vec2<unsigned> windowSize(1024, 768);
  
  for (int arg = 0; arg < argc; arg++) {
    if (std::string(argv[arg]) == "-m") {
      modelname = std::string(argv[arg + 1]);
      arg++;
      continue;
    }
    if (std::string(argv[arg]) == "-o") {
      outputname = std::string(argv[arg + 1]);
      arg++;
      continue;
    }
    if (std::string(argv[arg]) == "-v") {
      char *token = std::strtok(argv[arg + 1], ",");
      int i = 0;
      while (token != NULL) {
        eye[i++] = atof(token);
        token = std::strtok(NULL, ",");
      }

      if (i != 3)
        throw std::runtime_error("Error parsing view");
      hasEye = true;
      arg++;
      continue;
    }
    if (std::string(argv[arg]) == "-l") {
      char *token = std::strtok(argv[arg + 1], ",");
      int i = 0;

      while (token != NULL) {
        look[i++] = atof(token);
        token = std::strtok(NULL, ",");
      }

      if (i != 3)
        throw std::runtime_error("Error parsing view");
      hasLook = true;
      arg++;
      continue;
    }
    if (std::string(argv[arg]) == "-w") {
      char *token = std::strtok(argv[arg + 1], ",");
      int i = 0;

      while (token != NULL) {
        //windowSize[i++] = atoi(token); 
        token = std::strtok(NULL, ",");
      }

      if (i != 2)
        throw std::runtime_error("Error parsing window size");
      arg++;
      continue;
    }
  }

  tracer::scene SceneMesh;
  bool ModelLoaded = false;

  if (modelname != "") {
    SceneMesh = model::loadobj(modelname);
    ModelLoaded = true;
  }
 
  int image_width = windowSize.x; // 1024
  int image_height = windowSize.y; // 768

  tracer::camera cam(eye, look, tracer::vec3<float>(0, 1, 0), 60,
                     float(image_width) / image_height);
  // Render

  tracer::vec3<float> *image =
      new tracer::vec3<float>[image_height * image_width];

  auto start_time = std::chrono::high_resolution_clock::now();

  std::vector<tracer::triangle*> triangles;
  for(auto i = 0; i < SceneMesh.geometry.size(); i++){
    for(auto j = 0; j < SceneMesh.geometry[i].face_index.size(); j++){
      auto face = SceneMesh.geometry[i].face_index[j];
      auto v0 = SceneMesh.geometry[i].vertex[face[0]];
      auto v1 = SceneMesh.geometry[i].vertex[face[1]];
      auto v2 = SceneMesh.geometry[i].vertex[face[2]];

      tracer::triangle *triang = new tracer::triangle();
      (*triang).geomID = i;
      (*triang).primID = j;
      (*triang).center = tracer::vec3<float>((v0.x+v1.x+v2.x)/3,(v0.y+v1.y+v2.y)/3,(v0.z+v1.z+v2.z)/3);
      (*triang).axis = 0;
      (*triang).vertices.push_back(tracer::vec3<float>(v0.x, v0.y, v0.z));
      (*triang).vertices.push_back(tracer::vec3<float>(v1.x, v1.y, v1.z));
      (*triang).vertices.push_back(tracer::vec3<float>(v2.x, v2.y, v2.z));

      triangles.push_back(triang);
    }
  } 

  tracer::Tree *bvh_tree = tracer::createTree(createBBox(triangles));
  tracer::create_bvh(bvh_tree, triangles, 0);

  std::random_device rd;
  std::mt19937 gen(rd()); 
  std::uniform_real_distribution<float> distrib(0, 1.f); 
  
  auto num_pixeis = image_height * image_width;
  auto num_blocks = num_pixeis/64;
  int line, col;

  std::deque<block> locked_queue; 
  for (line= image_height - 1; line >= 0; --line) {
    for (col = 0; col < image_width ; col+=64) {
      block b = block(line, col, col+63);
      locked_queue.push_front(b);
    }
  }   
  std::vector<std::thread> threads;
  const auto NUM_THREADS = std::thread::hardware_concurrency()/2;
  threads.reserve(NUM_THREADS);
  
  for(int i = 0; i < NUM_THREADS; i++){  
    threads.push_back(std::thread(
      [&](const int i, const tracer::scene SceneMesh) -> void{
        while(locked_queue.size() != 0){
          mtx.lock();
          if(locked_queue.size() == 0){
            mtx.unlock();
            break;
          }
          block b = locked_queue.front();
          locked_queue.pop_front();
          mtx.unlock();
          for(int bl = b.c_start; bl <= b.c_end; bl++){
            size_t geomID = -1;
            size_t primID = -1;
            int w = bl;
            int h = b.l;
            auto is = float(w) / (image_width - 1);
            auto it = float(h) / (image_height - 1); 
            auto ray = cam.get_ray(is, it);

            float t = std::numeric_limits<float>::max(); // largest possible value for type float
            float u = 0;
            float v = 0;
            if (tracer::bvh_intersect(bvh_tree, ray, t, u, v, geomID, primID)) {
            //if (intersect(SceneMesh, ray.origin, ray.dir, t, u, v, geomID, primID)) {
                auto i = geomID;
                auto f = primID;
                auto face = SceneMesh.geometry[i].face_index[f];
                auto N = normalize(cross(SceneMesh.geometry[i].vertex[face[1]] -
                                            SceneMesh.geometry[i].vertex[face[0]],
                                        SceneMesh.geometry[i].vertex[face[2]] -
                                            SceneMesh.geometry[i].vertex[face[0]]));

                if (!SceneMesh.geometry[i].normals.empty()) {
                    auto N0 = SceneMesh.geometry[i].normals[face[0]];
                    auto N1 = SceneMesh.geometry[i].normals[face[1]];
                    auto N2 = SceneMesh.geometry[i].normals[face[2]];
                    N = normalize(N1 * u + N2 * v + N0 * (1 - u - v));
                }
                tracer::vec3<float> hit = 0;
                for (auto &lightID : SceneMesh.light_sources) {
                    auto light = SceneMesh.geometry[lightID];
                    light.face_index.size();
                    std::uniform_int_distribution<int> distrib1(
                        0, light.face_index.size() - 1);

                    int faceID = distrib1(gen);
                    const auto &v0 = light.vertex[faceID]; 
                    const auto &v1 = light.vertex[faceID];
                    const auto &v2 = light.vertex[faceID];

                    auto P = v0 + ((v1 - v0) * float(distrib(gen)) +
                                    (v2 - v0) * float(distrib(gen))); // de onde parte a luz
                    
                    hit = ray.origin +
                                ray.dir * (t - (float)(1e-6)); // onde a luz acerta
                    
                    auto L = P - hit; // vetor hit->P
                    
                    auto len = tracer::length(L); // tamanho do vetor

                    t = len - 1e-6; // len = t

                    L = tracer::normalize(L); // vetor hit -> P normalizado

                    auto mat = SceneMesh.geometry[i].object_material;
                    auto c =
                        (mat.ka * 0.5f + mat.ke) / float(SceneMesh.light_sources.size());
                    
                    /* Se há oclusão, então há sombra, sai do ciclo */
                    tracer::ray aux = tracer::ray(hit,L);
                    if(tracer::bvh_occlusion(bvh_tree, aux, t))
                    //if (occlusion(SceneMesh, hit, L, t))
                        continue;
                        
                    auto d = dot(N, L);

                    /* Se o produto escalar entre N e L <= 0, então estes situam-se no plano da lâmpada ou N situa-se acima desse plano, e sai do ciclo*/
                    if (d <= 0)
                        continue;
                    
                    auto H = normalize((N + L) * 2.f);

                    c = c + (mat.kd * d + mat.ks * pow(dot(N, H), mat.Ns)) /
                                float(SceneMesh.light_sources.size()); // trata da cor do pixel

                    image[h * image_width + w].r += c.r;
                    image[h * image_width + w].g += c.g;
                    image[h * image_width + w].b += c.b;  
                }
            }
          }
        }
      }, i,SceneMesh));
  }

  for(auto &thr :threads)
    thr.join();

  auto end_time = std::chrono::high_resolution_clock::now();

  std::cerr << std::chrono::duration_cast<std::chrono::milliseconds>(end_time -
                                                                     start_time)
                   .count()
            << ",";

  std::ofstream file(outputname, std::ios::out);

  file << "P3\n" << image_width << " " << image_height << "\n255\n";
  for (int h = image_height - 1; h >= 0; --h) {
    for (int w = 0; w < image_width; ++w) {
      auto &img = image[h * image_width + w];
      img.r = (img.r > 1.f) ? 1.f : img.r;
      img.g = (img.g > 1.f) ? 1.f : img.g;
      img.b = (img.b > 1.f) ? 1.f : img.b;

      file << int(img.r * 255) << " " << int(img.g * 255) << " "
           << int(img.b * 255) << "\n";
    }
  }
  /* Delete dos apontadores(faz sentido dar também dos NULL) */
  for(tracer::triangle* tri : triangles)
    delete tri;
  delete bvh_tree;  
  
  delete[] image;
  return 0;
}
