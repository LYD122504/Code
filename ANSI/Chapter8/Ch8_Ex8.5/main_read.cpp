#include <fcntl.h>
#include <unistd.h>
#include<stdlib.h>
#include "perstd.h" // 你的自定义头文件

int main() {
    FILE *fp;
    int c;

    // 打开文件 input.txt 以只读方式
    fp = fopen("input.txt", "r");
    if (fp == NULL) {
        write(2, "Failed to open file.\n", 22); // 输出错误信息到 stderr
        return 1;
    }

    // 循环读取每个字符，直到到达 EOF
    while ((c = getc(fp)) != EOF) {
        // 使用系统调用 write 输出到标准输出（fd=1）
        write(1, &c, 1);
    }

    return 0;
}
