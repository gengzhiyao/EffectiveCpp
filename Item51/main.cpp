#include <iostream>
#include <new>
#define MY_NEW nullptr

/**
 * !编写 new 和 delete 时需固守常规
 * 1. 保证 operator new 的返回值正确
 * 2. 内存不足时，需要循环调用 new-handler 直到成功分配内存或抛出异常
 * 3. 在申请0字节内存时，做好错误处理
 * 4. 避免掩盖正常行驶的 new
 * Notion: operator new 实际上不只一次分配内存，他会在每次申请失败后调用 new-handler ，假设 new-handler 函数能够释放一些内存，只有当指向 new-handler 的指针为 nullptr 时，才会抛出异常
 *
 * 尤其需要注意的是：若 operator new 是成员函数，那么就有可能会被继承，一旦被继承，那么可想而知，所分配的内存中只能容纳基类的大小，而我们针对某个 class 提供 operator new
 * 往往是为了最优化，也就是说，我们的这个需求是针对这个具体的类的，而不是针对该类继承体系下的派生类
 *
 * 对于数组版本的成员函数 operator new ，我们需要特殊处理，因为数组的大小是未知的，我们无法在 operator new 中直接使用 sizeof( Base ) 来分配内存，所以这里分配的内存应该是一块裸内存，也就是说，该内存中不包含任何的数据
 * 最后，编写了 operator new ，也要对应编写 operator delete ，否则会导致内存泄漏或内存损坏
 */

void* operator new( std::size_t size )
{
    if ( size == 0 )
    {
        size = 1;
    }

    while ( true )  // 退出当前循环的办法是：内存被成功分配、或 new-handler 中释放了内存、安装/卸载了 new-handler 、抛出异常、直接return等
    {
        void* pMem = malloc( size );
        if ( pMem != nullptr )
        {
            return pMem;
        }

        // 我们没有办法获得当前的 new_handler，只能使用 set_new_handler (nullptr) 找到当前的 new_handler，返回再回复回去
        std::new_handler globalHandler = std::set_new_handler( nullptr );
        std::set_new_handler( globalHandler );  // 恢复回去

        if ( globalHandler )
        {
            globalHandler( );  // 调用当前的 new_handler
        }
        else
        {
            throw std::bad_alloc( );// 没有 new_handler，抛出 std::bad_alloc
        }
        // 循环继续，尝试再次分配
    }
}

class Base
{
public:
    static void* operator new( std::size_t size ) noexcept
    {
        std::cout << "Base::operator new :: size = sizeof(Base)" << std::endl;// 只分配对象的基类部分，对象不完整

        // 除非添加错误处理机制
        if ( size != sizeof( Base ) )
        {
            return ::operator new( size );
        }

        // Dosomething else
        return MY_NEW;
    }

    static void operator delete( void* p, std::size_t size ) noexcept
    {
        if ( p == nullptr ) return; // 删除空指针可以安全执行

        if ( size != sizeof( Base ) )   // size 有问题时调用标准 operator delete
        {
            ::operator delete( p );
        }

        // 归还内存...
        return;
    }

    // virtual dtor
private:
    // data
};

class Derived : public Base
{
    // data
};

int main( )
{
    Derived* d = new Derived( );
    delete d;
    return 0;
}