echo 'Compiling...'
# gcc p1.c -static -O2 -std=c11 -o p1
gcc p1.c -O2 -std=c11 -o p1

echo 'Executing...'
for i in {0..49}
do
    echo "number-$i"
    time ./p1 < ./hw0_testdata/gcd/"$i".in > ./p1_result/p1-out"-$i"
done

echo 'Check'
for i in {0..49}
do
    echo "$i.out"
    diff ./hw0_testdata/gcd/"$i".out ./p1_result/p1-out"-$i"
    echo "\n"
done
