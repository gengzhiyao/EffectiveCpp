#include <iostream>
#include <functional>
/**
 * !考虑虚函数以外的其他选择
 */

#pragma region Implementing the Template Method Pattern using NVI
class GameCharacter1
{
public:
    int HealthValue () const        // 模板方法
    {
        return DoHealthValue ();
    }

private:
    virtual int DoHealthValue () const
    {
        std::cout << "GameCharacter::DoHealthValue" << std::endl;
        return 100;
    }
};

class ConcreteCharactre :public GameCharacter1
{
private:
    virtual int DoHealthValue () const override
    {
        std::cout << "ConcreteCharactre::DoHealthValue" << std::endl;
        return 100;
    }
};
#pragma endregion

#pragma region Implementing the strategy pattern using function pointers
class GameCharacter2;
int DefaultHealthFunc ( const GameCharacter2& character )
{
    std::cout << "DefaultHealthFunc" << std::endl;
    return 99;
}
class GameCharacter2
{
public:
    typedef int ( *HealthCalcFunc ) ( const GameCharacter2& );   // 声明一个函数指针
    explicit GameCharacter2 ( HealthCalcFunc defaultCalcFunc = DefaultHealthFunc )
        :m_someHealthCalcFunction ( defaultCalcFunc )
    {

    }

    int HealthValue () const
    {
        return m_someHealthCalcFunction ( *this );
    }
private:
    HealthCalcFunc m_someHealthCalcFunction;
};

class EvilBadGuy :public GameCharacter2
{
public:
    explicit EvilBadGuy ( HealthCalcFunc hcf = DefaultHealthFunc )
        :GameCharacter2 ( hcf )
    {

    }
};

int LoseHealthQuickly ( const GameCharacter2& ) { std::cout << "LoseHealthQuickly" << std::endl; return 99; }
int LoseHealthSlowly ( const GameCharacter2& ) { std::cout << "LoseHealthSlowly" << std::endl; return 99; }
#pragma endregion

#pragma region Implementing the strategy pattern using std::function
class GameCharacter3;
int DefaultHealthFunc3 ( const GameCharacter3& character )
{
    std::cout << "DefaultHealthFunc" << std::endl;
    return 99;
}
class GameCharacter3
{
public:
    typedef std::function<int ( const GameCharacter3& )> HealthCalcFunc;
    explicit GameCharacter3 ( HealthCalcFunc defaultCalcFunc = DefaultHealthFunc3 )
        :m_someHealthCalcFunction ( defaultCalcFunc )
    {

    }

    int HealthValue () const
    {
        return m_someHealthCalcFunction ( *this );
    }
private:
    HealthCalcFunc m_someHealthCalcFunction;
};

class EvilBadGuy3 :public GameCharacter3
{
public:
    explicit EvilBadGuy3 ( HealthCalcFunc hcf = DefaultHealthFunc3 )
        :GameCharacter3 ( hcf )
    {

    }
};


// 签名不同的函数指针
short ShortHealthFunc ( const GameCharacter3& character )
{
    std::cout << "ShortHealthFunc" << std::endl;
    return 99;
}

// 重载 operator() 的可调用对象
struct HealthCalculator
{
    int operator()(const GameCharacter3& character)
    {
        std::cout << "operator()" << std::endl;
        return 99;
    }
};

// 使用std::bind 将成员函数包装为函数指针
class GameLevel
{
public:
    float health ( const GameCharacter3& ) const
    {
        std::cout << "GameLevel::health" << std::endl;
        return 98;
    }
};
#pragma endregion

#pragma Replace virtual functions with the traditional strategy pattern.
class GameCharacter;
class HealthCalcFunc
{
public:
    virtual int Calc ( const GameCharacter& ) const
    {
        std::cout << "HealthCalcFunc::Calc" << std::endl;
        return 99;
    }
};

HealthCalcFunc healthFunc;
class GameCharacter
{
public:
    explicit GameCharacter ( HealthCalcFunc* phcf = &healthFunc )
        :pHealthCalcFunc(phcf)
    {
        
    }

    int HealthValue () const
    {
        return pHealthCalcFunc->Calc ( *this );
    }

private:
    HealthCalcFunc* pHealthCalcFunc;
};
#pragma endregion
int main ()
{
    // 模板方法模式
    GameCharacter1* gameCharacter = new ConcreteCharactre;
    std::cout << gameCharacter->HealthValue () << std::endl;

    // 函数指针实现策略模式
    EvilBadGuy ebg1 ( LoseHealthQuickly );
    EvilBadGuy ebg2 ( LoseHealthSlowly );
    ebg1.HealthValue ();
    ebg2.HealthValue ();

    // std::function实现策略模式
    EvilBadGuy3 ebg3 ( ShortHealthFunc );    // 函数指针
    // EvilBadGuy3 ebg4 (HealthCalculator () ); // 被解析为函数声明
    EvilBadGuy3 ebg4 { HealthCalculator () };   // 可调用对象
    GameLevel gl;
    EvilBadGuy3 ebg5 ( std::bind ( &GameLevel::health, gl, std::placeholders::_1 ) );
    ebg3.HealthValue ();
    ebg4.HealthValue ();
    ebg5.HealthValue ();

    return 0;
}