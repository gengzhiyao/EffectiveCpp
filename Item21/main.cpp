#include <iostream>

/**
 * !必须返回对象时切忌返回其reference
 * 1. 思考：什么叫做必须返回对象呢？ 那么什么又叫做reference呢？ 见下例
 */

#define RETURN_PASS_BY_VALUE 0      //决定是以pass-by-value还是pass-by-reference的方式返回

class Rational
{
public:
    Rational ( int numerator = 0, int denominator = 1 )
        :n ( numerator ), d ( denominator )
    {
        std::cout << "Rational::Ctor" << std::endl;
    }
    //...
private:
    int n, d;
#if RETURN_PASS_BY_VALUE
    friend const Rational operator*( const Rational& lhs, const Rational& rhs );    //可见，这里以pass-by-value的方式返回，这样看上去付出了调用构造和析构函数的成本，这样做真的不对吗???
#else
    friend const Rational& operator*( const Rational& lhs, const Rational& rhs );
#endif
};

#if RETURN_PASS_BY_VALUE
const Rational operator*( const Rational& lhs, const Rational& rhs )
{
    return ( lhs.n * rhs.n ) / ( lhs.d * rhs.d );
}
#else 
const Rational& operator*( const Rational& lhs, const Rational& rhs )
{
#define RETURN_STACK_OBJECT 0
#define RETURN_HEAP_OBJECT 0
#define RETURN_STATIC_OBJECT 1
#if RETURN_STACK_OBJECT
    Rational result ( lhs.n * rhs.n, lhs.d * rhs.d );   //Return_Stack_Object
    return result;  //[ERROR]：首先付出了一个调用构造函数的代价，更为危险的是，返回了局部变量的引用！
#elif RETURN_HEAP_OBJECT
    Rational* result = new Rational ( lhs.n * rhs.n, lhs.d * rhs.d );
    return *result; //[ERROR]: 一旦返回一个heap对象，那么问题来了：谁应该负责delete？
    /**
     * 尤其是面对这种代码的时候：
     * Rational w,x,y,z;
     * w = x * y * z;
     * !看似简单的一句代码，实则调用了两次new，创建了两个heap对象，为什么？
     * ! 展开：w = operator*( x, operator*( y, z ) ); 调用两次operator* 创建两次heap对象，并且无法delete！
     */
#elif RETURN_STATIC_OBJECT  //对应Item04 reference-returning 函数，可见应用场景不一致，这里的函数就是错误范例
    static Rational result ( lhs.n * rhs.n, lhs.d * rhs.d );
    return result;
    /**
     * 同样是糟糕的代码
     * 1. 首先为多线程引入的麻烦
     * 2. 其次看这段代码
     * bool operator==(const Rational& lhs, const Rational& rhs)
     *{
     *  if((a*b)==(c*d)){...}   //这段代码总是返回true，想想为什么？可以展开看      =>返回的永远是static对象的新值
     * }
     * 再者，我们更不用提是否需要返回一个array了...
     */
#endif
    //Notion:总的来说: 在这种情况下，我们不得不pass-by-value
}
#endif  // END RETURN_PASS_BY_VALUE

int main ()
{
#if !RETURN_PASS_BY_VALUE
    Rational a ( 1, 2 );
    Rational b ( 3, 5 );
    Rational c = a * b; //异常
#endif
    return 0;
}