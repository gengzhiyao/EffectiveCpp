#include <iostream>

/**
 * !以非成员函数和非友元函数替换成员函数
 * 本条款主要说明的还是封装性带来的便利
 */

namespace Encapsulation {
    /**
     * 在此命名空间下，讨论为什么使用非成员函数/非友元函数能够带来更强的封装性和可拓展性
     * Notion: 本条款适用于不使用类内的私有成员变量的情况下，这种函数一般视作是功能性的函数，并不访问类的私有成员变量
     * 考虑 ClearEverything() 和 ClearBrowser(WebBrowser& wb) 试问：哪种方式提供的封装性和可拓展性好？
     * [WARN]:答案是 ClearBrowser(WebBrowser& wb)
     * 为什么？
     * OOP 设计原则告诉我们，数据以及操作数据的那些函数应该被绑定在一块，这好像说明成员函数是较好的选择
     * 但实际上， ClearEverything() 比 ClearBrowser(WebBrowser& wb) 带来的封装性低！
     * 在对象内，越少的代码可以访问数据，才意味着越强的封装性，我们改变代码的机会也就越多
     * ClearBrowser(WebBrowser& wb) 这个函数能够访问的数据相比而言更少，从而带来了更强的封装性
     * 另外，让 ClearBrowser(WebBrowser& wb) 不作为其成员函数不意味着非是普通函数，它也可以作为其他类的静态成员函数如工具类：Utility::ClearBrowser
     * Notion: 值得强调的一点是，这里我们使用了命名空间，并将 ClearBrowser 置于该命名空间中，那么优点是什么？   **解耦**
     * 命名空间可以跨编译单元出现，但是类却不行。假如我们的项目中含有多种工具类，如果作为类的成员函数，需要该工具类的其他类需要包含我们一整个头文件
     * 但是如果以非成员非友元的形式出现，就可以将多个非成员非友元函数定义在不同的头文件中，当其他类需要时，仅仅包含一个执行特定功能的头文件就可以了
     * 这正是 C++ 标准库的组织方式，标准库并不是一个<standardlibrary> 而是很多头文件并在每个头文件内声明 std 的某些功能
     * 这样做的话，也方便我们后续拓展软件的功能
     */

    class WebBrowser
    {
    public:
        // ...
        void ClearCache () { std::cout << "清除缓存" << std::endl; }
        void ClearHistory () { std::cout << "清除浏览历史" << std::endl; }
        void RemoveCookies () { std::cout << "清除cookies" << std::endl; }

    public:
        // Convenience features
        void ClearEverything ()
        {
            std::cout << "便利成员函数：清除所有" << std::endl;
            ClearCache ();
            ClearHistory ();
            RemoveCookies ();
        }
    };

    void ClearBrowser ( WebBrowser& wb )
    {
        std::cout << "便利非成员非友元函数：清除所有" << std::endl;
        wb.ClearCache ();
        wb.ClearHistory ();
        wb.RemoveCookies ();
    }

    class Utility
    {
    public:
        static void ClearBrowser ( WebBrowser& wb )
        {
            std::cout << "其他类的静态成员函数：清除所有" << std::endl;
            wb.ClearCache ();
            wb.ClearHistory ();
            wb.RemoveCookies ();
        }
    };

}

int main ()
{

    return 0;
}