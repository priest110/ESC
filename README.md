# Paralelização e Optimização de um ray tracer

-    Using std::threads or std::async make all necessary changes to make the code parallel (3 points)
-    Implement a KD-tree sequentially to accelerate the ray-triangle intersection. (3 points)
-    Convert the KD-tree sequential construction into a parallel construction. (4 points)
-    Implement ambient occlusion in the ray tracer. (1 point)
-    Implement a locked queue and change the number of threads to match the core count of the machine. You should queue a group of pixels to render, and the working thread should pull a single-pixel block to use in the tracer function. The main thread should push all pixel blocks into the queue. (4 points)
-    Implement the tracer function using either CUDA or ISPC (5 points)
