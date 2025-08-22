#include "unistd.h"
#include "stdio.h"

int main() {
  char buf[BUFSIZ];
  int n;
  while ((n = read(0, buf, BUFSIZ)) > 0) {
    /* 这里我们用到了write和printf,但其实这是低级IO和高级IO混用的结果
    我们在Linux系统下write的输出会正常出现,而printf的输出却会在等write的结果输出完成后才会显示
    这是因为write是一个低级IO,他直接调用输出内核,无需经过stdout缓冲区
    而printf是一个高级IO,他会先写入到stdout缓冲区,因此不会实时输出结果
    当然我们可以通过显式刷新缓冲区,使其实时输出*/
    write(1, buf, n);
	  printf("%d\n",n);
    fflush(stdout); // 刷新stdout缓冲区
  }
}
