#!/bin/bash

echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=1000 --threads=1 --sync=m>> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=1000 --threads=2 --sync=m>> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=1000 --threads=4 --sync=m>> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=1000 --threads=6 --sync=m>> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=1000 --threads=8 --sync=m>> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=1000 --threads=10 --sync=m>> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=1000 --threads=12 --sync=m>> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=1000 --threads=14 --sync=m>> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=1000 --threads=16 --sync=m>> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=1000 --threads=18 --sync=m>> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=1000 --threads=20 --sync=m>> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=1000 --threads=24 --sync=m>> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=1000 --threads=28 --sync=m>> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=1000 --threads=32 --sync=m>> outputn.txt 2>> error.txt
done