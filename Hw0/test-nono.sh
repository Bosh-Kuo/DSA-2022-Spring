echo 'Compiling...'
# gcc p1.c -static -O2 -std=c11 -o p1
gcc nonogram.c -O2 -std=c11 -o nonogram

echo 'Executing...'
for i in {0..99}
do
    echo "number-$i"
    time ./nonogram < ./hw0_testdata/nonogram/"$i".in > ./nonogram_result/nonogram-out"-$i"
done

echo '\nCheck'
for i in {0..99}
do
    echo "$i.out"
    diff ./hw0_testdata/nonogram/"$i".out ./nonogram_result/nonogram-out"-$i"
    echo "\n"
done
