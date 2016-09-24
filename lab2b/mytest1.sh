#!/bin/bash

echo "./lab2b --iterations=10 --threads=1" > error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=10 --threads=1 >> output.txt 2> error.txt
done

echo "./lab2b --iterations=20 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=20 --threads=1 >> output.txt 2>> error.txt
done

echo "./lab2b --iterations=50 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=50 --threads=1 >> output.txt 2>> error.txt
done

echo "./lab2b --iterations=100 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=100 --threads=1 >> output.txt 2>> error.txt
done

echo "./lab2b --iterations=200 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=200 --threads=1 >> output.txt 2>> error.txt
done

echo "./lab2b --iterations=500 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=500 --threads=1 >> output.txt 2>> error.txt
done

echo "./lab2b --iterations=1000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=1000 --threads=1 >> output.txt 2>> error.txt
done

echo "./lab2b --iterations=2000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=2000 --threads=1 >> output.txt 2>> error.txt
done

echo "./lab2b --iterations=5000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=5000 --threads=1 >> output.txt 2>> error.txt
done

echo "./lab2b --iterations=10000 --threads=1" >> error.txt
for i in `seq 1 10`; do
	./lab2b --iterations=10000 --threads=1 >> output.txt 2>> error.txt
done
