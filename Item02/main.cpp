

/**
 * !尽量以const enum inline 替换 #define
 * 以编译器替换预处理器  #define宏定义并不视作语言的一部分
 * #define MAX 256  这个符号是不会被编译器看见的 也正因如此 不会被纳入编译器的符号表中
 * 另一个值得注意的是 class 内部的专属常量static const int member = 20; 这是一个声明式 而非定义式！ C++ 特殊规则
 * Notion: 如果你要对这个静态成员变量取地址 那么需要在源文件中提供一个定义式
 * 有些编译器不支持在类中直接为声明的静态成员变量赋值 这时候需要将声明和定义分离，在源文件中定义赋值
 * 解决办法：
 * 1. const int max = 256;  优点：1. 符号被纳入符号表，方便调试 2. 符号被放在.data/.bss/.rodata段，不像宏定义，在任意处都是一个数值，造成代码膨胀
 *      Notion:定义字符串时 使用const char* const name = "myname"  这两个const配合才能表达#define的意思 更一般地： 对于指针类型，都需要顶层const和底层const配合以表达正确含义
 * 2. 针对编译器不支持静态成员变量类内赋值的情况，提出enum hack，在类中使用enum充当static
 * 3. 对于宏函数，使用inline函数进行替换
 */

class GamePlayer
{
private:
    enum
    {
        num = 5,
    };

    int scores[num];    //enum hack 是模板元编程中常用的技法
};

/**
 * *宏函数带来的危害
 * #define CALL_WITH_MAX(a,b) f((a)>(b)?(a):(b))
 * int a = 5, b = 0;
 * CALL_WITH_MAX ( ++a, b );   // a 会被累加两次
 * CALL_WITH_MAX ( ++a, b + 10 );  // a 会被累加一次
 */
