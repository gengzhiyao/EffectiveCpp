cmake_minimum_required(VERSION 4.0)

project(EffectiveCpp)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(Item01)    #将C++看做是一个语言联邦
add_subdirectory(Item02)    #尽量以const enum inline 替换define
add_subdirectory(Item03)    #尽可能使用const
add_subdirectory(Item04)    #确定对象在使用前已经先初始化
add_subdirectory(Item05)    #了解C++默默编写并调用哪些函数
add_subdirectory(Item06)    #若不想使用编译器生成的特种成员函数，就明确拒绝
add_subdirectory(Item07)    #为多态基类声明virtual析构函数

add_subdirectory(Item09)    #绝不要在构造函数和析构函数中调用虚函数
add_subdirectory(Item10)    #令operator=返回自身的引用
add_subdirectory(Item11)    #在operator=中处理自我赋值
add_subdirectory(Item12)    #复制对象时切记复制包含基类在内的每个成分
add_subdirectory(Item13)    #以对象管理资源
add_subdirectory(Item14)    #在资源管理类中小心copying行为
add_subdirectory(Item15)    #在资源管理类中提供对原始资源的访问
add_subdirectory(Item16)    #成对使用new和delete时要采取相同的形式
add_subdirectory(Item17)    #以独立语句将newed对象置入智能指针
add_subdirectory(Item18)    #让接口容易被正确使用，不容易被误用
add_subdirectory(Item21)    #必须返回对象时妄想返回其reference



add_subdirectory(Item25)    #考虑写出一个不抛异常的 swap 函数
add_subdirectory(Item26)    #尽可能延后变量定义式的出现时间
add_subdirectory(Item27)    #尽量少做转型动作
add_subdirectory(Item28)    #避免返回handles指向对象内部的成分

add_subdirectory(Item30)    #透彻了解inlining的里里外外
add_subdirectory(Item31)    #将文件之间的编译依赖关系降至最低

add_subdirectory(Item33)    #避免隐藏继承下来的名称

add_subdirectory(Item36)    #绝不要重新定义继承而来的 non-virtual 函数