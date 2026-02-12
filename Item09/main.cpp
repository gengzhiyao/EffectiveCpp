#include <iostream>

/**
 * !绝不要在构造函数和析构函数中调用虚函数
 * 首先说现象：在构造函数中调用虚函数，子类中重写的虚函数总是不被调用的，并且随着继承体系的增加，若每个派生类的构造函数中都调用虚函数，那么这个函数会被当做普通函数，被调用N-1次
 * 那么为什么?
 * 在继承体系中，构造函数的调用一定是首先构造基类的构造函数，才调用子类的构造函数，在父类构造期间，子类的那一部分还没有构造，虚函数表还没有建立起来，在构造函数/析构函数中调用虚函数，都是被当做普通函数对待的
 * 即便是你不去调用虚函数，但是你的普通函数中可能调用虚函数，并且在构造和析构过程中调用这个普通函数，这仍然是错误的做法
 */

#define DESCENDANTS 0   //定义该宏为1，继承体系中有三代

class Transaction
{
public:
    Transaction ();
    virtual void LogTransaction () const = 0;
};

void Transaction::LogTransaction () const
{
    std::cout << "Base=>Transaction::LogTransaction ()" << std::endl;
}

Transaction::Transaction ()
{
    LogTransaction ();  //warning: pure virtual 'virtual void Transaction::LogTransaction() const' called from constructor
}

class BuyTransaction :public Transaction
{
public:
    virtual void LogTransaction ()  const override
    {
        std::cout << "Derived=>BuyTransaction::LogTransaction ()" << std::endl;
    }

public:
#if DESCENDANTS
    BuyTransaction ()
    {
        LogTransaction ();
    }
#endif
};

class SellTransaction :public Transaction
{
public:
    virtual void LogTransaction () const override
    {
        std::cout << "Derived=>SellTransaction::LogTransaction ()" << std::endl;
    }
};

#if DESCENDANTS
class Descendants final : public BuyTransaction
{
    virtual void LogTransaction ()  const override
    {
        std::cout << "Derived=>Descendants::LogTransaction ()" << std::endl;
    }
};
#endif

int main ()
{
    BuyTransaction b;
#if DESCENDANTS
    Descendants d;  //[WARN]: 父类的和爷爷类的虚函数都被调用了，但就是自己的没有被调用
#endif
    return 0;
}

