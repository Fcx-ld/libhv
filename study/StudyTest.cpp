#include "hv/hv.h"

int main() {
    char exe_filepath[MAX_PATH] = {0};
    char run_dir[MAX_PATH] = {0};

    // 获取hv编译版本
    const char* version = hv_compile_version();

    // 获取可执行文件路径
    get_executable_path(exe_filepath, sizeof(exe_filepath));
    // 获取运行目录
    get_run_dir(run_dir, sizeof(run_dir));

    printf("exe_filepath=%s\n", exe_filepath);
    printf("run_dir=%s\n", run_dir);

    // 写日志
    LOGI("libhv version: %s", version);

    return 0;
}
