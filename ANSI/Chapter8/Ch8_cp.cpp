#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>

#define PERMS 0666 // Read and write for owner, group and others
void error(const char*,...);
/*cp: copy file1 to file2*/
int main(int argc,char* argv[]) {
  int file1, file2, n;
  char buf[BUFSIZ];
  if (argc != 3)

	{	printf("%d",argc);
	  error("Usage: cp from to");}
  if ((file1 = open(argv[1], O_RDONLY, 0)) == -1)
    error("cp:can't open %s", argv[1]);
  if ((file2 = creat(argv[2], PERMS)) == -1)
    error("cp:can't create %s", argv[2]);
  while ((n = read(file1, buf, BUFSIZ)) > 0)
    if (write(file2, buf, n) != n)
      error("cp: write error on file %s", argv[2]);
  return 0;
}
void error(const char * fmt, ...) {
  va_list ap;
  va_start(ap,fmt);
  fprintf(stderr,"error: ");
  vfprintf(stderr,fmt,ap);
  fprintf(stderr,"\n");
  va_end(ap);
  exit(1);
}
