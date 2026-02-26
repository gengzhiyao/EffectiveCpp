#include <iostream>

/**
 * !确定 public 继承塑模出 is-a 关系
 * Notion: ** public 继承意味着 is-a 关系 **
 * 子类通过 public 继承父类，表明子类对象同时是一个父类对象，父类表现出来的是更加一般化的概念，子类表现出比父类更特殊的概念
 * 但是 private 继承却不会表现出 is-a 关系，更为重要的一点是 private 继承和 protected 继承根本不会表现出多态
 * 条款中：Scott Meyers 针对 public 继承进行了批判性的探讨：世界上并不存在适用于所有软件的完美设计
 */

class Brid
{
public:
    virtual void fly ();    // 鸟可以飞
};

class Penguin :public Brid  // 企鹅是一种鸟
{
public:
    //在这个继承体系中存在一定的问题：即：企鹅事实上不会飞，但是继承体系中强制给了飞行的方法
    virtual void fly () override
    {
        // 有一派的做法是：不合理的设计在运行时给出一个提示或运行期错误
        // error("企图使企鹅飞行")
    }

};

#if false   // 可能更合适的塑模方案
class Brid
{
public:
    // ...
};

class FlyingBrid :public Brid
{
public:
    virtual void fly ();    // 鸟可以飞
};

class Penguin :public Brid
{

};

#endif

// 但有些软件可能根本不在乎鸟是否能够飞行，所以我们的设计需要根据需求指定出符合需求的设计

int main ()
{
    return 0;
}