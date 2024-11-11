#!/bin/bash


cd ../HelloWorld
mkdir -p build
cd build
cmake -DCMAKE_CXX_COMPILER=clang++ ../
make

input_file="../inputs/input_for_hello.c"
output_file="./output/input_for_hello.ll"
cd ../
mkdir -p output
clang -O1 -S -emit-llvm ${input_file} -o  ${output_file}

build_file="./build/libHelloWorld.so"
opt -load-pass-plugin  ${build_file} -passes=hello-world -disable-output ${output_file}