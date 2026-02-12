cmake_minimum_required(VERSION 4.0)

project(EffectiveCpp)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(Item01)    #将C++看做是一个语言联邦
add_subdirectory(Item02)    #尽量以const enum inline 替换define
add_subdirectory(Item03)    #尽可能使用const
add_subdirectory(Item04)    #确定对象在使用前已经先初始化
add_subdirectory(Item05)    #了解C++默默编写并调用哪些函数





add_subdirectory(Item21)    #必须返回对象时妄想返回其reference


add_subdirectory(Item27)    #尽量少做转型动作
add_subdirectory(Item28)    #避免返回handles指向对象内部的成分

add_subdirectory(Item30)    #透彻了解inlining的里里外外
add_subdirectory(Item31)    #将文件之间的编译依赖关系降至最低

add_subdirectory(Item33)    #避免隐藏继承下来的名称

add_subdirectory(Item36)    #绝不要重新定义继承而来的 non-virtual 函数