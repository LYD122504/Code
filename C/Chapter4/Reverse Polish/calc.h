//������ͷ�ļ�,����������Ҫ�ĺ���
#define NUMBER '0'
// ���������������
int getch();
void ungetch(int);
void ungets(char s[]);
int mod_getch();
void mod_ungetch(int);
//getop �����ַ���
int getop(char s[]);
int getline(char s[], int lim);
int mod_getop(char s[], char* line);
//ջ����
void push(double);
double pop();
double show_top();
void copy_top();
void swap_top();
void clean();
