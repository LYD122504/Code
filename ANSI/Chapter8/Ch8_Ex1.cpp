#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define BUFSIZE 1024
void filecopy(int fd1,int fd2,char buf[]);
// cat: concatenate files, unix
int main(int argc, char *argv[]) {
  int fd;
  char buf[BUFSIZE]={};
  if (argc == 1)
    filecopy(0, 1, buf);
  else
    while (--argc > 0)
      if ((fd = open(*++argv, O_RDONLY, 0)) < 0) {
        fprintf(stderr, "cat: can't open %s.\n", *argv);
		exit(1);
      }
	  else {
        filecopy(fd, 1, buf);
		close(fd);
		  }
}
void filecopy(int fd1, int fd2, char buf[]) {
  int n;
  while ((n = read(fd1, buf, BUFSIZE)) > 0) {
    write(fd2, buf, BUFSIZE);
	//这个语句的作用是因为如果我们循环的话很容易出现后面的长度不够长,无法覆盖已有的内容,因此需要用这个将其清空.
	memset(buf,0,strlen(buf));
  }
}
