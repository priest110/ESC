# Paralelização e Optimização de um ray tracer

- Diferentes Mains

Existem 3 mains(main_async.cpp, que tem a paralelizão do ray tracing com uso de asyncs, construção da bvh paralelizada e ambient occlusion; main_ispc.cpp, que tem a paralelização da interseção do raio com os triângulos usando ispc; main_lq, que tem a paralelização do ray tracing com uso de uma locked queue e construção da bvh). Para usar cada uma é necessário ir à CMakeLists.txt na diretoria tracer e alterar na linha 10 para a main que queremos correr.

- Flags de compilação

Deverão ser adicionadas à CmakeCache.txt(diretoria tracer, na linha 42) as flags "scene/ispc.o -no-pie"(sendo que o "-no-pie" dependerá do sistema utilizado) para a linkagem do ispc ao projeto e "-pthread" para a biblioteca das pthreads.

- ISPC

Para corrermos o ispc primeiro é necessário correr antes "ispc -o ispc.o -h ispc.h" na ditoria scene.
