#include<sys/stat.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

#define NAME_MAX 14 //最长的文件名:具体由操作系统决定
#define MAX_PATH 1024 //路径长度限制
/*
typedef struct{ //可移植的目录项
    long ino;   // i node的编号
    char name[NAME_MAX+1]; // 文件名并加上'\0'
} Dirent;

typedef struct{ //最小的DIR: 无缓冲等特性
    int fd; // 目录的文件描述符
    Dirent d; // 当前目录项
} DIR;*/
/*
// 打开目录, 返回一个指向DIR的指针
DIR *opendir(const char *dirname);
// 读取目录, 返回一个指向目录项的指针
Dirent *readdir(DIR* dfd);
// 关闭目录
void closedir(DIR* dp);*/
/*
struct stat {
    dev_t st_dev;     // 设备编号
    ino_t st_ino;     // i-node编号
    mode_t st_mode;   // 文件类型和权限
    nlink_t st_nlink; // 硬链接数
    uid_t st_uid;     // 用户ID
    gid_t st_gid;     // 组ID
    dev_t st_rdev;    // 设备编号（如果是特殊文件）
    off_t st_size;    // 文件大小（字节）
    blksize_t st_blksize; // 块大小（字节）
    blkcnt_t st_blocks;   // 块计数
    struct timespec st_atim; // 最后访问时间
    struct timespec st_mtim; // 最后修改时间
    struct timespec st_ctim; // 最后状态改变时间
};
*/
/*
st_mode对应的值为:
S_IFMT   0160000  // 文件类型的掩码
S_IFDIR  0040000  // 目录
S_IFREG  0100000  // 普通文件
S_IFCHR  0020000  // 字符设备
S_IFBLK  0060000  // 块设备
S_IFLNK  0120000  // 符号链接
S_IFSOCK 0140000  // 套接字
S_IFIFO  0010000  // 命名管道
S_ISUID  0004000  // 设置用户ID位
S_ISGID  0002000  // 设置组ID位
S_ISVTX  0001000  // 粘着位
S_IRUSR  0000400  // 用户可读
S_IWUSR  0000200  // 用户可写
S_IXUSR  0000100  // 用户可执行
S_IRGRP  0000040  // 组可读
S_IWGRP  0000020  // 组可写
S_IXGRP  0000010  // 组可执行
S_IROTH  0000004  // 其他可读
S_IWOTH  0000002  // 其他可写
S_IXOTH  0000001  // 其他可执行
 */
// function fsize: 打印文件name的长度
void fsize(const char *name);
// 遍历目录dir, 对每个文件调用fcn
void dirwalk(const char *dir, void (*fcn)(const char*));
