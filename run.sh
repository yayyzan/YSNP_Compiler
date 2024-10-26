#!/bin/bash

clear
make clean
make
bin/c_compiler -S test.c -o test.s
