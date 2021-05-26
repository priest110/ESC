# Original
for((i = 0; i < $1; i++))
do
    ./bin/client $i & 
done

echo
