#include <iostream>

/**
 * !确定对象被使用前已经被初始化
 * 1. 对象的初始化与否首先要看该对象/变量所在的作用域位置，位于栈区的变量不会被初始化，即便是一个类，只要位于全局区，那么也会被默认初始化为0
 *  C++ 对于内置类型(C part of C++)部分不保证默认初始化，得出来的都是随机数，对于STL中的类型来说，保证构造对象时就已经被初始化
 * 2. 确保构造函数中，对每一个成员变量都初始化，并且是在构造函数初始化列表中，而非在构造函数的函数体内，因为函数体内是赋值操作  **对象的初始化动作发生在进入构造函数本体之前
 *  发生在构造函数初始化列表中的操作是调用各个成员变脸的构造函数，但是发生在构造函数的函数体内的操作是调用了成员变量的 operator=
 *  对于无参构造函数，那么我们在构造函数初始化列表中初始化这种类型的成员变量时，可以直接调用构造函数如：m_memberData()
 * 3. 对于成员变量是 const 类型或者是 reference 类型，必须使用初始化列表进行初始化，而非使用构造函数体内的赋值，这会导致编译错误
 * 4. 初始化次序问题：
 *  基类的构造函数总是优先于派生类被调用，成员变量的初始化总是以声明次序进行初始化
 *  Notion：non-local static 对象的初始化次序是不确定的 包括global namespace class
 * !C++ 对于定义于不同编译单元内的 non-local static 对象的初始化次序并无明确的定义
 */

// a.cpp
class FileSystem
{
public:
    std::size_t numDisks () const;
};

extern FileSystem etfs;  //我们要求，所有使用filesystem的对象都需要等待FileSystem构造完成之后才能进行初始化

//b.cpp
// 很显然的一点是，我们的tfs对象必须要在tempDir对象之前首先进行初始化，否则tempDir会调用到尚未初始化的tfs对象，造成未定义行为
class Directory
{
public:
    Directory (/*Args*/ )
    {
        std::size_t disks = etfs.numDisks ();
    }
};

Directory tempDir /*(Args)*/;

//! 解决办法也挺简单：reference-returning   (呼应Item21)
// 将以上静态变量以函数进行包裹
FileSystem& tfs ()
{
    static FileSystem fs;
    return fs;
}

Directory& tempDir()
{
    static Directory td;
    return td;
}
// 这样修改是因为： 函数内的local static对象会在该函数被调用时被初始化 更好的一点是，如果你不调用，那么这个对象就不会产生构造和析构的成本

// main.cpp
int main ()
{

    return 0;
}