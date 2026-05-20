#include <iostream>

/**
 * !写了 placement new ，也要写 placement delete
 * 首先，说明什么是 placement new 和 placement delete
 * placement new 是在指定内存地址上构造对象的运算符，placement new 在一般性术语的情况下，是指除了含有 size 参数之外，含有任意其他参数的 operator new，也就是说，placement new 是指任意 operator new
 * 的重载版本 placement delete 是在指定内存地址上析构对象的运算符，placement delete 在一般性术语的情况下，如同 placement new 一样
 * Notion: 但是：我们常常说的 placement new 是指标准库中提供的 operator new(std::size_t size, void* pMemory) 这个版本，这个也是日常开发中常用的一个版本
 *
 * 接下来说明，为什么需要 placement new 和 placement delete 成对出现
 */

class Widget
{
   // data
public:
    static void* operator new( std::size_t size, std::ostream& os ) { return malloc( size ); }
    static void  operator delete( void* pMemory, std::ostream& os ) { free( pMemory ); }
};

int main( )
{
    Widget* pWidget = new ( std::cerr ) Widget;
   /**
     * 以上代码中，new 做了两件事：1. 调用 operator new 分配内存，2. 调用 Widget 的构造函数初始化内存
     * 如果在 operator new 分配内存失败，new 会抛出 std::bad_alloc 异常，同时，它分配的那块内存并没有指针指向它，也就意味着，客户已经无法访问到那块内存了，也就无法释放那块内存了
     * 实际上，C++ 的运行库会自动调用标准的 operator delete 来释放那块内存
     * 标准的 operator new 签名为：void* operator new(std::size_t size); 对应的标准的 global 域中的 operator delete(void* pMemory) / class 域中的operator delete(void* pMemory, std::size_t size);
     * 但是，如果这个 new 是 placement new，如果抛出异常了，运行库并不知道要去调用哪个 placement delete 来释放那块内存，也就无法释放那块内存，造成内存泄漏
     * 只有在我们正确**配对**了 placement new 和 placement delete ，运行库才能知道到底应该调用哪个 operator delete 来释放那块内存
     * Notion: 所以，对应的 placement delete 为：void operator delete(void* pMemory, std::ostream& os);
     *
     * 另外需要注意的一点是：delete pWidget; 调用的不是 placement delete， 而是标准的 operator delete(void* pMemory);
     * placement delete 只有在 placement new 内存分配失败的情况下才会被主动调用，对一个指针调用 delete 绝不会导致调用 placement delete 来释放那块内存
     * 这意味着：如果要对所有的 placement new 相关的内存泄漏宣战，应该提供正常的 operator delete 以及 placement delete
     *
     * 但这确实并非一件容易的事情，为什么呢？
     * 1. 如果在全局作用域中声明operator new(std::size_t size) 那么，该 operator new 会替换掉全局作用域中的 operator new
     * 2. 如果在类作用域中声明operator new(std::size_t size, void* pMemory)  那么，当使用 new 创建对象时，就不会调用全局作用域中的 operator new
     * 3. 如果父类中已经声明了 operator new，那么子类中只要声明了 operator new，不论是何种形式，都会掩盖父类中的 operator new，一定不会发生重载！
     * ! 子类不能跨作用域重载父类同名函数 重载只能发生在同一个作用域里
     * 如果要在类中声明一个 operator new，请注意：除非你就是要阻止用户使用标准形式的 operator new(std::size_t size) 这个版本，否则确保那些标准形式的 operator new 还可用
     */
    return 0;
}

// 确保标准型的 operator delete 也可用
// 建立一个基类，内含所有的标准形式的 operator new 和 operator delete
// 凡是想以自定形式扩充标准形式的用户，利用继承以及 using 声明式，取得标准形式
class StandardNewDeleteForms
{
public:
    static void  operator delete( void* pMemory ) noexcept { ::operator delete( pMemory ); }
    static void* operator new( std::size_t size ) { return ::operator new( size ); }
    static void* operator new( std::size_t size, void* pMemory ) { return ::operator new( size, pMemory ); }
    static void  operator delete( void* pMemory, void* ptr ) { ::operator delete( pMemory, ptr ); }
};

class Widget : public StandardNewDeleteForms
{
public:
    using StandardNewDeleteForms::operator new;
    using StandardNewDeleteForms::operator delete;
    // 实现自己的逻辑
    static void  operator delete( void* pMemory, std::ostream& os );
    static void* operator new( std::size_t size, std::ostream& os );
};