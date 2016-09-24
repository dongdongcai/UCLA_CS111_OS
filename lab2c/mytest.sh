#!/bin/bash

echo "./lab2c --iterations=10000 --threads=1 --lists=8" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10000 --threads=1 --lists=8 >> output.txt 2>> error.txt
done

echo "./lab2c --iterations=10000 --threads=1 --lists=4" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10000 --threads=1 --lists=4 >> output.txt 2>> error.txt
done

echo "./lab2c --iterations=10000 --threads=1 --lists=2" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10000 --threads=1 --lists=2 >> output.txt 2>> error.txt
done

echo "./lab2c --iterations=10000 --threads=1 --lists=1" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10000 --threads=1 --lists=1 >> output.txt 2>> error.txt
done

echo "for mutex" >> output.txt
echo "./lab2c --iterations=10000 --threads=8 --lists=64 --sync=m" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10000 --threads=8 --lists=64 --sync=m >> output.txt 2>> error.txt
done

echo "./lab2c --iterations=10000 --threads=8 --lists=32 --sync=m" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10000 --threads=8 --lists=32 --sync=m >> output.txt 2>> error.txt
done

echo "./lab2c --iterations=10000 --threads=8 --lists=16 --sync=m" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10000 --threads=8 --lists=16 --sync=m >> output.txt 2>> error.txt
done

echo "./lab2c --iterations=10000 --threads=8 --lists=8 --sync=m" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10000 --threads=8 --lists=8 --sync=m >> output.txt 2>> error.txt
done

echo "./lab2c --iterations=10000 --threads=8 --lists=4 --sync=m" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10000 --threads=8 --lists=4 --sync=m >> output.txt 2>> error.txt
done

echo "./lab2c --iterations=10000 --threads=8 --lists=2 --sync=m" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10000 --threads=8 --lists=2 --sync=m >> output.txt 2>> error.txt
done

echo "./lab2c --iterations=10000 --threads=8 --lists=1 --sync=m" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10000 --threads=8 --lists=1 --sync=m >> output.txt 2>> error.txt
done

echo "for spin lock" >> output.txt
echo "./lab2c --iterations=10000 --threads=8 --lists=64 --sync=s" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10000 --threads=8 --lists=64 --sync=s>> output.txt 2>> error.txt
done

echo "./lab2c --iterations=10000 --threads=8 --lists=32 --sync=s" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10000 --threads=8 --lists=32 --sync=s >> output.txt 2>> error.txt
done

echo "./lab2c --iterations=10000 --threads=8 --lists=16 --sync=s" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10000 --threads=8 --lists=16 --sync=s >> output.txt 2>> error.txt
done

echo "./lab2c --iterations=10000 --threads=8 --lists=8 --sync=s" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10000 --threads=8 --lists=8 --sync=s >> output.txt 2>> error.txt
done

echo "./lab2c --iterations=10000 --threads=8 --lists=4 --sync=s" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10000 --threads=8 --lists=4 --sync=s >> output.txt 2>> error.txt
done

echo "./lab2c --iterations=10000 --threads=8 --lists=2 --sync=s" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10000 --threads=8 --lists=2 --sync=s >> output.txt 2>> error.txt
done

echo "./lab2c --iterations=10000 --threads=8 --lists=1 --sync=s" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10000 --threads=8 --lists=1 --sync=s >> output.txt 2>> error.txt
done