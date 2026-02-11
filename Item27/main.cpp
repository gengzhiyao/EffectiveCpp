#include <iostream>

/**
 * !尽量少做转型动作
 * C 风格的历史遗留问题：转型动作分两种：1. (T) expression  2. T(expression) 其中2实际上表示的是构造函数如：std::string str("Hello");
 * C++ 中提供了四种转型方案：
 * 1. const_cast            // 用于将对象的const属性移除
 * 2. dynamic_cast          // 安全向下转型
 * 3. reinterpret_cast      // 底层转型，结果取决于编译器实现，不具备ABI兼容性，不可移植
 * 4. static_cast           // 强迫转型，常用
 * 书中讲到唯一使用旧式风格转换的只有在explicit构造函数中 explicit Widget(/ *an Arg* /) 需要提前构造好唯一的形参传入
 * 转型动作不要涉及到编译器的实现：
 * 例如 C++ 对象模型依靠编译器实现，在以下例子中：
 * Notion:Derived d; Base* b = &d; 你不能够将对象的地址转为char* 之后进行任何操作，这是因为&d和b可能并不相同，因为会存在一个偏移量，涉及到this指针的调整
 * 另外就是例子Window与SpecialWindow对转型动作的参考
 * 最后：Scott Meyers 在P121中强调要在容器中存储派生类的指针而不是基类的指针，其含义是：代码中明明只有派生类，但是还是存储派生类指针并且进行逐个转型，这是非常糟糕的设计；容器里永远只会是 Derived，就直接写std::vector<Derived*>
 * 如果容器中是多态集合，那个还是必须写成基类的指针进行多态调用
 * 为什么是这种意思？ 从P121中的for循环就能看出来，所有的元素都转换为了SpecialWindow类型，但是容器中却存储基类的指针，那么这就是自寻麻烦
 */

#define USE_CAST_TO_CALL_BASE_OnResize 0    //Notion: 在一般的框架中，为什么调用Window::OnResize()而不是static_cast<Window>(*this).OnResize() ???
 /**
  * *if USE_CAST_TO_CALL_BASE_OnResize = 1  //static_cast< Window >( *this ).OnResize ();
  * Window::Constructor
  * SpecialWindow::Constructor
  * Window::Copy Constructor
  * Window::OnResize
  * *if USE_CAST_TO_CALL_BASE_OnResize = 0  //Window::OnResize()
  * Window::Constructor
  * SpecialWindow::Constructor
  * !Window::Copy Constructor
  * Window::OnResize
  */

class Window
{
public:
    Window (/*parent ...arg*/ )
    {
        /*=default*/
        std::cout << "Window::Constructor" << std::endl;
    }
    Window ( const Window& lhs )
    {
        std::cout << "Window::Copy Constructor" << std::endl;
    }
    virtual void OnResize ()
    {
        std::cout << "Window::OnResize" << std::endl;
    }
    virtual ~Window () = default;
};

class SpecialWindow :public Window
{
public:
    SpecialWindow (/*parent*/ )
        :Window (/*parent*/ )
    {
        std::cout << "SpecialWindow::Constructor" << std::endl;
    }

    SpecialWindow ( const SpecialWindow& lhs )
    {
        std::cout << "SpecialWindow::Copy Constructor" << std::endl;
    }

    virtual void OnResize () override
    {
    #if USE_CAST_TO_CALL_BASE_OnResize
        // 所以：这个转型并不是调用的当前对象的OnResize，而是由于转型动作产生了一个临时对象，我们对这个临时对象调用了OnResize，虽然成员函数在内存中只有一份，但是这个成员函数操作的数据却是那个临时对象副本的数据
        static_cast< Window >( *this ).OnResize ();     //!类型转换中：转换为值类型，****一定会产生新对象**** 产生拷贝构造/临时对象
    #else
        Window::OnResize ();    //解决办法：拿掉转型动作 不要将*this视作一个Base对象，而是调用Base::OnResize()并作用于当前对象身上。
    #endif
        // DoSomething
        // 经由static_cast< Window >( *this ).OnResize ();转换后，我们在临时对象身上操作了数据，但是对象本身的Base成分并没有改变，故现在造成对象变成了一种“伤残”的状态：对对象的基类部分的修改没有落实，但是对Derived部分的更改落实了
    }
};

int main ()
{
    SpecialWindow spciWnd;
    spciWnd.OnResize ();
}