#include <iostream> 

/**
 * !了解 new和 delete 的合理替换时机
 * 在什么情况下，我们才需要替换原本的 operator new/operator delete 呢？
 * 1. 用于检测程序运行时的错误、内存泄漏、内存越界等，还有一种情况是：编程错误可能导致数据读写越界，要么超过分配点之前，要么超过分配点之后，如果能够通过自定义的 operator new/operator delete 来检测到这种情况，那么就可以及时发现并修复问题
 * 2. 为了强化程序的效率：编译器的 operator new/operator delete 适用于一般目的，在小对象的频繁分配和释放时，会引入额外的开销，自定义的 operator new/operator delete 可以一次性分配内存，减少内存碎片化，泛型分配器往往比定制型分配器慢，并且通常还使用更多的内存
 * 3. 收集使用信息，实现用于统计最大分配的内存容量等功能
 * 4. 可以从底层出发，通过自定义 operator new/operator delete ，减少 page fault ，提高内存访问效率
 * 
 * 观念上，写一个定制型的 operator new 十分简单，但是如果考虑到内存对齐，那么写一个健壮的 operator new 就比较复杂了，如下例所示
 * 
 * 很多时候，自己编写 operator new 是不必要的，某些编译器已经内置，另外，boost库中也提供了对应的实现
 */

static const int signature = 0xDEADBEEF;

typedef unsigned char byte;

// 用于协助检测内存读写越界的 operator new
// Notion: operator new 应该都内含一个循环，反复调用某个 new handler，直到分配成功为止
// 该程序通过malloc返回了一个偏移 int 大小的指针，指向对象的起始地址
// 如果用户申请 sizeof(double) 大小的内存，返回的指针是偏移了 sizeof(int) 的位置，也就是说，如果在要求 8 字节偏移的机器上，假设分配的起始地址是 0x1000
// 返回的指针是 0x1000 + 4 = 0x1004，但0x1004不是 8 字节对齐，而 8 字节对齐要求偏移地址一定是 8 的倍数，即：0,8,16,...
void* operator new(std::size_t size) 
{
    // 使用 set_new_handler 获取当前的 new_handler
    std::new_handler globalHandler = std::set_new_handler(nullptr);
    std::set_new_handler(globalHandler);  // 恢复回去

    while (true) {
        std::size_t realSize = size + sizeof(int) * 2;

        void* pMem = malloc(realSize);
        if (pMem != nullptr) {
            // 分配成功，将signature写入内存的最前端和最后端
            *static_cast<int*>(pMem) = signature;
            *reinterpret_cast<int*>(static_cast<byte*>(pMem) + realSize - sizeof(int)) = signature;

            // 返回指针，指向恰好位于第一个 signature 之后的位置
            return static_cast<byte*>(pMem) + sizeof(int);
        }

        // 分配失败，处理 new_handler
        if (globalHandler) {
            globalHandler();  // 调用用户的 new_handler
        } else {
            // 没有 new_handler，抛出 std::bad_alloc
            throw std::bad_alloc();
        }
        // 循环继续，尝试再次分配
    }
}

int main()
{
    
    return 0;
}