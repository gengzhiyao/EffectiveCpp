#include <iostream>

/**
 * !若不想使用编译器生成的特种成员函数，就明确拒绝
 * Notion: 由于本条款过于老旧，现代C++中提倡的是 = delete
 */

class Uncopyable
{
private://古派作风，声明为private并且不去实现，其他类可以继承该类，但是要注意多重继承问题
    Uncopyable ( const Uncopyable& rhs );
    Uncopyable& operator=( const Uncopyable& rhs );
};

int main ()
{

    return 0;
}