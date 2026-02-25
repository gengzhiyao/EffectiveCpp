#include <iostream>
#include <memory>
/**
 * !让接口容易被正确使用，不容易被误用
 * Notion：本条款的重点在于接口的设计理念：利用类型安全系统
 * 1. 设计函数形参：尽量以enmu class/class 替换 int 类型，值得注意的是，旧式的 enum 仍然是类型不安全的，因为 enum 可以被当做 int 被使用
 * 2. 规定函数接口的作用：返回值类型/接口类型 考虑使用 const 进行修饰
 * 3. 设计成员函数的接口：尽量使类的接口和 STL 的接口形式一致：如大小使用size()
 * 4. 为避免内存泄露，函数的返回值尽量返回一个智能指针
 *      Optional：针对智能指针的特殊情况：返回一个指定的 deleter
 */

class Month
{
public:
    static Month Jan () { return Month ( 1 ); } // non-local static 对象的初始化次序问题，使用函数进行替代，见条款4
    //... 
private:
    explicit Month ( int month );
};

class Investment {/*...*/ };

void GetRidOfInvestment ( Investment* p )
{
    std::cout << "这里属于指定的删除器" << std::endl;
    // 执行一些清理操作
    delete p;
}

std::shared_ptr<Investment> CreateInvestment ()
{
    std::shared_ptr<Investment> ret ( static_cast< Investment* >( 0 ), GetRidOfInvestment );
    // 令 ret 创建一个对象
    ret = std::make_shared<Investment> ();
    return ret;
}

int main ()
{
    return 0;
}