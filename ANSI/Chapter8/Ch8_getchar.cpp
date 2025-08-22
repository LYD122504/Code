//#include<unistd.h>
#include<io.h>
#include <stdio.h>
/*getchar: unbuffered single character input */
int getchar(void);

int main()
{
    char c;
    while((c = getchar()) != EOF) {
        write(1,&c,1);
    }
}
int getchar(void){
    printf("[READ CALLED]\n");
    fflush(stdout);
    char c;
    return (read(0,&c,1))?(unsigned char) c:EOF;
}