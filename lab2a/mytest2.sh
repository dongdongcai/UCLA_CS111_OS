#!/bin/bash

echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2a --iterations=100000 --threads=1 >> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2a --iterations=100000 --threads=2 >> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2a --iterations=100000 --threads=4 >> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2a --iterations=100000 --threads=6 >> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2a --iterations=100000 --threads=8 >> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2a --iterations=100000 --threads=10 >> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2a --iterations=100000 --threads=12 >> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2a --iterations=100000 --threads=14 >> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2a --iterations=100000 --threads=16 >> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2a --iterations=100000 --threads=18 >> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2a --iterations=100000 --threads=20 >> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2a --iterations=100000 --threads=24 >> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2a --iterations=100000 --threads=28 >> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2a --iterations=100000 --threads=32 >> outputn.txt 2>> error.txt
done
