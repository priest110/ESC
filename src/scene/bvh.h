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
        int depth = 0;
        std::vector<tracer::vec3<float>> vertices;

        bool operator < (const triangle& tri2) const{
            return (vertices[tri2.depth / 3][tri2.depth % 3] < tri2.vertices[tri2.depth / 3][tri2.depth % 3]);
        }
    };

    struct Tree{
        Tree *left, *right;
        tracer::triangle *leaf;
    };    

    Tree* createTri(tracer::triangle* tri){
        Tree* newTree = new Tree();
        if(!newTree){
            printf("Memory error\n");
            return NULL;
        }
        newTree -> leaf = tri;
        newTree -> left = newTree -> right = NULL;
        return newTree;
    }

    Tree* createEmpty(){
        Tree* newTree = new Tree();
        if(!newTree){
            printf("Memory error\n");
            return NULL;
        }
        newTree -> leaf = NULL;
        newTree -> left = newTree -> right = NULL;
        return newTree;
    }
    int j = 0;
    void create_bvh(Tree *node, std::vector<tracer::triangle*> triangles, int depth, double axis_eval){
      //printf("\n %d:Tamanho da lista: %lu", j, triangles.size());
      if( triangles.size() > 2 ){
            std::sort(triangles.begin(), triangles.end());

            std::vector<tracer::triangle*> tri_left, tri_right;

            for(tracer::triangle* tri : triangles){
                (*tri).depth = depth;
                if(depth%3 == 1)
                    axis_eval = 1 + axis_eval;
                if((*tri).vertices[depth / 3][depth % 3] < axis_eval)
                    tri_left.push_back(tri);
                else
                    tri_right.push_back(tri);
                if(depth%3 == 1)
                    axis_eval -=1;
            }
            //printf("\n %d:Tamanho da lista esquerda: %lu", j, tri_left.size());
            //printf("\n %d:Tamanho da lista direita: %lu", j++,tri_right.size());

            node -> left = createEmpty();
            node -> right= createEmpty();
            
            if (axis_eval > -std::numeric_limits<float>::epsilon() &&
                    axis_eval < std::numeric_limits<float>::epsilon())
                axis_eval = (depth < 8) ? 0 : 1;
            else
                axis_eval = (depth < 8) ? axis_eval : axis_eval / 2;

            depth = (depth < 8) ? depth + 1 : 0;

            create_bvh(node -> left, tri_left, depth, axis_eval);
            create_bvh(node -> right, tri_right, depth, axis_eval);
        }
        else if(triangles.size() == 2){
            node -> left = createTri(triangles[0]);
            node -> right= createTri(triangles[1]);
        }else if(triangles.size() == 1){
            node -> leaf = triangles[0];
            node -> left = NULL;
            node -> left = NULL;
        }
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
            printf("\nTriângulo %d\nVértice 1: ", i);
            std::cout << (*root->leaf).vertices[0].x << " ";
            std::cout << (*root->leaf).vertices[0].y << " ";
            std::cout << (*root->leaf).vertices[0].z << "\nVértice 2:";

            std::cout << (*root->leaf).vertices[1].x << " ";
            std::cout << (*root->leaf).vertices[1].y << " ";
            std::cout << (*root->leaf).vertices[1].z << "\nVértice 3:";
            
            std::cout << (*root->leaf).vertices[2].x << " ";
            std::cout << (*root->leaf).vertices[2].y << " ";
            std::cout << (*root->leaf).vertices[2].z << "\nGeomID: "; 
            std::cout << (*root->leaf).geomID << " PrimID: " << (*root->leaf).primID << "\n ";

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
    int m = 0;
    /* Calcula a interseção com bvh */
    bool bvh_intersect(Tree* tree, const tracer::vec3<float> &ori,
                const tracer::vec3<float> &dir, float &t, float &u, float &v,
                size_t &geomID, size_t &primID, int depth, double axis_eval) {
        
        if(tree->leaf != NULL && tree->left == NULL && tree->right == NULL){
            if (tracer::intersect_triangle(ori, dir, (*tree->leaf).vertices[0],
                    (*tree->leaf).vertices[1], (*tree->leaf).vertices[2], t, u, v)) {
                geomID = (*tree->leaf).geomID; // figura geométrica intercetada
                primID = (*tree->leaf).primID; // face(triângulo) intercetada 
                        
                return (geomID != -1 && primID != -1);
            } 
        }
        else if(tree->leaf == NULL && tree->left == NULL && tree->right == NULL){
            return (geomID != -1 && primID != -1);
        }
        else{
            bool right, left;
            if(tree->right)
                right = bvh_intersect(tree->right, ori, dir, t, u,v, geomID, primID, depth, axis_eval);
            if(right) return right;
            if(tree->left)
                left = bvh_intersect(tree->left, ori, dir, t, u,v, geomID, primID, depth, axis_eval);
            return left;
        } 
  /*      else if(tree->leaf == NULL && (tree->left != NULL || tree->right != NULL)){
            bool bi = false;
            bool uni; // false -> left, true -> right

            if(depth%3 == 1)
                axis_eval = 1 + axis_eval; // para o y somar 1
            bool flag1 = ori[depth%3] >= axis_eval;
            bool flag2 = dir[depth%3] < 0;
            if(depth%3 == 1)
                axis_eval -=1; // para subtrair -1 do y

            if (axis_eval > -std::numeric_limits<float>::epsilon() &&
                    axis_eval < std::numeric_limits<float>::epsilon())
                axis_eval = (depth < 8) ? 0 : 1;
            else
                axis_eval = (depth < 8) ? axis_eval : axis_eval / 2;

            depth = (depth < 8) ? depth + 1 : 0;

            if(flag1 && flag2)
                bi = true;
            else 
                uni = flag2 ? false : true; // se flag 2 então a coordenada do raio é <0, logo, vamos para a esquerda
            
            if(bi && (tree->left != NULL && tree->right != NULL)){ 
                bvh_intersect(tree->left, ori, dir, t, u,v, geomID, primID, depth, axis_eval);
                bvh_intersect(tree->right, ori, dir, t, u,v, geomID, primID, depth, axis_eval);
            }
            else{
                if(uni  && tree->right != NULL){
                    bvh_intersect(tree->right, ori, dir, t, u,v, geomID, primID, depth, axis_eval);
                }
                else if(!uni && tree -> left != NULL){
                    
                    bvh_intersect(tree->left, ori, dir, t, u,v, geomID, primID, depth, axis_eval);
                }
            } 
        }   
        return (geomID != -1 && primID != -1); */
    }
    
 } // namespace tracer


