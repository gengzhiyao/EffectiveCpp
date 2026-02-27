#include <iostream>
#include <memory>
/**
 * !绝不要重新定义继承而来的虚函数的默认参数值
 * Notion: 继承而来的函数分为虚函数和非虚函数，对于非虚函数，本书中是禁止重新定义的，那么剩下的讨论就是对于虚函数来说了
 * 为什么？
 * 虚函数的机制是动态绑定，但是默认参数值却是静态绑定
 * 对于一个变量来说，静态绑定时依据声明时的类型进行确定的，但是动态绑定是依据虚函数表进行确定的
 *
 */

class Shape
{
public:
    enum class ShapeColor
    {
        Red,
        Green,
        Blue
    };

    virtual void draw ( ShapeColor color = ShapeColor::Red ) const = 0;
};

class Rectangle :public Shape
{
public:
    void draw ( ShapeColor color = ShapeColor::Green ) const override   // 重新定义缺省参数值是错误的
    {
        std::cout << "在重新定义默认参数值为绿色后，参数值为：";
        switch ( color )
        {
        case ShapeColor::Red:
            std::cout << "红色" << std::endl;
            break;
        case ShapeColor::Green:
            std::cout << "绿色" << std::endl;
            break;
        case ShapeColor::Blue:
            std::cout << "蓝色" << std::endl;
            break;
        default:
            break;
        }
    }

    /**
     * 如果父类中的虚函数有了默认值，子类中没有重定义的情况下， 即： void draw ( ShapeColor color ) const override
     * 当客户利用动态绑定的情况下，可以不指定参数的默认值，最终使用的是父类中的缺省参数值
     * !当客户使用对象调用函数时，即不发生多态的情况下，在调用时就必须给定参数值
     * 当然，更没有必要为父类和子类的虚函数定义同一个缺省参数值
     */
};

int main ()
{
    std::unique_ptr<Shape> shape ( new Rectangle );
    shape->draw ();     // output: 在重新定义默认参数值为绿色后，参数值为：红色  
    Rectangle rect;
    rect.draw ();    // 通过对象调用，必须给定参数值，这种情况不继承父类的默认参数值
    return 0;
}


