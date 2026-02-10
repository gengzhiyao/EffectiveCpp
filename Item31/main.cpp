#include <iostream>
#include <string>
#include <memory>
/**
 * !将文件之间的编译依赖关系降至最低
 * C++ 并没有把 将接口与实现分离这件事做的很好 class的定义式通常不止定义了接口，还定义了这个类的实现细节，这些实现细节就代表以成员变量为基础的类的数据
 * 一旦某一个类的实现进行了改变（一般是头文件进行了改变），那么所有依赖该头文件的编译单元全都需要进行重新编译
 * 解决方法：
 * 1. Pimpl 降低编译依赖关系，同时隐藏类的实现细节
 * 2.以声明代替实现：在头文件中不需要包含某个类的头文件，转为使用前向声明；在源文件中包含依赖类的头文件用以能够调用类的成员方法；这样就是靠链接时重定位
 *      为什么在头文件中不需要包含某个类的头文件，那么万一成员函数中返回值或者函数参数需要这个类进行值传递呢？   =>    函数的返回值和函数参数并不需要一个类的完整实现，但是在调用时，类的完整实现必须暴露  region 3
 * 3.在尤其关注程序的性能问题而非耦合性时，考虑使用实现代替Handle class/Interface ，因为他们会带来一些小的函数跳转开销
 *
 * Handle class 中，转发的那部分函数是完全可以被 inline 的，因此从这方面来看，并不会付出太多函数调用的成本
 * 但是对于Interface class 来说不然，虚函数的调用是在运行时，inline没有作为
 */

#pragma region 1.Pimpl(Handle class)
 // Obj.h
class ObjImpl;  //Forward declare
class Obj
{
public:
    Obj ();
    void Function ();
private:
    ObjImpl* m_objImpl;
};

//Obj.cpp
class ObjImpl
{
public:
    ObjImpl ( /*Argument*/ );
    void DoFunction (); //Imply Function in Obj
private:
    //Notion: Data member in Obj    存放所有Obj类中的数据成员，隐藏所有的实现细节，同时头文件中只暴露接口，避免数据成员的改动影响文件之间的编译依赖关系，造成项目的重新编译
};

Obj::Obj ()
    :m_objImpl ( new ObjImpl (/* Data Initialization */ ) )
{

}

void Obj::Function ()
{
    m_objImpl->DoFunction ();   // 转发函数
}
#pragma endregion   // END REGION 1

#pragma region 2.Interface(Abstract class)

// person.h
class Person    //Abstract class
{
public:
    virtual ~Person () = default;
    virtual std::string name () const = 0;
    virtual int age () const = 0;
    virtual double salary () const = 0;

    static Person* create ( const std::string& name, const int age, const double salary );
};
// end person.h

// person.cpp
class RealPerson :public Person
{
public:
    RealPerson ( const std::string& name, const int age, const double salary );
public:
    std::string name () const override { return m_name; }
    int age () const override { return m_age; }
    double salary () const override { return m_salary; }
private:
    std::string m_name;
    int m_age;
    double m_salary;
};

// 简单工厂模式 其实耦合度还是比较高的 因为需要包含子类的头文件
// Effective C++ 3rd P147 : 一个更现实的create代码往往会根据参数的不同创建不同类型的子类
Person* Person::create ( const std::string& name, const int age, const double salary )
{
    return new RealPerson ( name, age, salary );
}
// end person.cpp

#pragma endregion   //END REGION 2

#pragma region 3.Declaration instead of implementation
//main.h
class Date; //这种前向声明一般都会放到一个头文件中，在大型项目中就不必要到处前向声明了 一般命名为：XXXfwd.h 
Date today ();
void SetData ( Date date );
//end main.h
#pragma endregion   //END REGION 3


int main ()
{
    Person* p = Person::create ( "name", 18, 10000 );
    std::cout << "name: " << p->name () << " age: " << p->age () << " salary: " << p->salary () << std::endl;
}