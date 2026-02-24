#include <iostream>
#include <memory>
/**
 * !在资源管理类中小心 copying 行为
 * 本条款告诉我们在资源管理类中，要把握好自定义的资源管理类是否真的需要拷贝行为
 * 使用 Mutex 的情况下，不需要拷贝行为，因此，书中将拷贝构造函数实施为了private/Uncopyable 但在C++11中，提供了 delete function
 */

class Mutex
{

};

// 假设以下为 C-style 的API
void Lock ( Mutex* pm ) { }
void Unlock ( Mutex* pm ) { }

class LockManager   //针对Item13给出的建议，我们自己实现一个资源管理类
{
public:
    explicit LockManager ( Mutex* pm )
        :mutexPtr ( pm )
    {
        Lock ( mutexPtr );
    }

    ~LockManager ()
    {
        Unlock ( mutexPtr );
    }

    LockManager ( const LockManager& rhs ) = delete;
    LockManager& operator=( const LockManager& rhs ) = delete;

private:
    Mutex* mutexPtr;
};

Mutex m;    //提供一个全局的互斥锁

int main ()
{
    {//block    用于定义critical section
        LockManager m1 ( &m );  //锁定互斥锁
        // dosomething ...  //执行一系列操作
    }// end block   在区块末尾，自动释放互斥锁
    // [WARN] 我们需要注意的是，在区块内假如存在对象的复制情况呢？
    LockManager m2 ( &m );
    // LockManager m3 ( m2 ); //不允许复制

    return 0;
}