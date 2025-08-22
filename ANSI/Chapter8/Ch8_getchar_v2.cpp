#include<io.h>
//#include<unistd.h>
#include<stdio.h>
#undef getchar
/*getchar: simple buffered version*/
int getchar(void);
int main(){
    char c;
    while((c=getchar())!=EOF){
        write(1,&c,1);
    }
}
int getchar(void){
    static char buf[BUFSIZ];// 缓冲区
    static char *bufp=buf;
    static int n=0;
    if(n==0){
        printf("[READ CALLED]\n");
        fflush(stdout);
        n=read(0,buf,BUFSIZ);
        bufp=buf;
    }
    return(--n>=0)?*bufp++:EOF;
}