#include <iostream>
#include <string>
/**
 * !明智而审慎的使用多重继承
 * 1. 多重继承带来的成员函数调用问题
 * 首先阐明C++编译器在继承中处理成员的方式
 * 在继承关系中，成员函数的调用规则是和重载函数的匹配规则是一致的，编译器会首先根据传入的参数类型进行匹配，找到最佳的匹配函数，然后再检验其是否符合访问权限
 * 2. 多重继承带来的菱形继承问题
 * 虚基类与虚继承
 * 虚基类是指在多重继承关系中，被共享的基类，它在编译时会被编译器自动优化，只保留一份实例
 * 虚继承指的是在继承关系中，使用virtual关键字来声明继承基类，确保只保留一份基类的实例
 * 从正确行为来看，好像多重继承中不可避免的会和菱形继承扯上一些关系；另外，又好像所有的多重继承问题都无可避免的会涉及到菱形继承
 * 但实际上并不是这样。
 * 针对虚继承时，成员变量的存储位置等待补充...
 * 3. 虚基类的成员变量的初始化问题
 * Notion:虚基类的初始化由最派生类负责，不能由其他派生类负责
 * 4. 多重继承一般性的使用场景
 */

#pragma region Member Function Call Issues in Multiple Inheritance
class BorrowableItem
{
public: //! public
    void CheckOut()
    {
        std::cout << "CheckOut() of BorrowableItem" << std::endl;
    }
};

class ElectronicGadget
{
private: //! private
    void CheckOut()
    {
        std::cout << "CheckOut() of ElectronicGadget" << std::endl;
    }
};

class MP3Player : public BorrowableItem, public ElectronicGadget
{
    // CheckOut() function
};
#pragma endregion

class File
{
    // Data Member
};

class InputFile : virtual public File   //! virtual public File
{
};

class OutputFile : virtual public File  //! virtual public File
{
};

class IOFile : public InputFile,
               public OutputFile  // Notion: 注意将virtual放在哪里
{
};

#pragma region Virtual Base Class Issues    // !虚基类的初始化问题
class Base
{
public:
    int data;
    Base(int val) : data(val)
    {
        std::cout << "Base 构造函数: data = " << data << "\n";
    }
};

class Derived1 : virtual public Base
{
public:
    Derived1() : Base(10) {}  // 错误：Derived1 不能初始化虚基类
};

class Derived2 : virtual public Base
{
public:
    Derived2() : Base(20) {}  // 错误：Derived2 不能初始化虚基类
};

class FinalClass : public Derived1, public Derived2
{
public:
    FinalClass() : Base(999) {}  // 正确：最派生类负责初始化虚基类
};
#pragma endregion

class IPerson
{
public:
    virtual ~IPerson() = default;
    virtual std::string name() const = 0;
    virtual std::string birthday() const = 0;
};
// 假设我们有一个CPerson类，表示一个具体的人，它继承IPerson类，那么他就需要实现IPerson类中的虚函数

// 但又假设我们现在有这么一个类，用于协助以各种形式打印数据库的字段
class PersonInfo
{
public:
    typedef int DataBaseID;

public:
    explicit PersonInfo(DataBaseID id);
    virtual ~PersonInfo() = default;
    virtual const char* theName() const;
    virtual const char* theBirthday() const
    {
        return "2000-01-01";
    }

private:
    virtual const char* valueDelimOpen() const
    {
        return "[";
    } // 字段值的开分隔符
    virtual const char* valueDelimClose() const
    {
        return "]";
    } // 字段值的闭分隔符
};

const char* PersonInfo::theName() const
{
    std::string open(valueDelimOpen());
    std::string close(valueDelimClose());
    return (open + std::string("Name") + close).c_str();
}

// 一般而言，我们应该在工作中，更好的利用组件，而不是自己实现，来完成我们的任务
// 假设你通过工厂函数返回出来的IPerson类要求，它的name()函数要求返回的是一个带大括号修饰的字符串
// 并假设PersonInfo类是一个通用的组件，那么我们应该如何使用它来完成我们的任务？

// Notion：
// PersonInfo中，刚好有若干函数能够帮助CPerson类进行实现，那么这里隐藏着Item38/39
// 中的**根据某物实现出**的一种逻辑：即 复合/私有继承
// 复合是一种更加推荐的方式，但是这里我们又需要重写虚函数，因此又涉及到public继承的问题

class CPerson : public IPerson, private PersonInfo
{
public:
    explicit CPerson(DataBaseID id) : PersonInfo(id) {}
    virtual std::string name() const override
    {
        return PersonInfo::theName();
    }
    virtual std::string birthday() const override
    {
        return PersonInfo::theBirthday();
    }

private:
    virtual const char* valueDelimOpen() const override
    {
        return "{";
    } // 字段值的开分隔符
    virtual const char* valueDelimClose() const override
    {
        return "}";
    } // 字段值的闭分隔符
};

int main()
{
    /**
     * 根据传入的参数类型进行匹配，找到最佳的匹配函数，然后再检验其是否符合访问权限，但是这里的两个CheckOut()函数都具有一致的匹配性，没有哪个更好
     * !所以说，在这里的调用，会报错，因为编译器无法确定调用的是哪个函数，即便在ElectronicGadget中CheckOut()函数是private的，在编译器进行匹配时，首先会产生调用时的二义性错误
     */
    MP3Player mp3Player;
    // mp3Player.CheckOut();   // 二义性
    mp3Player
        .BorrowableItem::CheckOut(); // 明确调用BorrowableItem::CheckOut()函数

    return 0;
}