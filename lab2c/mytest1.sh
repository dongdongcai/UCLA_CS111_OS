#!/bin/bash

echo "./lab2c --iterations=10" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10 >> output.txt 2>> error.txt
done
gprof lab2c gmon.out >> analysis.txt

echo "./lab2c --iterations=20" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=20 >> output.txt 2>> error.txt
done
gprof lab2c gmon.out >> analysis.txt

echo "./lab2c --iterations=50" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=50 >> output.txt 2>> error.txt
done
gprof lab2c gmon.out >> analysis.txt

echo "./lab2c --iterations=100" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=100 >> output.txt 2>> error.txt
done
gprof lab2c gmon.out >> analysis.txt

echo "./lab2c --iterations=200" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=200 >> output.txt 2>> error.txt
done
gprof lab2c gmon.out >> analysis.txt

echo "./lab2c --iterations=500" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=500 >> output.txt 2>> error.txt
done
gprof lab2c gmon.out >> analysis.txt

echo "./lab2c --iterations=1000" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=1000 >> output.txt 2>> error.txt
done
gprof lab2c gmon.out >> analysis.txt

echo "./lab2c --iterations=2000" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=2000 >> output.txt 2>> error.txt
done
gprof lab2c gmon.out >> analysis.txt

echo "./lab2c --iterations=5000" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=5000 >> output.txt 2>> error.txt
done
gprof lab2c gmon.out >> analysis.txt

echo "./lab2c --iterations=10000" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10000 >> output.txt 2>> error.txt
done
gprof lab2c gmon.out >> analysis.txt

echo "for mutex" >> output.txt
echo "./lab2c --iterations=10000 --threads=8 --lists=64 --sync=m" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10000 --threads=8 --lists=64 --sync=m >> output.txt 2>> error.txt
done
gprof lab2c gmon.out >> analysis.txt

echo "./lab2c --iterations=10000 --threads=8 --lists=32 --sync=m" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10000 --threads=8 --lists=32 --sync=m >> output.txt 2>> error.txt
done
gprof lab2c gmon.out >> analysis.txt

echo "./lab2c --iterations=10000 --threads=8 --lists=16 --sync=m" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10000 --threads=8 --lists=16 --sync=m >> output.txt 2>> error.txt
done
gprof lab2c gmon.out >> analysis.txt

echo "./lab2c --iterations=10000 --threads=8 --lists=8 --sync=m" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10000 --threads=8 --lists=8 --sync=m >> output.txt 2>> error.txt
done
gprof lab2c gmon.out >> analysis.txt

echo "./lab2c --iterations=10000 --threads=8 --lists=4 --sync=m" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10000 --threads=8 --lists=4 --sync=m >> output.txt 2>> error.txt
done
gprof lab2c gmon.out >> analysis.txt

echo "./lab2c --iterations=10000 --threads=8 --lists=2 --sync=m" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10000 --threads=8 --lists=2 --sync=m >> output.txt 2>> error.txt
done
gprof lab2c gmon.out >> analysis.txt

echo "./lab2c --iterations=10000 --threads=8 --lists=1 --sync=m" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10000 --threads=8 --lists=1 --sync=m >> output.txt 2>> error.txt
done
gprof lab2c gmon.out >> analysis.txt

echo "for spin lock" >> output.txt
echo "./lab2c --iterations=10000 --threads=8 --lists=64 --sync=s" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10000 --threads=8 --lists=64 --sync=s>> output.txt 2>> error.txt
done
gprof lab2c gmon.out >> analysis.txt

echo "./lab2c --iterations=10000 --threads=8 --lists=32 --sync=s" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10000 --threads=8 --lists=32 --sync=s >> output.txt 2>> error.txt
done
gprof lab2c gmon.out >> analysis.txt

echo "./lab2c --iterations=10000 --threads=8 --lists=16 --sync=s" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10000 --threads=8 --lists=16 --sync=s >> output.txt 2>> error.txt
done
gprof lab2c gmon.out >> analysis.txt

echo "./lab2c --iterations=10000 --threads=8 --lists=8 --sync=s" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10000 --threads=8 --lists=8 --sync=s >> output.txt 2>> error.txt
done
gprof lab2c gmon.out >> analysis.txt

echo "./lab2c --iterations=10000 --threads=8 --lists=4 --sync=s" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10000 --threads=8 --lists=4 --sync=s >> output.txt 2>> error.txt
done
gprof lab2c gmon.out >> analysis.txt

echo "./lab2c --iterations=10000 --threads=8 --lists=2 --sync=s" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10000 --threads=8 --lists=2 --sync=s >> output.txt 2>> error.txt
done
gprof lab2c gmon.out >> analysis.txt

echo "./lab2c --iterations=10000 --threads=8 --lists=1 --sync=s" >> error.txt
for i in `seq 1 10`; do
	./lab2c --iterations=10000 --threads=8 --lists=1 --sync=s >> output.txt 2>> error.txt
done
gprof lab2c gmon.out >> analysis.txt