
for algo in opt; do
    echo "algorithm: $algo" >> traces.out
    echo "++++++++++++++++++++++++++++++" >> traces.out
    for trace in tr-blocked.ref; do
	echo "$trace file" >> traces.out
        echo "------------------------------" >> traces.out
	for size in 50 100 150 200; do 
		echo "mem size: $size" >> traces.out
        	./sim -f $trace -m $size -s 3000 -a $algo | tail -n 7 >> traces.out
		echo "" >> traces.out
        done
    done
done
