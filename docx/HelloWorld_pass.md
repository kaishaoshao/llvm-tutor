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

## 复现

参考src/HelloWorld_pass.cc

### 头文件解析

#### llvm/Support/raw_ostream.h

#### llvm/ADT/StringRef.h

#### llvm/IR/Function.h

Function

#### llvm/IR/PassManager.h

#### llvm/Passes/PassBuilder.h

#### llvm/Passes/PassPlugin.h

### 结构体

#### PassInfoMixin

#### PreservedAnalyses

`PreservedAnalyses` 是 LLVM 编译器基础设施中的一个类，用于表示一个 Pass 在运行后保留了哪些分析结果。在 LLVM 中，Pass 可以对程序的中间表示（IR）进行各种优化和转换，而这些优化和转换可能会改变 IR 的状态，从而影响其他分析的结果。`PreservedAnalyses` 类帮助 Pass 管理器（Pass Manager）了解哪些分析结果在 Pass 运行后仍然有效。

##### 主要用途

1. **保留分析结果**：当一个 Pass 运行后，它可以通过 `PreservedAnalyses` 对象告知 Pass 管理器哪些分析结果没有被破坏，仍然可以信任。
2. **优化性能**：通过保留有效的分析结果，可以避免不必要的重复计算，提高编译器的整体性能。

##### 常见用法

- **返回所有分析结果**：如果一个 Pass 没有修改 IR，或者它的修改不会影响任何分析结果，可以返回 `PreservedAnalyses::all()`，表示所有分析结果都保留。

  ```cpp
  return PreservedAnalyses::all();
  ```
- **保留特定分析结果**：如果一个 Pass 只修改了某些部分，可以使用 `PreservedAnalyses` 的 `preserve` 方法来保留特定的分析结果。

  ```cpp
  PreservedAnalyses PA;
  PA.preserve<SomeAnalysis>();
  return PA;
  ```
- **不保留任何分析结果**：如果一个 Pass 修改了 IR 的多个部分，导致大多数分析结果不再有效，可以返回 `PreservedAnalyses::none()`。

  ```cpp
  return PreservedAnalyses::none();
  ```

##### 示例

假设你有一个 Pass，它修改了一些函数的局部变量，但不影响全局变量的分析结果，你可以这样写：

```cpp
PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
  // 执行 Pass 的具体操作
  modifyLocalVariables(F);

  // 创建一个 PreservedAnalyses 对象
  PreservedAnalyses PA;

  // 保留全局变量分析结果
  PA.preserve<GlobalVariableAnalysis>();

  // 返回 PreservedAnalyses 对象
  return PA;
}
```

##### 总结

`PreservedAnalyses` 是一个重要的机制，帮助 LLVM 的 Pass 管理器了解 Pass 运行后的状态，从而优化后续的分析和优化过程。通过合理使用 `PreservedAnalyses`，可以提高编译器的效率和性能。

#### PassPluginLibraryInfo



#### Lambda 函数

```cpp
[](PassBuilder &PB) {
  PB.registerPipelineParsingCallback(
      [](StringRef Name, FunctionPassManager &FPM,
         ArrayRef<PassBuilder::PipelineElement>) {
        if (Name == "hello-world") {
          FPM.addPass(HelloWorld());
          return true;
        }
        return false;
      });
}
```

* 这个 lambda 函数接收一个 `PassBuilder` 对象 `PB` 作为参数。
* `PB.registerPipelineParsingCallback` 方法用于注册一个管道解析回调函数。
  * 回调函数接收三个参数：
    * `StringRef Name`：管道名称。
    * `FunctionPassManager &FPM`：函数级别的 Pass 管理器。
    * `ArrayRef<PassBuilder::PipelineElement>`：管道元素列表。
  * 如果 `Name` 等于 `"hello-world"`，则将 `HelloWorld` Pass 添加到 `FPM` 中，并返回 `true`。
  * 否则，返回 `false`。
