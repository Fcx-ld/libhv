#include "hv/hv.h"

int main() {
    char exe_filepath[MAX_PATH] = {0};
    char run_dir[MAX_PATH] = {0};

    // ��ȡhv����汾
    const char* version = hv_compile_version();

    // ��ȡ��ִ���ļ�·��
    get_executable_path(exe_filepath, sizeof(exe_filepath));
    // ��ȡ����Ŀ¼
    get_run_dir(run_dir, sizeof(run_dir));

    printf("exe_filepath=%s\n", exe_filepath);
    printf("run_dir=%s\n", run_dir);

    // д��־
    LOGI("libhv version: %s", version);

    return 0;
}
