#!/bin/bash

for i in $(ls *.dat); do
	./a.out $i &
done
