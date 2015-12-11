#!/bin/bash
cursize=32
blocksize=512
rm block_size_512b_varying_chunksize.txt
touch block_size_512b_varying_chunksize.txt
while [ ${cursize} -lt 65536 ];
do
	while [ ${blocksize} -lt $((cursize*1024)) ];
	do
		for i in `seq 1 5`;
		do
			echo "Iteration ${i} for chunksize ${cursize} and blocksize ${blocksize}";
			./app ${cursize} $((blocksize-10)) >> block_size_512b_varying_chunksize.txt
		done
		blocksize=$((blocksize*2))
	done
	blocksize=512
	cursize=$((cursize*2))
done

