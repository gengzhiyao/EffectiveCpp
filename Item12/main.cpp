#include <iostream>

/**
 * !复制对象时切记复制包含基类在内的每个成分
 * 在Item05中，我们说到，编译器会为我们生成一份拷贝构造函数和拷贝赋值运算符，他们有着正确的行为
 * 但是一旦当我们自己写一份拷贝行为的函数时，我们就需要注意了
 * Notion: 在继承体系中，编译器不会帮我们调用父类的拷贝行为函数
 * 1. 当我们完成了一个类的拷贝构造函数和拷贝赋值运算符后，假设有一天在类中添加了新的数据成员，那么：
 * !我们需要同时修改其特种成员函数，就像我们从未编写过这个类一样
 * 2. 在派生类中，派生类的拷贝构造函数和拷贝赋值运算符需要调用父类的拷贝构造函数和拷贝赋值运算符
 *  Notion: 假如我们忘记调用，编译器也不会报错提醒，甚至都没有任何的警告
 *  为什么呢？
 *  如果你忘记在子类的拷贝构造函数初始化列表中指定父类的拷贝构造函数，那么编译器会调用父类的默认无参的**构造函数**，将子类对象的父类那一部分执行默认构造
 *  这样造成的结果就是，子类对象自己的那一部分执行了拷贝构造，但是父类的那一部分执行了默认构造，致使对象属于一种半残缺的状态
 */

class Date { /*拷贝行为函数*/ };

void LogCall ( const std::string& funcName )
{
    std::cout << funcName << std::endl;
}

// 原本我们的这个类中，只有std::string name这个数据成员
class Customer
{
public:
    Customer () //小心，当你提供了默认构造函数时，即使你在子类中不调用父类的拷贝构造时，编译器也不会报错
    {
        LogCall ( "Customer Ctor" );
    }    
        Customer ( const Customer& rhs );
    Customer& operator=( const Customer& rhs );

private:
    std::string name;
    // 新加入一个成员
    Date date;
};

Customer::Customer ( const Customer& rhs )
    :name ( rhs.name )
    // 添加新的数据成员Data，那么我们需要同时修改copy ctor和copy assignment
    , date ( rhs.date )
{
    LogCall ( "Customer Copy Ctor" );
}

Customer& Customer::operator=( const Customer& rhs )
{
    LogCall ( "Customer Copy Assignment" );
    name = rhs.name;
    // add
    date = rhs.date;
    return *this;
}

class PriorityCustomer :public Customer
{
public:
    PriorityCustomer ( const int nPriority )
        :priority ( nPriority )
    {
        LogCall ( "PriorityCustomer Ctor" );
    }
    PriorityCustomer ( const PriorityCustomer& rhs );
    PriorityCustomer& operator=( const PriorityCustomer& rhs );

private:
    int priority;
};

PriorityCustomer::PriorityCustomer ( const PriorityCustomer& rhs )
    :priority ( rhs.priority )
    // 在继承体系中，切记要调用父类的拷贝构造函数和父类的拷贝赋值运算符
    ,Customer(rhs)
{
    LogCall ( "PriorityCustomer Copy Ctor" );
}

PriorityCustomer& PriorityCustomer::operator=( const PriorityCustomer& rhs )
{
    LogCall ( "PriorityCustomer Copy Assignment" );
    priority = rhs.priority;
    // add
    Customer::operator=( rhs );
    return *this;
}


int main ()
{
    PriorityCustomer a ( 5 );
    PriorityCustomer b ( a );
    PriorityCustomer c ( 1 );
    c = b;

    return 0;

}