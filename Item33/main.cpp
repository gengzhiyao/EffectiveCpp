#include <iostream>

/**
 * 基础：
 * Notion:名称隐藏和ADL查找     **C语言中并没有ADL**
 * 名称查找的阶段：
 * 源码(.cpp) → 编译期前端（语法分析/语义分析/名称查找/重载决议）→ 中间代码 → 编译期后端（名称修饰/优化）→ 目标文件(.o) → 链接器（根据修饰后的符号名解析引用）→ 可执行文件
 * 名称查找发生在名称修饰之前!      名称查找只发生在当前的编译单元
 * C++ 的查找顺序为：
 * 1. 普通查找：按照作用域规则：当前作用域 → 外层 → 全局 一旦找到名字，直接停止继续查找外层，这是 C++ 的硬性规则，和ADL无关
 * 2. 普通查找搜集到的函数集和ADL查找找到的函数集一起参与函数重载决议 ADL是补充候选，不决定查找范围
 * 3. 只要不是成员函数调用和显示函数调用，ADL就不会被抑制，ADL一定会发生
 *      显式调用    ::f(a)
 *      成员调用    obj.f(a)
 * 名称查找 = 作用域查找(包含using引入)+ADL   <=>    using 只影响普通查找，不影响ADL
 * Notion:ADL 主要解决的问题是：当函数和其参数类型位于不同命名空间时，简化函数的调用
 */

 /**
  * !避免隐藏继承下来的名称
  *
  */

namespace N {
    class A { };
    void f ( A ) { std::cout << "N::f" << std::endl; }
}

void f ( int ) { std::cout << "全局::f(int)" << std::endl; }

namespace Inner {
    void f ( double ) { std::cout << "内层::f(double)" << std::endl; }
}

/**
 * WithoutUsing 注释掉 using Inner::f
 * 查找顺序：
 * 1. 普通查找：找到::f(int)
 * 2. ADL：找到N::f(A)
 * 重载决议：优先匹配 N::f(A) 输出：N::f
 */
int WithoutUsing ()
{
    N::A a;

    f ( a );
    return 0;
}

/**
 * WithUsing 引入Inner::f(double)
 * 查找顺序：
 * 1. 普通查找：Inner::f(double)，内层作用域找到，停止外层查找，隐藏::f(int)
 * 2. ADL：N::f(A)
 * 重载决议：
 * 匹配N::f(A)
 */
int WithUsing ()
{
    N::A a;
    using Inner::f;
    f ( a );
    return 0;
}

int main ()
{
    WithoutUsing ();
    return 0;
}