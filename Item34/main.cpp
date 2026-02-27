#include <iostream>

/**
 * !区分接口继承和实现继承
 * 1. 接口继承：声明一个纯虚函数的目的是为了让派生类只继承函数接口
 * 2. 接口继承可重写：声明普通的虚函数的目的是：让派生类继承该接口，并且提供了默认实现
 * 3. 实现继承：不变形凌驾于特异性，继承接口并给出一份强制实现
 * Notion： 既不能够全为虚函数，也不能够全为普通成员函数
 */

#pragma region Defination
class Base
{
public:
    virtual void function1 () = 0;  // 接口继承
    virtual void function2 ();      // 实现继承可重写
    void function3 ();              // 实现继承
};

class Derived :public Base
{

};
#pragma endregion

#pragma region Sample

class Airport
{

};

class Airplane
{
public:
    #if false
    virtual void fly ( const Airport& destination )
    {
        // 假设该函数决定飞行方式
        /**
         * 缺点：如果将来某个函数的飞行方式不再依赖于默认的飞行方式，这样的虚函数又没有在编译时强制重写
         * 那么可能会造成灾难
         * 这种方式是实现继承
         * 接口继承要求子类必须重写分类中对应的函数接口
         */
    }
    #endif

    virtual void fly ( const Airport& destination ) = 0;
};

void Airplane::fly ( const Airport& destination )
{
    std::cout << "纯虚函数的默认实现" << std::endl;
}

class ModelA :public Airplane
{
public:
    virtual void fly ( const Airport& destination )
    {
        Airplane::fly ( destination );  // 调用纯虚函数的默认实现
    }
};

class ModelC :public Airplane
{
public:
    // 如果 fly 并非是一个纯虚函数，那么 ModelC 在未说明需要的情况下，就将 fly() 方法继承过来，造成的结果是不好的
    /**
     * !较为妥当的做法：
     * 1. Airplane 类中提供一个普通的成员函数，代表默认的飞行行为；飞行方法设置为纯虚函数，子类中必须重写该函数，对于默认行为，子类调用父类的默认实现
     * 2. 给纯虚函数一个默认实现，并且在子类中调用其默认实现
     */
};

#pragma endregion


int main ()
{
    Airport airport;
    ModelA* a = new ModelA;
    // a->Airplane::fly (airport); // 调用纯虚函数的默认实现
    a->fly ( airport );

    return 0;
}