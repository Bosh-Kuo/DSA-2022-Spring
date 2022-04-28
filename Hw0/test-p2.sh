echo 'Compiling...'
# gcc p1.c -static -O2 -std=c11 -o p1
gcc p2.c -O2 -std=c11 -o p2

echo 'Executing...'
for i in {0..99}
do
    echo "number-$i"
    time ./p2 < ./hw0_testdata/nonogram/"$i".in > ./p2_result/p2-out"-$i"
done

echo 'Check'
for i in {0..99}
do
    echo "$i.out"
    diff ./hw0_testdata/nonogram/"$i".out ./p2_result/p2-out"-$i"
    echo "\n"
done
