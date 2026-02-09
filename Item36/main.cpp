#include <iostream>

/**
 * !绝不要重新定义继承而来的 non-virtual 函数
 * Notion: 本条款与条款33相呼应
 * 为什么？ => 涉及动态绑定、静态绑定以及虚函数机制 vtable vptr
 * 动态绑定的前提：
 * 1. 子类通过public方式继承父类
 * 2. 通过父类的 pointer/reference 进行调用
 * 3. 进行动态绑定的函数须要在父类中声明为 virtual 函数
 * Notion: non-virtual 函数是静态绑定的，即使是使用父类的 pointer/reference 进行调用，也只会调用各自的版本，不会发生多态
 * 动态绑定的虚函数，不论是使用父类/子类的 pointer/reference 进行调用，最终调用的都是子类中的实现
 * 当 non-virtual 函数被调用时，其行为表现不在于产生的对象&d的类型，而在于调用该函数的那个 pointer/reference 所声明的类型(pD/pB)
 *      for example: pD=&d / pB=&d 在这种情况下，对象d会根据调用 pointer/reference 类型的不同表现出精神分裂般不同的行为！
 */

#define REDEFINE_NON_VIRTUAL_FUNC 1     // 控制子类中是否重写父类中的 non-virtual 函数（隐藏父类函数）

class Base
{
public:
    void mf1 ()
    {
        std::cout << "Base::mf1()" << std::endl;
    }

    virtual void mf2 ()
    {
        std::cout << "Base::mf2()" << std::endl;
    }
};

class Derived :public Base
{
public:
#if REDEFINE_NON_VIRTUAL_FUNC
    void mf1 ()
    {
        std::cout << "Derived::mf1()" << std::endl;
    }
#endif

    virtual void mf2 () override
    {
        std::cout << "Derived::mf2()" << std::endl;
    }
};

int main ()
{
    Derived d;
    Base* b = &d;
    std::cout << "Redefine a non-virtual function mf1() in derived class which may make mistakes. Also is statically bound! " << std::endl;
    b->mf1 ();
    d.mf1 ();
    Derived* d1 = std::addressof ( d );
    d1->mf1 ();

    std::cout << "Dynamically bound" << std::endl;
    d.mf2 ();
    b->mf2 ();
    d1->mf2 ();

    std::cout << "Statically bound" << std::endl;
    //Object sliding
    Base b1 = d;
    b1.mf2 ();

    return 0;
}