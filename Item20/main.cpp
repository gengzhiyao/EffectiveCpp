#include <iostream>

/**
 * !以pass-by-reference-to-const替换pass-by-value
 * 默认情况下，C++ 以值传递的方式传递对象至函数的形参/ 函数的返回值，这些参数都是以实际实参的拷贝为初值的，并且调用端所获得的也是对象的拷贝
 * 尤其是对于自定义的类型，对象的拷贝不意味着仅仅发生一次本对象的构造和析构，同样伴随着的是类内部数据成员的拷贝构造和析构
 * 如果对象中还包含了继承关系，随之而来的还有子类对象的父类部分的构造/拷贝构造和析构
 * 另外，对于继承关系谱中，pass-by-reference不仅能够避免低效的问题，同时还能够避免对象的切片
 * Notion: 对于内置类型来说，值传递的方式比引用传递的方式更加高效；另外，对于 STL 中的迭代器和函数对象，同样传值比传引用高效
 * 即便是用户自定义的小 types，也并不意味着是值传递方式的合格候选人，因为一些编译器会将内置类型放入缓存器内，但自定义类型不会
 */

#pragma region Person
class Person
{
public:
    virtual ~Person () = default;

private:
    std::string name;
    std::string address;
};

class Student :public Person
{
private:
    // Data...
};

// bool Validate ( Student stu );
bool Validate ( const Student& stu );   //pass-by-reference-to-const
#pragma endregion

#pragma region Window
class Window
{
public:
    std::string name () const;  // 返回窗口名称
    virtual void display () const;  // 显示窗口
private:
    // data
};

class ScrolledWindow :public Window
{
public:
    virtual void display () const override;
};

void PrintWindowNameAndDisplay ( Window wnd )
{
    /**
     * 派生类对象以值传递的方式传递给基类的对象，造成的是：基类的构造函数被调用，但是属于派生类对象的那一部分信息丢失了
     */
    std::cout << wnd.name () << std::endl;
    wnd.display (); // 无论传递的是哪个派生类对象，都只会调用 Window::display()
    // 改变方式：pass-by-reference-to-const
}
#pragma endregion


int main ()
{
    return 0;
}