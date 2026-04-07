#include <iostream>
#include <new>

/**
 * !了解 new_handler 的行为
 * Notion： 在 new 之前，必须先 set_new_handler，否则 new 会抛 std::bad_alloc 异常
 * 即便是类的 new_handler，在设置时也只能设置全局函数，不能设置类的成员函数（隐含this指针）或静态成员函数
 * 如果 new-handler 什么都不做，会无限循环，new_handler 要么释放内存，要么调用 std::abort() 退出程序，要么抛出异常
 */

// 资源管理类
class NewHandlerHolder
{
public:
    explicit NewHandlerHolder(std::new_handler old_handler) 
        : m_oldHandler(old_handler)
    {
    }

    ~NewHandlerHolder()
    {
        std::set_new_handler(m_oldHandler); // 析构时恢复旧的new_handler，抛弃当前无论成功与否的new_handler
    }

    NewHandlerHolder(const NewHandlerHolder& rhs) = delete;
    NewHandlerHolder& operator=(const NewHandlerHolder& rhs) = delete;

private:
    std::new_handler m_oldHandler;  // 异常安全地恢复全局new_handler
};

// Widget::operator new 被调用 -> 将全局的 handler 替换为 Widget 的专属 handler 
// -> 调用 ::operator new(size) -> 若失败，调用 Widget::专属的 handler
// Widget::operator new 退出时，不论成功与否，都恢复全局的 handler 
class Widget
{
public:
    static std::new_handler set_new_handler(std::new_handler handler)   // std::new_handler 的类型是 void(*)()，这里需要的是传入一个函数指针
    {
        std::new_handler oldHandler = m_currentHandler;
        m_currentHandler = handler;
        return oldHandler;
    }

    static void* operator new(std::size_t size) noexcept    // 函数中装入类专属的new_handler，函数退出时自动还原
    {
        NewHandlerHolder holder(std::set_new_handler(m_currentHandler));    // std::set_new_handler 返回旧的new_handler，这里将其保存到 holder 中
        return ::operator new(size);
    }

private:
    static std::new_handler m_currentHandler;
};

std::new_handler Widget::m_currentHandler =
    nullptr;   // 类的静态成员变量的定义，类内只是声明

template <typename T>
class NewHandlerSupport
{
public:
    static std::new_handler set_new_handler(std::new_handler handler) noexcept;
    static void*            operator new(std::size_t size) noexcept;

private:
    static std::new_handler m_currentHandler;
};

template <typename T>
std::new_handler NewHandlerSupport<T>::set_new_handler(
    std::new_handler handler) noexcept
{
    std::new_handler oldHandler = m_currentHandler;
    m_currentHandler = handler;
    return oldHandler;
}

template <typename T>
void* NewHandlerSupport<T>::operator new(std::size_t size) noexcept
{
    NewHandlerHolder holder(std::set_new_handler(m_currentHandler));
    return ::operator new(size);
}

template <typename T>
std::new_handler NewHandlerSupport<T>::m_currentHandler = nullptr;

class MyWidget : public NewHandlerSupport<MyWidget>  // CRTP
{
    // 不必再次声明set_new_handler和operator new，因为它们已经从基类继承了
};

// NewHandlerSupport<MyWidget>::m_currentHandler;
// NewHandlerSupport<OtherWidget>::m_currentHandler;
// 两种不同的类，各自拥有独立的 m_currentHandler 静态成员变量，互不干扰。

int main()
{
    std::set_new_handler(
        []()
        {
            std::cout << "new-handler" << std::endl;
            std::abort();
        });
    // int* p = new int[922337203685477587];   // 32G 内存分配失败，调用 new_handler

    // Widget* p = new (std::nothrow) Widget;  // 常用形式，分配失败返回 nullptr，不抛异常
    // nothrow 保证的只是 operator new 本身不抛，但构造函数内部仍然可能抛(构造函数内调用new再次分配内存)。所以 nothrow new 并不提供完全的异常安全保证，实用性有限。
    return 0;
};
