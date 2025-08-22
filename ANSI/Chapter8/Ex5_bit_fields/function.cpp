#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include"perstd.h"

#define PERMS 0666 // RW for owner, group, others

FILE _iob[OPEN_MAX] = {
    {0,NULL, NULL, {1, 0, 0, 0, 0}, 0},     // stdin
    {0,NULL, NULL, {0, 1, 0, 0, 0}, 1},     // stdout
    {0,NULL, NULL, {0, 1, 1, 0, 0}, 2}      // stderr
};

// 这里fopen只是设置了FILE的基本参数,但是并没有分配缓冲区,这需要用到getc里面的_fillbuf函数来实现
FILE *fopen(const char *name, const char *mode){
    int fd;// 文件描述符,用于标识已经打开的文件
    FILE *fp;//文件指针,用于返回封装好的文件信息

    //检查需要设置的模式是否合法
    if(*mode!='r'&&*mode!='w'&&*mode!='a')
        return NULL;

    //在文件指针数组中寻找一个空闲位置用于存放新打开的文件;如果没有找到空闲位置,则返回NULL
    for(fp=_iob;fp<_iob+OPEN_MAX;fp++)
        if((fp->flag.is_read==0)&& (fp->flag.is_write==0))
            break;
    if(fp>=_iob+OPEN_MAX)
        return NULL;
    
    // 根据希望的打开模式来设置文件权限
    if(*mode=='w')
        fd=creat(name,PERMS);
    else if(*mode=='a'){
        if(fd=open(name,O_WRONLY,0)==-1)
            fd=creat(name,PERMS);
        lseek(fd,0L,2);
    }
    else
        fd=open(name,O_RDONLY,0);
    if(fd==-1)
        return NULL;

    // 初始化文件指针的各个字段
    fp->fd=fd;
    fp->cnt=0;
    fp->base=NULL;
    fp->flag.is_unbuf=0;
    fp->flag.is_eof=0;
    fp->flag.is_err=0;
    if(*mode=='r'){
        fp->flag.is_read=1;
        fp->flag.is_write=0;
    }
    else{
        fp->flag.is_read=0;
        fp->flag.is_write=1;
    }
    return fp;
}

/*
_fillbuf有三个功能,分别为 
1. 判断文件的权限是否已经打开且可读,否则返回EOF 
2. 如果文件没有分配缓冲区,那么需要为其分配缓冲区 
3. 如果缓冲区已经被建立,那么就调用read填充缓冲区
*/
int _fillbuf(FILE *fp){
    int bufsize;
    // 判断文件权限:1.要求可读,2.不能有错,3.不能已经到达文件末尾
    if((fp->flag.is_read==0)||fp->flag.is_err||fp->flag.is_eof)
        return EOF;

    // 判断缓冲区大小：
    // 如果文件标记为无缓冲（_UNBUF），则只分配 1 字节的缓冲区；
    // 否则分配默认缓冲大小 BUFSIZ（通常是 1024 或 4096）
    bufsize=fp->flag.is_unbuf?1:BUFSIZ;
    // 如果尚未分配缓冲区（base 为 NULL），则使用 malloc 申请内存
    if(fp->base==NULL){
        if((fp->base=(char *)malloc(bufsize))==NULL)
            return EOF; // 如果分配缓冲区失败,则返回EOF
    }
    // 此时无论有无缓冲区,都已经生成了一个缓冲区,因此其实都可以当做缓冲区内无内容处理了
    // 设置指针 ptr 指向缓冲区开头
    fp->ptr=fp->base;
    // 从文件描述符 fp->fd 读取 bufsize 个字节到缓冲区
    // 返回值是读取的字节数，赋给 cnt
    fp->cnt=read(fp->fd,fp->ptr,bufsize);
    // _fillbuf函数返回的是一个字符,所以我们需要将读取的第一个字符返回
    if(--fp->cnt<0){
        // cnt==-1 表示文件已到结尾
        if(fp->cnt==-1)
            fp->flag.is_eof==1; // 如果读取到文件末尾,则设置EOF标志
        // cnt< -1 表示读取错误
        else
            fp->flag.is_err==1; // 如果读取出错,则设置ERR标志
        fp->cnt=0;
        return EOF;
    }
    // 返回读取到的第一个字符，并将 ptr 向前移动一位
    return (unsigned char)(*fp->ptr++);
}
