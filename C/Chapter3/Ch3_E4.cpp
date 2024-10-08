#include<stdio.h>
#include<string.h>
#include<limits.h>
#define MAXSIZE 200
void reverse(char s[]);
void itoa(int n, char s[]);
int main()
{
    char s[MAXSIZE] = {};
    int a;
    printf("请输入一个数字:");
    scanf("%d", &a);
    itoa(a,s);
    printf("数字转换的字符串为");
    printf("%s\n", s);
}
void itoa(int n, char s[])
{
    int i, sign;
    int flag=0;
    if (n==INT_MIN)
      {
	flag=1;
	n=n+1;
      }
    if ((sign = n) < 0)//record sign
        n = -n;//make n positive
    i = 0;
    do {//generate digits in reverse order
        s[i++] = n % 10 + '0';//get next digit
	if(flag)
	  {
	  s[i-1]+=1;
	  flag--;
	  }
	} while ((n /= 10) > 0);//delete it
    if(sign<0)
		s[i++]='-';
    s[i]= '\0';
	reverse(s);
}
//reverse: reverse string s in place
void reverse(char s[])
{
    int c, i, j;
    for (i = 0, j = strlen(s) - 1; i < j; i++, j--)
    {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}
