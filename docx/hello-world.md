## hello-world

展示了如何创建一个简单的 LLVM 传递。这个传递的作用是打印输入模块中每个定义的函数的名称以及它接受的参数数量。

## 生成.so

```bash
cd ../HelloWorld
mkdir -p build
cd build
cmake -DCMAKE_CXX_COMPILER=clang++ ../
make
```

## 生成LLVM测试文件

```bash
input_file="../inputs/input_for_hello.c"
output_file="./output/input_for_hello.ll"
cd ../
mkdir -p output
clang -O1 -S -emit-llvm ${input_file} -o  ${output_file}

```

### 参数

* -S: 生成汇编码
* -emit-llvm : 生成LLVM的中间表示（IR）文件，而不是汇编代码或可执行文件

## 运行Hello-world.so

```bash
build_file="./build/libHelloWorld.so"
opt -load-pass-plugin  ${build_file} -passes=hello-world -disable-output ${output_file}
```



### 参数

* -load-pass-plugin:opt工具的一个选项，用于加载一个LLVM pass插件。LLVM pass是用于分析和转换LLVM IR代码的模块。
* -passes=hello-world：这个选项指定了要运行的LLVM pass的名称。在这个例子中，它告诉opt工具运行名为hello-world的pass。
* -disable-output：这个选项告诉opt工具不要输出优化后的LLVM IR代码。如果不设置这个选项，opt工具会将优化后的代码输出到标准输出或文件。

### 效果

```bash
(llvm-tutor) Hello from: foo
(llvm-tutor)   number of arguments: 1
(llvm-tutor) Hello from: bar
(llvm-tutor)   number of arguments: 2
(llvm-tutor) Hello from: fez
(llvm-tutor)   number of arguments: 3
(llvm-tutor) Hello from: main
(llvm-tutor)   number of arguments: 2
```
