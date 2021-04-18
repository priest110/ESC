
echo 
echo "ISPC"

# ASYNC
# Original
for((i = 0; i < 5; i++))
do
    ./src/ESCViewer2021ISPC -v 0,1,3 -l 0,1,0 -m /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/models/cornell/CornellBox-Sphere.obj
done

echo 
echo "ISPC"

# ASYNC
# Original
for((i = 0; i < 5; i++))
do
    ./src/ESCViewer2021ISPC -v 0,1,3 -l 0,1,0 -m /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/models/cornell/CornellBox-Water.obj
done


echo 
echo "ISPC ORIGINAL"

# ASYNC
# Original
for((i = 0; i < 10; i++))
do
    ./src/ESCViewer2021ISPC -v 0,1,3 -l 0,1,0 -m /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/models/cornell/CornellBox-Original.obj
done

echo
echo "ISPC MIRROR"

# Mirror
for((i = 0; i < 10; i++))
do
    ./src/ESCViewer2021ISPC -v 0,1,3 -l 0,1,0 -m /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/models/cornell/CornellBox-Mirror.obj
done
