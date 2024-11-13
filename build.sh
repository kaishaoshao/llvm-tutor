script_dir="$(dirname "$(readlink -f "$0")")"
build_dir="${script_dir}/build"
llvm_local_dir="/usr/local/"
mkdir -p ${build_dir}
cmake -B "${build_dir}"                 \
      -DLT_LLVM_INSTALL_DIR=${llvm_local_dir}  \
       && make -C "${build_dir}" -j4

