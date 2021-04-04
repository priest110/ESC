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
            switch(tri2.depth) {
                            case 0:{ // a.x
                                return (vertices[0].x < tri2.vertices[0].x);
                            }
                            case 1:{ // a.y
                                return (vertices[1].y < tri2.vertices[1].y);
                            }
                            case 2: { // a.z
                                return (vertices[2].z < tri2.vertices[2].z);
                            }
                            case 3: { // b.x
                                return (vertices[0].x < tri2.vertices[0].x);
                            }
                            case 4: { // b.y
                                return (vertices[1].y < tri2.vertices[1].y);
                            }
                            case 5: { // b.z
                                return (vertices[2].z < tri2.vertices[2].z);
                            }
                            case 6: { // c.x
                                return (vertices[0].x < tri2.vertices[0].x);
                            }
                            case 7: { // c.y
                                return (vertices[1].y < tri2.vertices[1].y);
                            }
                            case 8: { // c.z
                                return (vertices[2].z < tri2.vertices[2].z);
                            } default: break;
                        }    
                    }
    };

    

    struct Tree{
        Tree *left, *right;
        std::vector<tracer::triangle> leaf;
    };    

    Tree* createTree(std::vector<tracer::triangle> tri){
        Tree* newTree = new Tree();
        if(!newTree){
            printf("Memory error\n");
            return NULL;
        }
        newTree -> leaf = tri;
        newTree -> left = newTree -> right = NULL;
        return newTree;
    }
    
    void create_bvh(Tree *node, int depth, double axis_eval){
        std::vector<tracer::triangle> triangles = node -> leaf;

        if( triangles.size() > 2 ){
            std::sort(triangles.begin(), triangles.end());

            std::vector<tracer::triangle> tri_left, tri_right;

            for(tracer::triangle tri : triangles){
                tri.depth = depth;
                switch(depth) {
                    case 0:{ // a.x
                        if(tri.vertices[0].x < axis_eval)
                            tri_left.push_back(tri);
                        else
                            tri_right.push_back(tri);
                        break;
                    }
                    case 1: { // a.y
                        if(tri.vertices[0].y < axis_eval+1)
                            tri_left.push_back(tri);
                        else
                            tri_right.push_back(tri);
                        break;
                    }
                    case 2: { // a.z
                        if(tri.vertices[0].z < axis_eval)
                            tri_left.push_back(tri);
                        else
                            tri_right.push_back(tri);
                        break;
                    }
                    case 3: { // b.x
                        if(tri.vertices[1].x < axis_eval)
                            tri_left.push_back(tri);
                        else
                            tri_right.push_back(tri);
                        break;
                    }
                    case 4: { // b.y
                        if(tri.vertices[1].y < axis_eval)
                            tri_left.push_back(tri);
                        else
                            tri_right.push_back(tri);
                        break;
                    }
                    case 5: { // b.z
                        if(tri.vertices[1].z < axis_eval)
                            tri_left.push_back(tri);
                        else
                            tri_right.push_back(tri);
                        break;
                    }
                    case 6: { // c.x
                        if(tri.vertices[2].x < axis_eval)
                            tri_left.push_back(tri);
                        else
                            tri_right.push_back(tri);
                        break;
                    }
                    case 7: { // c.y
                        if(tri.vertices[2].y < axis_eval)
                            tri_left.push_back(tri);
                        else
                            tri_right.push_back(tri);
                        break;
                    }
                    case 8: { // c.z
                        if(tri.vertices[2].z < axis_eval)
                            tri_left.push_back(tri);
                        else
                            tri_right.push_back(tri);
                        break;
                    }
                }
            }

            //node -> leaf = NULL;  conteúdo a null

            node -> left = createTree(tri_left);
            node -> right= createTree(tri_right);
            
            if (axis_eval > -std::numeric_limits<float>::epsilon() &&
                    axis_eval < std::numeric_limits<float>::epsilon())
                axis_eval = (depth < 8) ? 0 : 1;
            else
                axis_eval = (depth < 8) ? axis_eval : axis_eval / 2;

            depth = (depth < 8) ? depth + 1 : 0;

            create_bvh(node -> left, depth, axis_eval);
            create_bvh(node -> right, depth, axis_eval);
        }
        else if(triangles.size() == 2){
            std::vector<tracer::triangle> tri_left, tri_right;
            tri_left.push_back(triangles[0]);
            tri_right.push_back(triangles[1]);
            node -> left = createTree(tri_left);
            node -> right= createTree(tri_right);
        }
        else if(triangles.size()== 0){
            
        }//Nothing

    }
    int i = 0;
    void printLeafNodes(Tree *root){
        // if node is null, return
        if (root->leaf.empty())
            return;

        // if node is leaf node, print its data   
        if (root->left == NULL && root->right == NULL){
            i++;
            printf("Triângulo %d\nVértice 1: ", i);
            std::cout << root->leaf[0].vertices[0].x << " ";
            std::cout << root->leaf[0].vertices[0].y << " ";
            std::cout << root->leaf[0].vertices[0].z << "\nVértice 2:";

            std::cout << root->leaf[0].vertices[1].x << " ";
            std::cout << root->leaf[0].vertices[1].y << " ";
            std::cout << root->leaf[0].vertices[1].z << "\nVértice 3:";
            
            std::cout << root->leaf[0].vertices[2].y << " ";
            std::cout << root->leaf[0].vertices[2].x << " ";
            std::cout << root->leaf[0].vertices[2].z << "\n";  
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
    

    
            /* Escolher um eixo e ordernar a lista de triângulos por esse eixo */
        /*     */