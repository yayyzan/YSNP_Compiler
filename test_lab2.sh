#!/bin/bash

make clean
make

echo -e "$(cat test.c | ./bin/print_canonical)"


