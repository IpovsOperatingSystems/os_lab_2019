for i in {1..150}
do
    od -An -N1 -i /dev/random >> numbers.txt
done
