#include "alloc.h"
#include <unistd.h>    // 提供 sbrk、sysconf 等系统调用接口（Linux/Unix）
#include <sys/mman.h>  // 提供 mmap、munmap、mprotect 等内存映射函数
#include <stdio.h>     // 提供 printf 等标准 I/O 函数

// ==========================
// 自定义 malloc 函数实现
// ==========================
void *malloc(unsigned nbytes) {
    Header *p, *prevp;  // p 遍历空闲链表的指针，prevp 记录前驱节点
    unsigned nunits;    // 需要分配的块数（以 Header 单位计）

    // 检查请求的内存是否超过限制
    if (nbytes > MAXBYTE) {
        fprintf(stderr, "alloc:can't allocate more than %u bytes\n.", MAXBYTE);
        return NULL;
    }

    // 计算需要多少 Header 单位，+1 是头部占用，-1 用于向上取整
    nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;

    // 如果 free list 还未初始化，则创建一个环形链表哨兵节点
    if ((prevp = freep) == NULL) {
        base.s.ptr = freep = prevp = &base;  // base 是哨兵节点
        base.s.size = 0;                     // base 本身不存储实际数据
    }

    // 遍历 free list 查找合适空闲块
    for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
        // 找到空闲块足够大
        if (p->s.size >= nunits) {
            if (p->s.size == nunits) {
                // 空间刚好 → 从链表摘掉整块
                prevp->s.ptr = p->s.ptr;
            } else {
                // 空间比需要的大 → 拆分
                p->s.size -= nunits;  // 剩余部分保留
                p += p->s.size;       // p 指向新分配的块头
                p->s.size = nunits;   // 设置新块大小
            }
            freep = prevp;            // 更新 free list 起点
            return (void *)(p + 1);   // 返回数据区地址（跳过头部）
        }

        // 遍历一圈没找到 → 需要向系统申请更多内存
        if (p == freep) {
            if ((p = morecore(nunits)) == NULL)
                return NULL;          // 系统无法提供更多内存
        }
    }
}

// ==========================
// 向系统申请更多内存
// ==========================
static Header *morecore(unsigned nu) {
    if (nu < NALLOC) nu = NALLOC; // 最少申请 NALLOC 个 Header 单位

    size_t nbytes = nu * sizeof(Header);  // 转换为字节数

    // 使用 mmap 分配匿名内存
    void *vp = mmap(NULL, nbytes,
                    PROT_READ | PROT_WRITE,
                    MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (vp == MAP_FAILED) return NULL; // mmap 失败

    Header *up = (Header *)vp;
    up->s.size = nu; // 设置新块大小

    // 更新最大分配块记录（用于 free 时简单检查）
    maxalloc = (up->s.size > maxalloc) ? up->s.size : maxalloc;

    // 将新申请的内存加入 free list
    free((void *)(up + 1));

    return freep; // 返回 free list 起点
}

// ==========================
// 自定义 free 函数
// ==========================
void free(void *ap) {
    Header *bp, *p;
    bp = (Header *)ap - 1; // 指向块头（Header）

    // 安全检查：块大小为 0 或超过最大分配块
    if (bp->s.size == 0 || bp->s.size > maxalloc) {
        fprintf(stderr, "free:can't free %u units\n", bp->s.size);
        return;
    }

    // 在 free list 中找到合适位置插入
    for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr) {
        // 处理链表首尾跨界情况
        if (p >= p->s.ptr && (bp > p || bp < p->s.ptr)) break;
    }

    // 合并后继空闲块
    if (bp + bp->s.size == p->s.ptr) {
        bp->s.size += p->s.ptr->s.size;
        bp->s.ptr = p->s.ptr->s.ptr;
    } else {
        bp->s.ptr = p->s.ptr;
    }

    // 合并前驱空闲块
    if (p + p->s.size == bp) {
        p->s.size += bp->s.size;
        p->s.ptr = bp->s.ptr;
    } else {
        p->s.ptr = bp;
    }

    freep = p; // 更新 free list 起点
}

// ==========================
// 自定义 calloc 函数
// ==========================
void *calloc(unsigned n, unsigned size) {
    unsigned nsize = n * size;
    void *p, *q;

    // 调用 malloc 分配内存
    if ((p = q = malloc(nsize)) != NULL) {
        // 初始化为 0
        for (unsigned i = 0; i < nsize; i++)
            *(char *)q++ = 0;
    }

    return p;
}

unsigned bfree(char *p, unsigned n) {
  Header *bp;
  if (n < sizeof(Header))
    return 0;
  bp=(Header *)p;
  bp->s.size = n / sizeof(Header);
  free((void *)(bp + 1));
  return bp->s.size;
}
