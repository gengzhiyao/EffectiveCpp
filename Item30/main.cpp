#include <iostream>

/**
 * !透彻了解inlining的里里外外
 * inlining函数看似能够将函数本体嵌入到代码中以减少函数调用的成本，但实际上，可能并非如此
 * 问题：
 * 1. 多处调用同一个inline函数，所导致的代码膨胀问题，可能降低CPU缓存命中率，本意想优化函数调用的成本，但招致来的可能是更低的效率
 * 2. inline函数通常被定义在头文件内，与此相似的是模板（尤其是函数模板）同样被定义在头文件内，这时候可能会将function template声明为inline 但实际上，需要考虑，模板实例化时，是否所有的模板都需要进行 inline ?
 * 3. inline是申请，并且强制inline，**对所有虚函数进行inline**都是无效的！因为inline发生在编译期，而虚函数的调用发生在运行期
 * 4. 有时候编译器可能接受inline的同时还会生成一个函数调用？为什么？ => 示例中的函数指针 需要拿到函数的地址，这时候编译期需要生成inline函数的outline版本
 * 5. 构造函数和析构函数不要进行inline 因为在继承关系中，子类的构造函数inline往往伴随着将父类的构造函数进行inline，如此一来，父类中的成员变量在编译器的动作下，可能所有的成员变量都进行inline
 * 6. 在动态库的设计中，所有使用到头文件中inline函数的地方，inline代码都被嵌入到了可执行文件中，这时候，如果对inline函数进行了改动，只替换动态库，原来的inline嵌入代码是没有改变的，招致来的是莫名其妙的错误.
 */

inline int foo ( int a, int b )
{
    return a + b;
}

int ( *pf ) ( int a, int b ) = nullptr; //function pointer

int main ()
{
    pf = foo;   // generate outlined function
    return 0;
}