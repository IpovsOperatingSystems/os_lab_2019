sum=0
count=0
for var in "$@"; do
    sum=$(($sum + $var))
    count=$(($count + 1))
done

echo "average - $((sum / $count))"
echo "count - $count"
