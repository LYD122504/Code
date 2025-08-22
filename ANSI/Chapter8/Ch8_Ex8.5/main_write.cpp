#include <fcntl.h>
#include <unistd.h>
#include<stdlib.h>
#include "perstd.h" // 你的自定义头文件
#define write_stdout(fd,str) write(fd, str, strlen(str))  // 简单的输出到标准输出

int main() {
    FILE *fp;
    int c;

    // 打开文件 input.txt 以只读方式
    fp = fopen("input.txt", "w");
    if (fp == NULL) {
        write(2, "Failed to open file.\n", 22); // 输出错误信息到 stderr
        return 1;
    }
    putc('H', fp);
    putc('e', fp);
    putc('l', fp);
    putc('l', fp);
    putc('o', fp);
    putc(',', fp);
    // 刷新缓冲区，确保数据写入文件
    //write_stdout(fp->fd,"Flushing buffer...\n");
    fflush(fp);

    return 0;
}
