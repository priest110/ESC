#include "ispc.h"
#include <stdio.h>



    int main(){
        ispc::__ISPC_STRUCT_triangle__ triangle tri;
        tri.vertices[0].v[0] = 0.0;
        tri.vertices[0].v[1] = 0.0;
        tri.vertices[0].v[2] = 0.0;
        tri.vertices[1].v[0] = 1.0;
        tri.vertices[1].v[1] = 1.0;
        tri.vertices[1].v[2] = 1.0;
        tri.vertices[2].v[0] = 0.5;
        tri.vertices[2].v[1] = 0.5;
        tri.vertices[2].v[2] = 0.5;
        ispc::nada(tri);
        return 0;
    }

