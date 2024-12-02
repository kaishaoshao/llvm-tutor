#!/bin/bash
scipts_dir="$(dirname "$(readlink -f "")")"
pass="$scipts_dir/src/$1.cc"
so_output="$scipts_dir/output/$1.so"
input_file="$scipts_dir/inputs/input_for_hello.c"
output_file="$scipts_dir/output/input_for_hello.ll"

clang++ -fPIC -shared ${pass} -o ${so_output}  
clang -O1 -S -emit-llvm ${input_file} -o  ${output_file}
opt -load-pass-plugin ${so_output} -passes=hello-world  -disable-output ${output_file}