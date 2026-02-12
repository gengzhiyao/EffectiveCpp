#include <iostream>

/**
 * !了解C++默默编写并调用哪些函数
 * 当一个类中没有编写任何构造函数时，编译器会为你声明默认无参的构造函数，同时还会生成拷贝构造函数，拷贝赋值运算符以及析构函数，并且都是inline和public的，但是基类的析构函数默认不是virtual的
 * 他们只有被调用的时候编译器才会为他们创建
 * 编译器生成的拷贝构造函数只是将源对象的内一个个non-static成员变量拷贝到目标对象
 * 对于内置类型来说，是拷贝每一个bits到目标对象中；对于STL中的类型来说，是逐个调用各对象的拷贝构造函数
 * Notion:对于含有reference和const成员变量的类，编译期不会生成拷贝构造函数以及拷贝赋值运算符
 * 因为对于引用类型，成员变量在进行拷贝时，编译器无法改变引用谁，因为C++不允许让reference改指另一个对象；同样也不能主动修改所引用源对象的内部值，如改变"newDog"为"oldDog"
 * 此外，对于父类中删除的或者置为private的拷贝构造以及拷贝赋值运算，子类也无法合成对应的特种成员函数
 */

class Empty
{
public:
    // Empty () { }
    // Empty ( const Empty& rhs ) { }
    // Empty& operator=( const Empty& rhs ) { }
    // ~Empty () { }
};

template<typename T>
class NamedObject
{
public:
    NamedObject ( std::string& name, const T& value );
    //...
private:
    std::string& nameValue;
    const T objectValue;
};

int main ()
{
    std::string newDog ( "newDog" );
    std::string oldDog ( "oldDog" );

    NamedObject<int> p ( newDog, 2 );
    NamedObject<int> s ( oldDog, 6 );

    // p = s;   //![ERROR]

    return 0;
}