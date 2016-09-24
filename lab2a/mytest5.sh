#!/bin/bash

echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2a --iterations=100000 --threads=1 --sync=c >> outputc.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2a --iterations=100000 --threads=2 --sync=c >> outputc.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2a --iterations=100000 --threads=4 --sync=c >> outputc.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2a --iterations=100000 --threads=6 --sync=c >> outputc.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2a --iterations=100000 --threads=8 --sync=c >> outputc.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2a --iterations=100000 --threads=10 --sync=c >> outputc.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2a --iterations=100000 --threads=12 --sync=c >> outputc.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2a --iterations=100000 --threads=14 --sync=c >> outputc.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2a --iterations=100000 --threads=16 --sync=c >> outputc.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2a --iterations=100000 --threads=18 --sync=c >> outputc.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2a --iterations=100000 --threads=20 --sync=c >> outputc.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2a --iterations=100000 --threads=24 --sync=c >> outputc.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2a --iterations=100000 --threads=28 --sync=c >> outputc.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2a --iterations=100000 --threads=32 --sync=c >> outputc.txt 2>> error.txt
done
