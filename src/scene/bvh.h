#include <algorithm>
#include <functional>
#include <array>
#include <iostream>
#include <cstdint>
#include <vector>
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

    int j = 0;
    
    void create_bvh(Tree *node, std::vector<tracer::triangle*> triangles, int axis){
      //printf("\n %d:Tamanho da lista: %lu", j, triangles.size());
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

            //printf("\n %d:Tamanho da lista esquerda: %lu", j, tri_left.size());
            //printf("\n %d:Tamanho da lista direita: %lu", j++,tri_right.size());

            axis = (axis == 2) ? 0 : axis+1;

            std::vector<std::future<void>> tasks;
            tasks.push_back(std::async(
                    [&](Tree *node) -> void {
                        BBox* left = createBBox(tri_left);
                        node -> left = createTree(left);
                        create_bvh(node -> left, tri_left, axis);
                    },node));
            tasks.push_back(std::async(
                    [&](Tree *node) -> void {
                        BBox* right= createBBox(tri_right);
                        node -> right= createTree(right);
                        create_bvh(node -> right, tri_right, axis);
                    },node));

            //create_bvh(node -> left, tri_left, axis);
            //create_bvh(node -> right, tri_right, axis);
        }
    }

    /* Calcula a interseção com bbox */
    bool box_intersect(const tracer::ray ray, BBox* box){
        float tmin, tmax, tmin_y, tmax_y, tmin_z, tmax_z;
        /* Versão otimizada */
        // x
        tmin = ((*box).vertices[ray.sinal[0]].x - ray.origin.x)/ray.inv_dir.x;
        tmax = ((*box).vertices[1-ray.sinal[0]].x - ray.origin.x)/ray.inv_dir.x;

        // y
        tmin_y = ((*box).vertices[ray.sinal[1]].y - ray.origin.y)/ray.inv_dir.y;
        tmax_y = ((*box).vertices[1-ray.sinal[1]].y - ray.origin.y)/ray.inv_dir.y;

        if(tmin > tmax_y || tmin_y > tmax)
            return false;
        if(tmin_y > tmin) tmin = tmin_y;
        if(tmax_y < tmax) tmax = tmax_y;

        // z
        tmin_z = ((*box).vertices[ray.sinal[2]].z - ray.origin.z)/ray.inv_dir.z;
        tmax_z = ((*box).vertices[1-ray.sinal[2]].z - ray.origin.z)/ray.inv_dir.z;

        if(tmin > tmax_z || tmin_z > tmax)
            return false;
        if(tmin_z > tmin) tmin = tmin_z;
        if(tmax_z < tmax) tmax = tmax_z;

        /* Versão normal */
/*        // x
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
*/
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
    
    int i = 0;
    void printLeafNodes(Tree *root){
        //printf("\n %f ", tri.vertices[0].x);
        //newTree -> leaf = &tri;
        //printf("%f \n", (*newTree ->leaf).vertices[0].x);
        // if node is null, return
        if (root->leaf == NULL && root->left == NULL && root->right == NULL)
            return;
        // if node is leaf node, print its data   
        if (root->leaf != NULL && root->left == NULL && root->right == NULL){
                i++;

                printf("\nBounding Box %d\nVértice 1:", i);
                std::cout << (*root->leaf).vertices[0].x << " ";
                std::cout << (*root->leaf).vertices[0].y << " ";
                std::cout << (*root->leaf).vertices[0].z << "\nVértice 2:";

                std::cout << (*root->leaf).vertices[1].x << " ";
                std::cout << (*root->leaf).vertices[1].y << " ";
                std::cout << (*root->leaf).vertices[1].z << "\n";
            
         /*   for(triangle* tri : (*root->leaf).triangles){
                printf("\nTriângulo %d\nVértice 1: ", i);
                std::cout << (*tri).vertices[0].x << " ";
                std::cout << (*tri).vertices[0].y << " ";
                std::cout << (*tri).vertices[0].z << "\nVértice 2:";

                std::cout << (*tri).vertices[1].x << " ";
                std::cout << (*tri).vertices[1].y << " ";
                std::cout << (*tri).vertices[1].z << "\nVértice 3:";
                
                std::cout << (*tri).vertices[2].x << " ";
                std::cout << (*tri).vertices[2].y << " ";
                std::cout << (*tri).vertices[2].z << "\nGeomID: "; 
                std::cout << (*tri).geomID << " PrimID: " << (*tri).primID << "\n ";
            }*/
            return;
        }
        // if left child exists, check for leaf
        // recursively
        if (root->left)
        printLeafNodes(root->left);
            
       // if right child exists, check for leaf
        // recursively
        if (root->right)
        printLeafNodes(root->right); 
    }
 } // namespace tracer


