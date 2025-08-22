#include <stdio.h>
#include <string.h>

// 假设 alloc.h 里已经包含了你的 malloc/free 声明
#include "alloc.h"
int main() {
    printf("=== 测试自定义 malloc/free ===\n");

    // 分配 100 字节
    char *p1 = (char *)malloc(100);
    if (!p1) {
        printf("malloc 失败\n");
        return 1;
    }
    strcpy(p1, "Hello, mmap malloc!");
    printf("p1 内容: %s\n", p1);

    // 分配 200 字节
    char *p2 = (char *)malloc(200);
    if (!p2) {
        printf("malloc 失败\n");
    }
    strcpy(p2, "Another block with custom malloc");
    printf("p2 内容: %s\n", p2);

    // 释放第一个块
    free(p1);
    printf("p1 已释放\n");

    // 再分配 50 字节
    char *p3 = (char *)malloc(50);
    if (!p3) {
        printf("malloc 失败\n");
        return 1;
    }
    strcpy(p3, "New allocation after free");
    printf("p3 内容: %s\n", p3);

    // 释放剩余块
    free(p2);
    free(p3);
    printf("p2, p3 已释放\n");

    return 0;
        }
