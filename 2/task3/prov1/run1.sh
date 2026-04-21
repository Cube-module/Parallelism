#!/bin/bash

rm -f results.txt

echo "N k T T_init" >> results.txt

for N in 3000
do
  for k in $(seq 1 40)
  do
    echo "Running N=$N k=$k"
    ./build/main1 $N $k
  done
done