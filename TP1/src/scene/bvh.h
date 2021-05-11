#include <algorithm>
#include <functional>
#include <array>
#include <iostream>
#include <cstdint>
#include <mutex>
#include <thread>
#include <vector>
#include <deque>
#include "../math/vec.h"

namespace tracer {
    struct triangle{
        unsigned int geomID;
        unsigned int primID;
        int axis = 0;
        tracer::vec3<float> center;
        std::vector<tracer::vec3<float>> vertices;

        bool operator < (const triangle& rhs) const{
            return (center[rhs.axis] < rhs.center[rhs.axis]);
        }

       };

    struct BBox{
        tracer::vec3<float> vertices[2];
        std::vector<tracer::triangle*> triangles;
    };

    BBox* createBBox(std::vector<tracer::triangle*> triangles){
            BBox* box = new BBox();
            float min_x, min_y, min_z, max_x, max_y, max_z;
                    
            min_x = std::numeric_limits<float>::max();
            min_y = std::numeric_limits<float>::max();
            min_z = std::numeric_limits<float>::max();
            max_x = std::numeric_limits<float>::lowest();
            max_y = std::numeric_limits<float>::lowest();
            max_z = std::numeric_limits<float>::lowest();

            for(triangle *tri : triangles){
                for(tracer::vec3<float> v : (*tri).vertices){
                    if(v[0] > max_x)
                        max_x = v[0];
                    if(v[0] < min_x)
                        min_x = v[0];
                    if(v[1] > max_y)
                        max_y = v[1];
                    if(v[1] < min_y)
                        min_y = v[1];
                    if(v[2] > max_z)
                        max_z = v[2];
                    if(v[2] < min_z)
                        min_z = v[2];
                }
            }
            (*box).triangles = triangles;
            (*box).vertices[0] = tracer::vec3<float>(min_x, min_y, min_z);
            (*box).vertices[1] = tracer::vec3<float>(max_x, max_y, max_z);
            
            return box;
        } 

    struct Tree{
        Tree *left, *right;
        tracer::BBox *leaf;
    };    

    Tree* createTree(tracer::BBox *box){
        Tree* newTree = new Tree();
        if(!newTree){
            printf("Memory error\n");
            return NULL;
        }
        newTree -> leaf = box;
        newTree -> left = newTree -> right = NULL;
        return newTree;
    }

    struct create_bvh_argument{
        Tree *_node;
        std::vector<tracer::triangle*> _triangles;
        int _axis;

        create_bvh_argument(Tree *node, std::vector<tracer::triangle*> triangles, int axis)
            : _node(node), _triangles(triangles), _axis(axis) {};
    };

    void create_bvh(Tree *node, std::vector<tracer::triangle*> triangles, int axis){
        if(triangles.size() == 1){
            node -> leaf = createBBox(triangles);
            node -> left = NULL;
            node -> left = NULL;
        }
        else if(triangles.size() == 2){
            std::size_t const half_size = triangles.size() / 2;
            std::vector<tracer::triangle*> tri_left(triangles.begin(), triangles.begin() + half_size);
            std::vector<tracer::triangle*> tri_right(triangles.begin() + half_size, triangles.end());
            BBox* left = createBBox(tri_left);
            BBox* right= createBBox(tri_right);
            node -> left = createTree(left);
            node -> right= createTree(right);
        }
        else{
            std::sort(triangles.begin(), triangles.end());

            //double mid = (triangles[0]->center[axis] + triangles[triangles.size()-1]->center[axis])/2;
            //std::vector<tracer::triangle*> tri_left, tri_right;

            for(tracer::triangle* tri : triangles){
                (*tri).axis = axis;
               /* if(mid > (*tri).center[axis])
                    tri_left.push_back(tri);
                else    
                    tri_right.push_back(tri); */
            }
            std::size_t const half_size = triangles.size() / 2;
            std::vector<tracer::triangle*> tri_left(triangles.begin(), triangles.begin() + half_size);
            std::vector<tracer::triangle*> tri_right(triangles.begin() + half_size, triangles.end());
            axis = (axis+1)%3;

            BBox* left = createBBox(tri_left);
            node -> left = createTree(left);
            BBox* right= createBBox(tri_right);
            node -> right= createTree(right);
            create_bvh(node -> left, tri_left, axis);
            create_bvh(node -> right, tri_right, axis);
        }
    }

    /* Calcula a interseção com bbox(versão normal) */
    bool box_intersect(const tracer::ray ray, BBox* box){
        float tmin, tmax, tmin_y, tmax_y, tmin_z, tmax_z;
         // x
        tmin = ((*box).vertices[0].x - ray.origin.x)/ray.dir.x;
        tmax = ((*box).vertices[1].x - ray.origin.x)/ray.dir.x;
        
        if(tmin > tmax) std::swap(tmin, tmax);
        
        // y
        tmin_y = ((*box).vertices[0].y - ray.origin.y)/ray.dir.y;
        tmax_y = ((*box).vertices[1].y - ray.origin.y)/ray.dir.y;
        
        if(tmin_y > tmax_y) std::swap(tmin_y, tmax_y);

        if(tmin > tmax_y || tmin_y > tmax)
            return false;

        if(tmin_y > tmin) tmin = tmin_y;
        if(tmax_y < tmax) tmax = tmax_y;

        // z
        tmin_z = ((*box).vertices[0].z - ray.origin.z)/ray.dir.z;
        tmax_z = ((*box).vertices[1].z - ray.origin.z)/ray.dir.z;

        if(tmin_z > tmax_z) std::swap(tmin_z, tmax_z);

        if(tmin > tmax_z || tmin_z > tmax)
            return false;

        if(tmin_z > tmin) tmin = tmin_z;
        if(tmax_z < tmax) tmax = tmax_z; 

        return true;
    }

    /* Calcula a interseção com bvh */
    bool bvh_intersect(Tree* tree, const tracer::ray &ray, float &t, float &u, float &v, size_t &geomID, size_t &primID) {
        if(tree->left == NULL && tree -> right == NULL){
            triangle* tri = (*tree->leaf).triangles[0];
            if (tracer::intersect_triangle(ray.origin, ray.dir, (*tri).vertices[0],
                    (*tri).vertices[1], (*tri).vertices[2], t, u, v)) {
                triangle* tri = (*tree->leaf).triangles[0];
                geomID = (*tri).geomID; // figura geométrica intercetada
                primID = (*tri).primID; // face(triângulo) intercetada 
            } 
        }
        else{
            bool interseta = box_intersect(ray, tree->leaf);
            if(tree->left && interseta)
                bvh_intersect(tree->left, ray, t, u, v, geomID, primID);
            if(tree->right && interseta)
                bvh_intersect(tree->right, ray, t, u, v, geomID, primID);
            
        }
        return (geomID != -1 && primID != -1);
    }

    /* Calcula a oclusão com bvh */
    bool bvh_occlusion(Tree* tree, const tracer::ray &ray, float &t) {
        float u, v;
        bool flag1 = false, flag2 = false;
        if(tree->left == NULL && tree -> right == NULL){
            triangle* tri = (*tree->leaf).triangles[0];
            if (tracer::intersect_triangle(ray.origin, ray.dir, (*tri).vertices[0],
                    (*tri).vertices[1], (*tri).vertices[2], t, u, v)) {
                return true;
            } 
        }
        else{
            bool interseta = box_intersect(ray, tree->leaf);
            if(tree->left && interseta)
                flag1 = bvh_occlusion(tree->left, ray, t);
            if(tree->right && interseta)
                flag2 = bvh_occlusion(tree->right, ray, t);
            return flag1 || flag2;
        }
        return false;
    }
 } // namespace tracer

