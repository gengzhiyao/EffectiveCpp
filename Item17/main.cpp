#include <memory>
#include <iostream>
/**
 * !以独立语句将newed对象置入智能指针
 * Notion: 揭示的是：异常引发的内存泄漏问题
 * 语言层面并没有规定参数的求值顺序，在资源被创建（如：new Widget）和资源被转换为资源管理对象 两个时间点之间有可能发生异常干扰的情况下，就存在潜在的内存泄漏问题
 * 为什么可以使用独立语句避免资源泄露：
 * *编译器对跨语句的操作不能进行重新排列，但是对于同一语句内的表达式，求值顺序并没有进行规定
 */

int Priority()
{
    std::cout << "返回值代表程序处理的优先级" << std::endl;
    return 1; // priorityLevel
};

class Widget
{

};

void Process ( std::shared_ptr<Widget> pw, int priority )
{
    std::cout << "根据优先级处理事件" << std::endl;
}


int main ()
{
    // Process ( new Widget, Priority::priorityLevel ); // shared_ptr 的构造函数是 explicit 的，不能直接将裸指针转换为智能指针
    Process ( std::shared_ptr<Widget> ( new Widget ), Priority () );
    /**
     * !编译器在产出对Process的调用码之前，必须首先得到各个实参
     * Notion: 调用约定决定的是参数的参数如何传入寄存器以及哪一方负责清理栈帧，不决定参数的求值顺序
     * 以上代码中，可能的求值顺序是：
     * 1. new Widget
     * 2. Priority()
     * 3. Ctor shared_ptr
     * 一旦 Priority() 的调用抛出异常，堆区的 Widget 对象没有被智能指针接管，就泄露了
     */
    return 0;
}