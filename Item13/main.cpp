#include <iostream>
#include <memory>
/**
 * !以对象管理资源(RAII)
 * Notion: 该条款示例中的auto_ptr已经 discard ,因而使用unique_ptr进行展示
 * 理论基础：依赖于 C++ 的一个硬性保证：当一个局部对象离开作用域时（无论是正常结束还是抛出异常），它的析构函数一定会被调用 核心思想是：利用栈上对象的生命周期来控制资源的生命周期
 * 对于shared_ptr，引用计数型的智能指针存在循环引用的问题
 * 本条款中，需要强调的是，对于 unique_ptr 和 shared_ptr ,默认行为是调用 delete，对于数组形式的对象，需要使用 delete[]
 * 数组形式：
 * std::unique_ptr<int[]> p(new int[10]);
 * std::shared_ptr<int[]> p(new int[10]);   // C++17以后
 */

 // 作用投资管理类的基类存在
class Investment    //! root class
{
    //...
};

// 其他投资行为 可能含有多种具体投资类
class ConcreteInvestment :public Investment
{
    //...
    // virtual destructor
};

Investment* CreateInvestment ()    //factory method
{
    // switch ... case... return new ???;
    return new ConcreteInvestment;
}

void foo ()
{
    // ...
    Investment* pInv = CreateInvestment (); // 尽量延后变量定义式出现的时间
    //...maybe contain loop/goto/continue/return etc.
    // 过早的return/异常/循环体内的 delete 过早break/后期代码的维护等种种情况下，可能会导致我们的程序无法到达这个 delete，造成内存泄露
    delete pInv;

    std::unique_ptr<Investment> pSmartInv ( CreateInvestment () );  // 获取对象之后立刻放进管理对象中，运用析构函数确保资源被释放
}

int main ()
{

    return 0;
}