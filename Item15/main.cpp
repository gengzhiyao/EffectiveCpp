#include <iostream>
#include <memory>
/**
 * !在资源管理类中提供对原始资源的访问
 * 资源管理类中存在两种提供对原始资源访问的办法
 * 1. 提供一个 get() 函数，返回原始资源的拷贝
 * 2. 提供一个隐式转换函数，将资源管理类隐式转换为原始资源
 */

class Investment { /*...*/ };

class ConcreteInvestment :public Investment {/*...*/ };

Investment* CreateInvestment () //factory function
{
    return new ConcreteInvestment;
}

// 假定现在存在某一 API，要求提供原始的裸指针
int DaysHeld ( const Investment* pi )
{
    std::cout << "返回投资天数" << std::endl;
    return 9;
}

#pragma region Font
typedef void* FontHandle;   // 假定 FontHandle 作为一个 GDI 对象的句柄

FontHandle GetFont ();

void ReleaseFont ( FontHandle fontHandle );

class Font
{
public:
    explicit Font ( FontHandle fontHandle )
        :f ( fontHandle )
    { }

    ~Font ()
    {
        ReleaseFont ( f );
    }

    FontHandle get () const     // 通过 get 获得原始资源
    {
        return f;
    }

    operator FontHandle() const // 提供隐式转换构造函数，转换为原始资源
    {
        return f;
    }
private:
    FontHandle f;
};

void ChangeFontSize ( FontHandle hFont, int size )
{
    std::cout << "改变hFont的大小为size指定的值" << std::endl;
}
#pragma endregion

int main ()
{
    std::shared_ptr<Investment> pi ( CreateInvestment () );
    DaysHeld ( pi.get () );
    Font font ( GetFont () );

    // 假定我先现在想要拷贝一个Font对象
    FontHandle f1 = font;   //但由于隐式转换函数的存在，实际上发生的却是FontHandle的拷贝 FontHandle f1 = font.get();
    return 0;
}