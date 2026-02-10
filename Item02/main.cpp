

/**
 * !尽量以const enum inline 替换 #define
 * 以编译器替换预处理器  #define宏定义并不视作语言的一部分
 * #define MAX 256  这个符号是不会被编译器看见的 也正因如此 不会被纳入编译器的符号表中
 * 
 * 解决办法：
 * 1. const int max = 256;  优点：1. 符号被纳入符号表，方便调试 2. 符号被放在.data/.bss/.rodata段，不像宏定义，在任意处都是一个数值，造成代码膨胀
 *      Notion:定义字符串时 使用const char* const name = "myname"  这两个const配合才能表达#define的意思 更一般地： 对于指针类型，都需要顶层const和底层const配合以表达正确含义
 *
 */