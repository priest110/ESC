
echo 
echo "SEQUENTIAL SPHERE"

# ASYNC
# Original
for((i = 0; i < 5; i++))
do
    ./src/ESCViewer2021Seq -v 0,1,3 -l 0,1,0 -m /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/models/cornell/CornellBox-Sphere.obj
done
echo 
echo "LQ ORIGINAL"

# ASYNC
# Original
for((i = 0; i < 10; i++))
do
    ./src/ESCViewer2021LQ -v 0,1,3 -l 0,1,0 -m /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/models/cornell/CornellBox-Original.obj
done

echo
echo "LQ MIRROR"

# Mirror
for((i = 0; i < 10; i++))
do
    ./src/ESCViewer2021LQ -v 0,1,3 -l 0,1,0 -m /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/models/cornell/CornellBox-Mirror.obj
done

echo
echo "LQ SPHERE"

# Sphere
for((i = 0; i < 10; i++))
do
    ./src/ESCViewer2021LQ -v 0,1,3 -l 0,1,0 -m /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/models/cornell/CornellBox-Sphere.obj
done

echo
echo "LQ WATER"

# Water
for((i = 0; i < 10; i++))
do
    ./src/ESCViewer2021LQ -v 0,1,3 -l 0,1,0 -m /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/models/cornell/CornellBox-Water.obj
done

echo 
echo "BVH ORIGINAL"

# BVH
# Original
for((i = 0; i < 10; i++))
do
    ./src/ESCViewer2021BVH -v 0,1,3 -l 0,1,0 -m /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/models/cornell/CornellBox-Original.obj
done

echo
echo "BVH MIRROR"

# Mirror
for((i = 0; i < 10; i++))
do
    ./src/ESCViewer2021BVH -v 0,1,3 -l 0,1,0 -m /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/models/cornell/CornellBox-Mirror.obj
done

echo
echo "BVH SPHERE"

# Sphere
for((i = 0; i < 10; i++))
do
    ./src/ESCViewer2021BVH -v 0,1,3 -l 0,1,0 -m /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/models/cornell/CornellBox-Sphere.obj
done

echo
echo "BVH WATER"

# Water
for((i = 0; i < 10; i++))
do
    ./src/ESCViewer2021BVH -v 0,1,3 -l 0,1,0 -m /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/models/cornell/CornellBox-Water.obj
done

echo 
echo "BVH + PARALLEL CONST. ORIGINAL"

# BVH + PARALLEL CONSTRUCTION
# Original
for((i = 0; i < 10; i++))
do
    ./src/ESCViewer2021BVHP -v 0,1,3 -l 0,1,0 -m /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/models/cornell/CornellBox-Original.obj
done

echo
echo "BVH + PARALLEL CONST. MIRROR"

# Mirror
for((i = 0; i < 10; i++))
do
    ./src/ESCViewer2021BVHP -v 0,1,3 -l 0,1,0 -m /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/models/cornell/CornellBox-Mirror.obj
done

echo
echo "BVH + PARALLEL CONST. SPHERE"

# Sphere
for((i = 0; i < 10; i++))
do
    ./src/ESCViewer2021BVHP -v 0,1,3 -l 0,1,0 -m /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/models/cornell/CornellBox-Sphere.obj
done

echo
echo "BVH + PARALLEL CONST. WATER"

# Water
for((i = 0; i < 10; i++))
do
    ./src/ESCViewer2021BVHP -v 0,1,3 -l 0,1,0 -m /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/models/cornell/CornellBox-Water.obj
done

echo 
echo "ASYNC + BVH + PARALLEL CONST. ORIGINAL"

# ASYNC + BVH + PARALLEL CONSTRUCTION
# Original
for((i = 0; i < 10; i++))
do
    ./src/ESCViewer2021ABVHP -v 0,1,3 -l 0,1,0 -m /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/models/cornell/CornellBox-Original.obj
done

echo
echo "ASYNC + BVH + PARALLEL CONST. MIRROR"

# Mirror
for((i = 0; i < 10; i++))
do
    ./src/ESCViewer2021ABVHP -v 0,1,3 -l 0,1,0 -m /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/models/cornell/CornellBox-Mirror.obj
done

echo
echo "ASYNC + BVH + PARALLEL CONST. SPHERE"

# Sphere
for((i = 0; i < 10; i++))
do
    ./src/ESCViewer2021ABVHP -v 0,1,3 -l 0,1,0 -m /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/models/cornell/CornellBox-Sphere.obj
done

echo
echo "ASYNC + BVH + PARALLEL CONST. WATER"

# Water
for((i = 0; i < 10; i++))
do
    ./src/ESCViewer2021ABVHP -v 0,1,3 -l 0,1,0 -m /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/models/cornell/CornellBox-Water.obj
done

echo 
echo "LQ + BVH + PARALLEL CONST. ORIGINAL"

# LOCKED QUEUE + BVH + PARALLEL CONST
# Original
for((i = 0; i < 10; i++))
do
    ./src/ESCViewer2021LQBVHP -v 0,1,3 -l 0,1,0 -m /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/models/cornell/CornellBox-Original.obj
done

echo
echo "LQ + BVH + PARALLEL CONST. MIRROR"

# Mirror
for((i = 0; i < 10; i++))
do
    ./src/ESCViewer2021LQBVHP -v 0,1,3 -l 0,1,0 -m /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/models/cornell/CornellBox-Mirror.obj
done

echo
echo "LQ + BVH + PARALLEL CONST. SPHERE"

# Sphere
for((i = 0; i < 10; i++))
do
    ./src/ESCViewer2021LQBVHP -v 0,1,3 -l 0,1,0 -m /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/models/cornell/CornellBox-Sphere.obj
done

echo
echo "LQ + BVH + PARALLEL CONST. WATER"

# Water
for((i = 0; i < 10; i++))
do
    ./src/ESCViewer2021LQBVHP -v 0,1,3 -l 0,1,0 -m /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/models/cornell/CornellBox-Water.obj
done

echo 