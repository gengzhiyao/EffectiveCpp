#include <iostream>
#include <vector>
/**
 * !考虑写出一个不抛异常的 swap 函数
 * Notion: swap 函数在标准库中的典型实现
 * namespace std
 * {
 *     template<typename T>
 *     void swap ( T& a, T& b )
 *     {
 *         T temp ( a );
 *         a = b;
 *         b = temp;
 *    }
 * }
 * 为什么需要写出自定义的 swap 函数呢？
 * 1. 在拷贝赋值运算符中可以提供强烈的异常安全保证，同时还可以简化移动操作
 * 2. 在C++11以前，标准库中给出的 swap 函数由于其通用性，并不能够保证数据高效传递  =>  尤其是面对 pimpl idiom 时，operator= 可能会交换 Impl 中的数据，但实际上，我们只需要置换指针成员
 * Notion: 我们现在的场景是：现在正在调用 swap 函数用于置换两个对象的值
 * 在 STL 中，我们可以看到三行代码中存在调用 operator=，其实现为：Widget& operator=( const Widget& rhs )，在其中进行了大量的数据交换，导致性能低效
 * !为了提高效率，我们真正想做的是在swap中仅仅交换两个pImpl指针的值即可，以下是我们所做的尝试：
 * 1. 为 std::swap 添加特化版本用以交换两个 pImpl 的值
 *      但是由于 pImpl 是私有成员，我们无法直接使用
 * 2. 故：我们将 swap 函数作为成员函数实现在类中
 *      但是 swap 成员函数的调用和普通函数的调用方式不一致，我们需要的形式是 swap ( T& a, T& b )
 * 3. 因此，在该类所在的命名空间中，提供一个 普通版 的 swap 函数，该函数仅仅是为了调用成员函数版本的 a.swap(b)
 * 4. 另外，在调用 swap 时，使用using std::swap
 *      在编写和调用 swap 函数时，应该位于类或模板所在的命名空间，首先保证 ADL 能够找到对应的 swap，找不到时，using 声明式提供 STL 中的 swap 作为兜底方案
 */

class WidgetImpl
{
public:
    //function
private:
    //data
};

class Widget
{
public:
    Widget ()
        :pImpl ( new WidgetImpl )
    {

    }
    Widget ( const Widget& rhs );
    Widget& operator=( const Widget& rhs )
    {
        if ( this == &rhs )  return *this;
        *pImpl = *rhs.pImpl;    // 拷贝对象内部的所有数据...
        //... 删除旧数据
        return *this;
    }

    // [WARN] 第一次看先省略此处代码
    void swap ( Widget& other ) noexcept
    {
        std::cout << "swap 成员函数被调用" << std::endl;
        using std::swap;
        swap ( pImpl, other.pImpl );
    }

private:
    WidgetImpl* pImpl;
};

namespace std {
    template<>
    void swap<Widget> ( Widget& a, Widget& b )
    {
        // swap ( a.pImpl, b.pImpl );   //[ERROR]: 无法通过编译，企图访问私有成员pImpl
        a.swap ( b );
    }
}

namespace Sample {
    // 假定 Widget 和 WidgetImpl 都是类模板而非类的情况下
    template<typename T>
    class WidgetImpl
    {
        //...
    };

    template<typename T>
    class Widget
    {
        //...
    };

    namespace std       //![ERROR] 不合法的代码
    {
        // BUG 1. 企图偏特化函数模板
        // template<typename T>
        // void swap<Widget<T>> ( Widget<T>& a, Widget<T>& b )
        // {
        //     a.swap ( b );
        // }

        // BUG 2. 企图为std空间中的函数添加重载版本
        // template<typename T>
        // void swap ( Widget<T>& a, Widget<T>& b )
        // {
        //     a.swap ( b );
        // }
    }
} // namespace Sample

void swap ( Widget& a, Widget& b )
{
    std::cout << "全局的swap函数被调用：作用是 => 调用成员函数" << std::endl;
    a.swap ( b );
}

int main ()
{
    Widget a, b;
    swap ( a, b );
    return 0;
}