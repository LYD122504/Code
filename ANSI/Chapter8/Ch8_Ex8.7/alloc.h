
#define NALLOC 1024  // 请求的最小块大小
#define MAXBYTE (unsigned) 10240 //单次申请最大的字节数
// 定义一个类型别名 Align，用于强制内存对齐。
// 在很多机器上，long 类型的对齐要求足够覆盖绝大多数基本类型，
// 这样可以保证 malloc 返回的内存可以安全存放任何类型的数据。
typedef long Align;  

// 内存块的头部结构（block header）
// 每一个空闲或已分配的内存块前面都会有这么一个 header，用来记录块大小和链表指针。
union header {
    struct {
        /* 
        这里在 union header 定义过程中使用了 union header*，
        这是 C/C++ 里合法的“不完全类型”用法。
        因为我们只定义了一个指针（其大小是已知的，和具体 union header 内部无关），
        所以编译器是允许的。
         
        这样可以实现链表结构：每个 header 节点通过 ptr 指向下一个空闲块。
        */
        union header *ptr;   // 指向下一个空闲内存块（用于维护空闲链表）
        unsigned size;       // 当前块的大小（以 Header 单位计，而不是字节数）
    } s;

    /*
    用 Align 类型成员占位，确保整个 header 的大小至少满足 long 的对齐要求。
    因为 union 的大小取决于“最大成员”的大小，
    所以加上 Align 可以避免 header 太小导致对齐问题。
    */
    Align x;  
};

// 为 union header 定义一个更简洁的别名 Header，方便在代码中使用。
typedef union header Header;

static Header base;          // 空闲链表的起始节点（哨兵节点）
static Header *freep = nullptr; // 指向空闲链表的当前节点
static unsigned maxalloc;

void *malloc(unsigned nbytes);

static Header *morecore(unsigned nu);

void free(void *ap);

void *calloc(unsigned n, unsigned size);

unsigned bfree(char *p, unsigned n);
