#include <iostream>

/**
 *
 * @brief 明智而审慎的使用 private 继承
 * @details
 * 1. 在private
 * 继承关系下，编译器不会将子类对象解释为父类对象，也就意味着：private继承并不是
 * is-a 关系
 * 2. private 继承意味着一种根据某物实现出的关系，private
 * 继承纯粹是一种实现技术，使用private 继承的目的是为了采用 Base
 * 中已经备妥的某些特性，而 派生类和基类不存在任何观念上的关系
 * 3. private 继承关系下，子类仍然可以 override
 * 父类中的虚函数，只是访问权限被限制，在外界不能够被访问，但是在子类中仍然可以被调用
 * 通过条款38已知，复合也是一种根据某物实现出的关系，因此，如何在两者之间取舍？
 * Notion: 尽可能使用复合，而不是 private 继承
 * !总结： private 继承应该只用在当一个派生类想要使用父类的protected成员，或为了使用 virtual 函数作为自己的工具时
 */

class Person
{
public:
    virtual void sayHello()
    {
        std::cout << "Hello, I am a Person" << std::endl;
    }
};

class Student : private Person
{
public:
    void Test()
    {
        Person* p = this;   // 类内部可以实现多态
        p->sayHello();  // 调用子类的 sayHello() 函数
        // ! 但其实这种调用方式是多此一举的
    }
    void sayHello() override
    {
        std::cout << "Hello, I am a Student" << std::endl;
    }
};

// 假设这是一个第三方库中的不可修改的工具类
class Timer
{
public:
    explicit Timer(int seconds) {}
    virtual void OnTick()
    {
        std::cout << "假设这是定时器的 OnTick() 回调函数" << std::endl;
    }
};

// 我们需要使用 Timer 类，但是我们不能修改 Timer 类的代码
// 要求是：
// 重新定义 Timer 类的 OnTick() 函数，用于定义自己的行为逻辑
// 但是我们自己的类如何public 继承 Timer 类，就失去了 is-a 关系的意义，所以使用public继承是不合适的
// 另外，使用public继承可能导致用户以为他们可以调用该函数，这样就破坏了封装性
class Widget : private Timer
{
    private:
    void OnTick() override
    {
        std::cout << "假设这是 Widget 类需要重写的 OnTick() 回调函数" << std::endl;
    }
};

// 更好的设计方案是：使用复合而不是 private 继承
class GoodWidget
{
    private:
    class WidgetTimer:public Timer
    {
        public:
        virtual void OnTick() override
        {
            std::cout << "这是私有 WidgetTimer 类重写的 OnTick() 回调函数" << std::endl;
        }
    };

    WidgetTimer m_privateTimer;
};

// 为什么使用复合而不是 private 继承？
// 1. 首先是为继承而考虑：在 private 继承关系下，恐怕用户能够重新定义 OnTick() 函数，这样改变了我们需要的行为规则；但这种封装在现代 C++ 已经可以使用 final 来约束
//    但是如何使用以上 GoodWidget 的设计方案，这种组合特征就能够使用户无法重新定义 OnTick() 函数，从而保持了封装性
// 2. 将编译依赖关系降低
//    如果使用继承，那么Wigets在继承 Timer 时，Timer 类的定义式必须已经出现，也就是说，我们不得不 include Timer 类的头文件
//    但是如果使用复合呢？我们可以将 Widget 类的成员 m_privateTimer 声明为 WidgetTimer* ，并将 GoodWidget::WidgetTimer 的定义移动到源文件中，以实现 Pimpl 用法，降低编译关系。

// Notion: 有一种情况需要使用 private 继承：非常非常极端的情况
// 我们知道的是，对于一个空类，它作为一个成员变量存在时，是需要占用一个字节的内存空间的，如果将这个空类作为一个类的成员，还会涉及到字节对齐的问题，导致这个类膨胀
// 但是如果使用 private 继承，就可以避免这个问题，这就是 EBO 优化
// 从现实层面讲，空类并不是真的空，而是有一些 non-virtual 函数，typedefs/ enums / static 成员变量等。
// 但实际上很少有 空类存在，因此 EBO 被使用的情况就少多了，一般情况下，使用复合也就变得更加正当了

int main()
{
    Student stu;
    stu.Test();
    // stu.sayHello();
    return 0;
}