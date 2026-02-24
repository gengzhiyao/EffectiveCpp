#include <iostream>

/**
 * !在 operator= 中处理自我赋值
 * 自我赋值在 C++ 中是可以编译通过的合法语句。虽然想象中，这种情况一般不会发生，但实际上，由于 C++ 中的别名机制，自我赋值的情况一旦发生，就比较难以排除
 * 例如：若在一个循环语句中，a[i] = a[j]，有潜在的自我赋值情况
 * 或者说：*px = *py 若px和py指向的是同一个对象，那么这里的赋值也会是自我赋值的情况
 * 如果在考虑到指针/引用/继承，那么就更加难以发现了，尤其是在继承关系中，父类的指针若原本指向一个对象，而在赋值后又指向了相同的一个派生类的对象，这种自我赋值很难排查
 * 最好的解决 自我赋值 的办法：使用 copy & swap idiom
 */

class Bitmap
{

};

class Widget
{
    //...
private:
    Bitmap* pb;
public:
    Widget& operator=( const Widget& rhs );
};

// 版本1：一份不安全的 operator= 版本
// 自我赋值：函数内的*this和rhs有可能是同一个对象，在这种情况下：
// 我们首先delete了当前对象的位图对象，但是this和rhs都指向了同一个对象
// 那么在 *rhs.bp 代码处，一定会访问已经释放的对象
Widget& Widget::operator=( const Widget& rhs )
{
    std::cout << "Widget::operator=" << std::endl;
    // if ( this == &rhs )   return *this;  // 解决自我赋值的传统做法：藉由证同测试

    delete this->pb;
    pb = new Bitmap ( *rhs.pb );    // 使用rhs的bitmap对象*pb来构建rhs的副本即lhs，调用copy constructor
    return *this;
}
// 以上代码即使进行了证同测试，同样也是不安全的，因为：如果new抛出了异常，当前对象的pb指针指向的是一块已经被我们自己 delete 的对象，是悬空指针

// 版本2：
/**
 * Notion: 安全处理自我赋值的 operator= 版本
 * 即便是在 new 的时候抛出了异常，当前对象的pb所指向的仍然是原本的那块 Bitmap 对象
 * 该手法的实现完全是基本手工排列语句，保证了处理自我赋值和提供异常安全保证
 * 当然，由于函数内部存在堆内存的分配和释放，如果你更加关注效率，你也可以在该函数的开始处放置证同测试，但也需要考虑是否会增加目标代码文件的机器码
Widget& Widget::operator=( const Widget& rhs )
{
    Bitmap* pOrigin = pb;
    pb = new Bitmap ( *rhs.pb );
    delete pOrigin;
    return *this;
}
 */


int main ()
{
    Bitmap* pBitmap = new Bitmap;
    Widget* pWidget1 = new Widget;
    pWidget1 = pWidget1;    // 编译器可能会进行优化，跳过这段代码
    Widget* pWidget2 ( pWidget1 );

    return 0;
}