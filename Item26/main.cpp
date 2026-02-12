#include <iostream>
/**
 * !尽可能延后变量定义式的时间
 * 尽可能延后的意义：
 * 不应该只是延后变量定义的时间，更应该做的是：尝试延后该变量的定义时间直到我们能够调用其构造函数将参数传递给该类的成员变量，如suitablePositionforEncrypted，定义并初始化，而不是延后定义时间之后再重新赋值
 */

void Encrypt ( std::string& s )
{
    // 加密操作
}

std::string EncryptPassWord ( const std::string& password )
{
    std::string encrypted;  //可以看到，如果密码的长度过小，代码就直接跳过了解密密码的过程，这里对变量的定义是完全无效的
    if (password.length()<15)
    {
        return "just return an empty string or throw an exception";
    }

    //DoSomething...
    std::string suitablePositionforEncrypted ( password );
    Encrypt ( suitablePositionforEncrypted );
    // return encrypted;
    return suitablePositionforEncrypted;
}

int main ()
{
    return 0;
}