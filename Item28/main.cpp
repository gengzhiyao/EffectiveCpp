#include <iostream>
#include <memory>

/**
 * !避免返回Handles指向对象内部的成分
 * Handles: Pointers, references, iterator
 * 内部: 私有成员变量，私有成员函数，指代一切被封装起来的事物
 * 为什么？
 * 在 class Rectangle 中，我们明明使用 const 修饰了成员函数，保证该函数不改变类内成员变量的值
 * 但是成员函数返回了Handles指向“对象内部”的成分
 * 这时候我们通过引用调用非 const 函数，还是将 class Rectangle 中的数据修改了！
 * 注意：
 * 1. 成员变量的封装性等价于返回其Handle的成员函数的封装性
 * 2. const 成员函数返回 non-const reference，且该返回值存储于对象外部(Point)，并且和对象自身有关联，那么该函数的调用者可以修改这笔数据!
 * 
 * 解决方案很明显：
 * 1. 返回一个 const reference
 * 2. 问题：
 *    Notion:返回一个Handles会出现生命周期产生的异常问题：最明显的是，返回局部变量或临时对象的引用!
 */

class Point
{
public:
    Point () = default;
    Point ( int x, int y )
        :m_x ( x ), m_y ( y )
    {
    }

    void SetX ( int nValue ) { m_x = nValue; }
    void SetY ( int nValue ) { m_y = nValue; }

    int GetX () const { return m_x; }
    int GetY () const { return m_y; }

private:
    int m_x, m_y;
};

struct RectData
{
    Point leftTop;
    Point rightBottom;
};

class Rectangle
{
public:
    Rectangle ( Point nLeftTop, Point nRightBottom )
    {
        m_pData = std::make_shared<RectData> ();
        m_pData->leftTop = nLeftTop;
        m_pData->rightBottom = nRightBottom;
    }

    Point& GetLeftTop () const
    {
        return m_pData->leftTop;
    }

    Point& GetRightBottom () const
    {
        return m_pData->rightBottom;
    }
private:
    std::shared_ptr<RectData> m_pData;
};

int main ()
{
    Point value1 ( 1, 1 );
    Point value2 ( 3, 3 );

    const Rectangle rect ( value1, value2 );
    rect.GetLeftTop ().SetX ( 2 );

    std::cout << rect.GetLeftTop ().GetX() << std::endl;
}

class GUIObject
{

};

const Rectangle BoundingBox(const GUIObject& nGuiObj);

GUIObject* obj =new GUIObject();

/**
 * BoundingBox(*obj)这里已经返回了一个临时对象，临时对象通过GetRightBottom()返回了一个 Handle 
 * 指针pRightBottom指向了这个临时对象，表达式结束后，临时对象被销毁
 * Notion:Which Makes Dangling Handles
 * 只是通常不建议返回指向对象内部成分的Handles，但有时候不得不这么做，比如operator[]
 */
const Point* pRightBottom=&(BoundingBox(*obj).GetRightBottom());