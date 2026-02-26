#include <iostream>

/**
 * !若所有形参都需要类型转换，请声明为非成员函数
 * Notion: 本条款中最为重要的一句话：只有当参数被列于函数的参数列表中时，这个参数才能够执行可能的隐式转换。 这意味这，对于成员函数，其参数中隐含着一个 this 指针，这个隐含的参数是无法参与类型转换的
 * 这条规则可以推广到更一般的原则：使用对称运算符时，考虑采用非成员函数
 */

#define USE_NON_MEMBER_FUNCTION 0   // 置1代表使用非成员函数 operator*

 // 假设现在需要实现一个有理数相乘的类
class Rational
{
public:
    Rational ( int numerator = 0, int denominator = 1 )    // 构造函数刻意不为 explicit
        :m_numerator ( numerator ), m_denominator ( denominator )
    { }

    int numerator () const { return m_numerator; }
    int denominator () const { return m_denominator; }

#if !USE_NON_MEMBER_FUNCTION
    // 作为成员函数做有理数相乘
    const Rational operator*( const Rational& rhs ) const
    {
        return Rational ( m_numerator * rhs.numerator (), m_denominator * rhs.denominator () );
    }
    #endif
private:
    int m_numerator;
    int m_denominator;
};

#if USE_NON_MEMBER_FUNCTION
// 使编译器在每一个实参身上执行隐式类型转换
const Rational operator*( const Rational& lhs, const Rational& rhs )
{
    return Rational ( lhs.numerator () * rhs.numerator (), lhs.denominator () * rhs.denominator () );
}
#endif

/**
 * 为什么没有采用友元函数的形式？
 * 如果非成员函数必须访问私有成员，那么让它成为友元是可以接受的。但更好的设计是让非成员函数只使用类的公有接口实现。
 * 更一般性的原则：能够使用 public 接口，就不要访问类的私有数据
 */

int main ()
{
    Rational oneEight ( 1, 8 );
    Rational oneHalf ( 1, 2 );
    Rational result = oneHalf * oneEight;

    std::cout << "分子：" << result.numerator () << "  分母：" << result.denominator () << std::endl;

    result = oneHalf * 2;
    #if USE_NON_MEMBER_FUNCTION
    result = 2 * oneHalf;    //[WARN]: 使用 operator* 作为成员函数会报错 ,不支持乘法交换律
    // 展开：result = 2.operator*(oneHalf)
#endif
    return 0;
}