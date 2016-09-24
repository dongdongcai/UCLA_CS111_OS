#!/bin/bash

echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=1000 --threads=1 --sync=s>> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=1000 --threads=2 --sync=s>> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=1000 --threads=4 --sync=s>> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=1000 --threads=6 --sync=s>> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=1000 --threads=8 --sync=s>> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=1000 --threads=10 --sync=s>> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=1000 --threads=12 --sync=s>> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=1000 --threads=14 --sync=s>> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=1000 --threads=16 --sync=s>> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=1000 --threads=18 --sync=s>> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=1000 --threads=20 --sync=s>> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=1000 --threads=24 --sync=s>> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=1000 --threads=28 --sync=s>> outputn.txt 2>> error.txt
done
echo "./lab2a --iterations=100000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=1000 --threads=32 --sync=s>> outputn.txt 2>> error.txt
done
