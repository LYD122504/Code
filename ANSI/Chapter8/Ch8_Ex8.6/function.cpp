#include "perdir.h"
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
/*
// 打开目录，返回一个 DIR* 指针
DIR *opendir(const char *dirname)
{
    int fd;
    struct stat stbuf;
    DIR *dp;

    // 逐步检查：
    // 1. open() 打开目录文件，返回文件描述符
    // 2. fstat() 获取文件状态，确认是目录
    // 3. (st_mode & S_IFMT) != S_IFDIR 表示不是目录
    // 4. malloc() 分配一个 DIR 结构体保存 fd
    if ((fd = open(dirname, O_RDONLY)) == -1
     || fstat(fd, &stbuf) == -1
     || (stbuf.st_mode & S_IFMT) != S_IFDIR
     || (dp = (DIR*)malloc(sizeof(DIR))) == NULL)
        return NULL;

    dp->fd = fd;   // 保存文件描述符
    return dp;     // 返回目录指针
}

// 读取目录，返回一个指向目录项的指针
Dirent *readdir(DIR* dfd)
{
    struct dirent dirbuf;   // 用来临时存放原始的目录项
    static Dirent d;        // 转换后的目录项（返回给用户，必须 static）

    // 循环读取目录项（每次一个 direct 大小）
    while (read(dfd->fd, (char *)&dirbuf, sizeof(dirbuf)) == sizeof(dirbuf)) {
        if (dirbuf.d_ino == 0)
            continue;   // d_ino=0 表示该目录项无效（已删除或空）

        d.ino = dirbuf.d_ino;  // 拷贝 inode 编号
        strncpy(d.name, dirbuf.d_name, NAME_MAX);  // 拷贝文件名（最多 DIRSIZ 个字符）
        d.name[NAME_MAX] = '\0';  // 确保字符串以 '\0' 结尾
        return &d;  // 返回目录项指针
    }
    return NULL;  // 目录读完，返回 NULL
}


// 关闭目录，释放资源
void closedir(DIR* dp)
{
    if (dp) {
        close(dp->fd);  // 关闭文件描述符
        free(dp);       // 释放 DIR 结构体内存
    }
}
*/
// function fsize: 打印文件name的长度
void fsize(const char *name)
{
    struct stat stbuf;  
    // 定义一个 stat 结构体，用来存放文件的状态信息（大小、类型、权限等）

    if (stat(name, &stbuf) == -1) {  
        // 调用 stat() 获取文件信息，失败时返回 -1
        // 可能失败的原因：文件不存在、权限不足等
        fprintf(stderr, "fsize: can't access %s\n", name);
        return;  
        // 获取不到文件信息就直接返回
    }

    if ((stbuf.st_mode & S_IFMT) == S_IFDIR)  
        // 检查文件类型：
        // st_mode & S_IFMT 提取文件类型信息;S_IFMT相当于是一个文件类型掩码
        // 如果结果等于 S_IFDIR，说明是一个目录
        dirwalk(name, fsize);  
        // 对目录执行 dirwalk()，遍历其中的所有文件和子目录
        // 遍历时对每个文件/子目录继续调用 fsize，实现递归

    printf("%5lu %6o %3lu %8ld %s\n", stbuf.st_ino, stbuf.st_mode, stbuf.st_nlink, stbuf.st_size, name);  
    // 输出文件大小（字节数）和文件名
    // %8ld 表示按长整型输出，占至少 8 个字符宽度，右对齐
}
// 遍历目录dir, 对每个文件调用fcn
// 遍历目录 dir，对其中的每个文件/子目录调用函数 fcn
void dirwalk(const char* dir, void (*fcn)(const char*)) {
    char name[MAX_PATH];
    DIR* dfd;
    struct dirent* dp;

    if ((dfd = opendir(dir)) == NULL) {
        perror(dir);
        return;
    }

    while ((dp = readdir(dfd)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;

        snprintf(name, sizeof(name), "%s/%s", dir, dp->d_name);
        (*fcn)(name);
    }
    closedir(dfd);
}
