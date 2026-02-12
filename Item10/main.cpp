
/**
 * !令operator=返回自身的引用
 * 为了像内置类型一样能够实现连续赋值，包括=，+=，-=，这些运算符的重载尽量返回一个自身的引用
 */

class Widget
{
public:
    Widget () = default;
    Widget ( int value )    //non-explicit
        :m_value ( value )
    {
    }
    Widget& operator=( const Widget& rhs )
    {
        //...
        return *this;
    }
private:
    int m_value;
};

int main ()
{
    int x, y, z;
    x = y = z = 15;

    Widget a, b, c;
    a = b = c = 15; 
    // a.operator=( b.operator=( c = 15 ) );
    return 0;
}