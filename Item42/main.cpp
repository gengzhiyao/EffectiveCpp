#include <iostream>

/**
 * !了解 typename 的双重意义
 * 声明模板参数 / 声明嵌套从属类型
 * typename 和 class 在声明模板参数时，没有任何区别，这里不必多讲
 * 什么是嵌套从属类型？
 * template 内出现的参数如果依赖于某个*模板参数*，那么这个参数就是从属名称
 * 如果嵌套从属名称表明的是类型，那就是嵌套从属类型名称
 * 嵌套从属类型名称会导致解析困难
 * T::const_iterator* x; 既可以表示为 x 是一个 const_iterator*类型
 * ，也可以表示声明了T::const_iterator这个值，导致x什么也不是，更可以表示为T中静态成员变量
 * const_iterator 和 x 的乘积 C++
 * 编译器在解析时，总是认为T::const_iterator不是一个类型，而是一个值，除非你明确告诉他这是一个类型
 * !任何时候，当你想要在 template 中使用嵌套从属类型名称时，都必须使用 typename
 * 关键字来指定这是一个类型 typename
 * 只能够使用在嵌套从属名称中，在其他情况下不应该存在 typename 关键字 
 * Notion: 例外：
 * 不允许出现在继承列表中以及构造函数初始化列表中
 */

template <typename T>
void print2nd(
    const T& container)   // container 是从属名称，因为它依赖于 T 这个模板参数
{
    if (container.size() >= 2)
    {
        /*typename*/ T::const_iterator it =
            container.begin();   // it的类型是 T::const_iterator，其中
                               // const_iterator 是什么必须取决于 template
                               // 参数，这是嵌套从属名称/ 嵌套从属类型名称
        // std::advance(it, 1);
        ++it;
        int value = *it; // value 是非从属名称，因为它不依赖于 T 这个模板参数
        std::cout << value << std::endl;
    }
};

// 例外：
template <typename T>
class Derived : public Base<T>::Nested
{
public:
    explicit Derived(int x) 
    : Base<T>::Nested(x)
    {
        typename Base<T>::Nested temp(x);   // 既不再继承列表中，也不再构造函数初始化列表中，所以必须使用 typename 关键字
    }
};

// 另外的标准库中的一些例子
template<typename T>
void workWithIterator(T iter)
{
    typename std::iterator_traits<T>::value_type temp = *iter;
    // 抑或 typedef typename std::iterator_traits<T>::value_type value_type; value_type temp = *iter;
}

int main()
{
    return 0;
}