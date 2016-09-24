#!/bin/bash

echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=1000 --threads=1 >> outputn.txt 2>> error.txt
done
