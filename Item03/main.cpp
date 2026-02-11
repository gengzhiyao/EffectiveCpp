#include <iostream>
#include <vector>
/**
 * 基础：const的二义性
 * 在 C 语言中，const修饰的变量，放在.rodata段中，并非表示是一个编译器常量，只是表示一个只读变量
 * 在 C++ 中，const修饰的变量具有了二义性：只读变量和编译器常量
 * 1. 只读变量：const 修饰的返回值，只能在运行时得到的常量，表示只读 const int ret = GetValue();
 * 2. 编译期常量：当const作用于编译期可以确定的常量表达式初始化的变量时，是编译期常量
 */

 /**
  * !尽可能使用const
  * 1. const 可以用来修饰任何作用域中的变量，包括global namespace class function block 还能够配合static进行修饰 还能够修饰成员函数是否为const
  * Notion:STL中的迭代器以指针为原型，因此const修饰迭代器时表示迭代器的指向不可变，但指向元素的值可变
  * 2. const & static 作为成员变量
  * bitwise const在编译器层面保证对象的常量性，但是不会递归检查对象的常量性是否改变，例如使用指针改变对象的值
  * logical const 配合mutable 从逻辑上保证对象的constness
  */

int GetVaule ()
{
    int value = 0;
    // ...
    return value + rand ();
}

class Rational
{

};

Rational operator*( const Rational& lhs, const Rational& rhs ) { /*DoSomething*/ return Rational(); } //不返回const对象的话，可能引发：(a*b)=c;其中:Rational a,b;

#pragma region Const-MemberFunction
// const 用于修饰成员函数
class TextBlock
{
public:
    TextBlock ( const std::string& value )
        :m_text ( value )
    {
    }
    const char& operator[]( std::size_t position ) const { return m_text[position]; }
    char& operator[]( std::size_t position ) { return m_text[position]; }
private:
    std::string m_text;
};

void Print ( const TextBlock& ctb ) { std::cout << ctb[0] << std::endl; }   //Notion: 尤其是在这种情况下，会调用const版本的成员函数

class CTextBlock
{
public:
    CTextBlock ( char* value )
        :pText ( value )
    {
    }
public:
    char& operator[]( std::size_t position ) const { return pText[position]; }  //bitwise const 声明，不适当的const成员函数返回一个对象内部的Handle

private:
    char* pText;
};
#pragma endregion   //END Const-MemberFunction

class PerfectTextBlock
{
public:
    //...
    const char& operator[]( std::size_t position ) const
    {
        //... 边界检查
        //... 日志数据访问
        //... 数据完整性校验
        return text[position];
    }

    /*
    char& operator[]( std::size_t position )
    {
        ... 边界检查
        ... 日志数据访问
        ... 数据完整性校验
        return text[position];
    }
    // 去除重复代码
    */

    //! 必须要求non-const函数中调用const版本的兄弟，反则不许
    // const 成员函数保证不改变对象的状态，但是non-const版本不保证，使用const版本调用non-const版本就是冒风险改变对象的状态
    char& operator[]( std::size_t position )
    {
        return const_cast< char& >( static_cast< const PerfectTextBlock& >( *this )[position] );    //去除重复代码
    }

private:
    std::string text;
};

int main ()
{
#pragma region 1. constAmbiguity    //const的二象性
    const int constant = 5; //编译期常量
    constexpr int constexpression = 5;// 编译期常量

    const int readOnly = GetVaule ();   // 只读变量

    int array0[constant] = { 0 };   //可以
    // int array1[readOnly] = { 0 };   //不可以
#pragma endregion

#pragma region 2. Iterator          //const与迭代器
// Iterator
    std::vector<int> vec ( 10, 2 );
    const std::vector<int>::iterator iter = vec.begin (); // ++iter; 不可以改变迭代器的指向

    std::vector<int>::const_iterator iter1 = vec.cbegin (); // *iter1 = 10;  不可以更改迭代器指向的值
#pragma endregion

#pragma region 3. Const-MemberFunction  //const与成员函数 bitwise/logical constness
    TextBlock tb ( "Hello" );
    const TextBlock ctb ( "World" );

    std::cout << "调用 non-const 版本的operator[] :" << tb[0] << std::endl;
    std::cout << "调用 const 版本的operator[] :" << ctb[0] << std::endl;

    tb[0] = 'L';
    // ctb[0] = 'U'; 该版本不能够改动只因为返回的是 const char& 而非const成员函数！

    const CTextBlock cctb ( "Hello" );
    char* pc = &cctb[0];
    *pc = 'J';
    std::cout << "这里对象cctb的bitwise状态被更改了吗？" << cctb[0] << std::endl;   // 实际上老旧编译期会更改，现在的编译器比较聪明了，上句代码会引发异常


#pragma endregion
    return 0;
}