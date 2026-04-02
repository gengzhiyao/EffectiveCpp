#include <iostream>
#include <list>
/**
 * !通过复合 塑模出 has-a 关系或根据某物实现出
 * 1. 复合意味着 has-a 关系或
 * 根据某物实现的关系，当你进行复合的时候，一定要想到，你正在处理两个不同领域的对象
 * Scott Meyers 将此分为两种情况：
 *  抽象出来的人、汽车等，属于应用域；互斥锁、缓冲区属于实现域
 * 类中内含应用域表现出来 has-a 关系；内含实现域，表现出来 根据某物实现的关系
 */

// 假定，我们现在有一个需求，需要实现一个set容器
// 我们很自然的想到使用标准库中的set容器，但不幸的是，标准库中的set容器通过平衡树实现，速度快，但空间占用多
// 但我们的要求是占用的空间应该尽可能小，我们需要自己实现一个set容器，通过链表实现，空间占用少，但速度慢

template <typename T>
class MySet /*:public std::list<T>*/ //![WARN]: 错误做法
{
public:
    // Data Member

    // Set容器是不允许有重复元素出现的，但是当你继承自list时，list是允许有重复元素出现的
    // 并且你要谨记：public继承意味着 is-a 关系
    // 这种继承关系下，绝不是 is-a 关系

private:
    std::list<T> m_List;

public:
    bool member(const T& t) const
    {
        return std::find(m_List.begin(), m_List.end(), t) != m_List.end();
    }
    void insert(const T& t)
    {
        if (!member(t))
        {
            m_List.push_back(t);
        }
    }

    void remove(const T& t)
    {
        auto it = std::find(m_List.begin(), m_List.end(), t);
        if (it != m_List.end())
        {
            m_List.erase(it);
        }
    }

    std::size_t size() const
    {
        return m_List.size();
    }
};

int main()
{
    return 0;
}
