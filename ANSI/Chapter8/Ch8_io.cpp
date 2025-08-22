/*
  书中提到的直接调用了syscalls.h,但是由于使用的系统是Ubuntu,并非书中提及的Unix系统.

  因此我们用man read来查找Ubuntu系统对read的操作手册,
  #include <unistd.h>
  ssize_t read(int fd, void *buf, size_t count);
  所以从此我们知道在ubuntu系统中read和write函数定义在unistd.h头文件里

  对于BUFSIZ是Unix系统的缓冲区,其值是在系统内部设定的,我们用grep BUFSIZ
  /usr/include/*来在usr/include/的所有文件中查找BUFSIZ.
  /usr/include/stdio.h:#define BUFSIZ 8192
  /usr/include/stdio.h:   Else make it use buffer BUF, of size BUFSIZ.
  所以我们需要引入stdio.h
*/
#include "unistd.h"
#include "stdio.h"
#define BUFSIZE 4
/*copy input to output*/
int main() {
  char buf[BUFSIZ];
  int n;
  while (n = read(0, buf, BUFSIZ))
  /*
	这里如果我们自定义一个BUFSIZ,如果BUFSIZ过小,例如我们记BUFSIZ是4,那么他每次都会从缓冲区中读取4个字符,而如果输入字符不是BUFSIZ的整数倍,那么在最后一次读写的时候,会出现读取字符数小于BUFSIZ的情况,同时将其写出.
   */
    write(1, buf, n);
  return 0;
}
