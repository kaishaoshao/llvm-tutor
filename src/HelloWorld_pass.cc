// 实现HelloWorld pass
#include <llvm/ADT/StringRef.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

// 创建一个pass
// 使用命名空间防止污染
namespace {
// 创建visitor函数负责打印收到的函数类实例中的函数名称和参数数量
void visitor(Function &F){
      errs() << "(llvm20-tutor) Hello form: "<< F.getName() << "\n";
      errs() << "(llvm20-tutor) number of arguments" << F.arg_size() << "\n";
}  

// 新增新的Pass Manager实现
// 定义一个HelloWorld结构体，继承PassinInfoMixin<HelloWorld>
// PassInfoMixin是LLVM IR passes的基类
struct HelloWorld : PassInfoMixin<HelloWorld>{
    // PreservedAnalyses: 帮助 Pass 管理器（Pass Manager）
    // 了解哪些分析结果在 Pass 运行后仍然有效。

    // 定义了run方法，这个是Pass的主入口点，接受两个参数
    // Function &F: 表示要运行Pass的函数
    // FunctionnanlysisManager &：表示与该函数的相关分析管理起，可以用于查询分析结果
    PreservedAnalyses run(Function &F,FunctionAnalysisManager &){
        // 执行pass操作
        visitor(F);
        // 表示这个pass不会破坏任何分析结果，所有分析结果都保留
        return PreservedAnalyses::all();
    }
    // 告诉Pass Manager这个Pass是否必须被运行，如果返回true，即使函数被标记
    // 为optnone，这个Pass也会被运行，如果返回false，则会被优化级别-O0下的
    // optnone所忽略
    static bool isRequired(){
        return true;
    }
};

} // namespace



// 注册Pass

// 函数返回一个PassPluginLibraryInfo结构体，用于存储Pass的插件信息
// LLVM_PLUGIN_API_VERSION：这是 LLVM 插件 API 的版本号，用于确保兼容性。
// "HelloWorld": 插件的名称
// LLVM_VERSION_STRING：LLVM 版本号
// lambda函数：这是 PassBuilder 的回调函数，用于注册 Pass 的解析回调。
//     
llvm::PassPluginLibraryInfo getHelloWorldPluginInfo(){
    return {LLVM_PLUGIN_API_VERSION, "Helloworld", LLVM_VERSION_STRING,
            [](PassBuilder &PB) {
                PB.registerPipelineParsingCallback(
                    [](StringRef Name, FunctionPassManager &FPM,
                       ArrayRef<PassBuilder::PipelineElement>){
                        if(Name == "hello-world"){
                            FPM.addPass(HelloWorld());
                            return true;
                        }
                        return false;
                        });
        }
    };
}


// 它是Pass插件的核心接口，确保optgongju能够shibie并加载HelloWorld Pass
// 通过命令行参数-passes="hello-world"，可以将HelloWorld Pass添加到优化管道中
// LLVM_ATTRIBUTE_WEAK: 这是一个宏，用于指定该函数是一个弱符号（weak symbol）
// 弱符号允许链接器在找不到该符号时忽略它，而不是报错
// llvm::PassPluginLibraryInfo：这是返回值的类型，表示 Pass 插件的库信息。
// llvmGetPassPluginInfo：这是函数的名称，是 LLVM Pass 插件的标准入口点。
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
    return getHelloWorldPluginInfo();
}
