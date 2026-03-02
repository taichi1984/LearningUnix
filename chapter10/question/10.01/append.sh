letters=(A B C D E F G H I J)

for i in {0..4}
do
  (
    for j in {1..50}
    do
      head -c 10000 /dev/zero | tr '\0' "${letters[$i]}" >> testfile
    done
  ) &
done
wait
