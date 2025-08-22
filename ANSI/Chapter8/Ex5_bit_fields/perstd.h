#define EOF (-1) // 定义文件结束标志,用于文件读写操作中标识结束或出错
#define BUFSIZ 1024 //定义默认的缓冲区默认大小为1024字节,一般用于做I/O缓冲区
#define OPEN_MAX 20 // max #files open at once

typedef struct {
    unsigned is_read:1;
    unsigned is_write:1;
    unsigned is_unbuf:1;
    unsigned is_eof:1; // end-of-file indicator
    unsigned is_err:1; // error indicator
} FILE_FLAGS;

// 自定义了一个类似于stdio.h的FILE结构体,用于文件操作
typedef struct _iobuf{
    int cnt; // character left in buffer: 当前缓冲区中还剩下的可读/写字符数
    char *ptr; // next character position: 指向当前缓冲区中下一个要读/写的字符位置
    char *base=NULL; // location of buffer: 指向缓冲区的起始位置
    FILE_FLAGS flag; // mode of file access: 文件状态标志
    int fd; // file descriptor: 文件描述符
} FILE;

// 声明一个FILE类型的数组_iob,代表所有打开的文件(最大20个)
extern FILE _iob[OPEN_MAX];

// 利用宏定义将_iob数组中的前三个元素映射到标准输入、输出和错误流
#define stdin (&_iob[0])
#define stdout (&_iob[1])
#define stderr (&_iob[2])

// 定义了两个内部函数,其用于处理缓冲区的填充和刷新操作
int _fillbuf(FILE *fp); //_fillbuf函数用于在输入时填充缓冲区
//int _flushbuf(int,FILE*); //_flushbuf函数用于在输出时刷新缓冲区

FILE *fopen(const char *name, const char *mode); // 声明fopen函数,用于打开文件并返回一个FILE指针

/*这里我们在宏定义里面使用了()来括起了操作数,这是因为我们需要利用括号的最高优先级来保证我们的操作顺序的正确性 */
#define feof(p) ((p)->flag.is_eof)// 判断是否到达文件末尾
#define ferror(p) ((p)->flag.is_err)// 判断是否发生错误
#define fileno(p) ((p)->fd)// 读取文件描述符

/* getc宏的作用是从指定文件中读取一个字符.
如果在缓冲区中仍有剩余字符可读,那么我们就直接从缓冲区中读取下一个字符并且将指针向后移动一位.
否则,我们就调用_fillbuf函数来填充缓冲区.
*/
#define getc(p) (--(p)->cnt>=0 ? (unsigned char)(*(p)->ptr++) : _fillbuf(p))

/* putc宏的作用是往指定文件中写入一个字符.
如果在缓冲区中还有剩余空间可写,那么我们就直接将字符写入缓冲区并且将指针向后移动一位.
否则,我们就调用_flushbuf函数来刷新缓冲区,并且将字符写入.
*/
#define putc(x,p) (--(p)->cnt>=0 ? (*(p)->ptr++ = (x)) : _flushbuf((x), p))

#define getchar() getc(stdin)
#define putchar(x) putc((x), stdout)