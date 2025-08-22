#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include<stdlib.h>
int get(int fd, long pos, char *buf, int n);
int main(int argc, char *argv[]) {
  int fd;
  char buf[BUFSIZ]={};
  long pos = 2;
  int n=12;
  if (argc == 1)
    fd = 0;
  else
    if ((fd = open(argv[1], O_RDONLY, 0)) == -1) {
    fprintf(stderr, "Error:open %s", argv[1]);
	exit(1);
  }
  if(get(fd, pos, buf, n)!=-1)
	write(1, buf, BUFSIZ);
  close(fd);
}
int get(int fd, long pos, char *buf, int n) {
  if (lseek(fd, pos, 0) >= 0)
    return read(fd, buf, n);
  else
	return -1;
}
