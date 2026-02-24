#include <iostream>

/**
 * !成对使用new/delete时采用相同的形式
 * Notion: 单一对象的内存布局不同于数组的内存布局 明确来讲：数组所用的内存通常还包括数组大小的记录
 * 在 new 一个对象时，先分配内存，然后调用构造函数；在 delete 一个对象时，先调用对象的析构函数，再释放空间
 * typedef std::string AddressLines[4];
 * 对于数组调用 delete 和对于单一对象调用 delete[] 都会引发 UB
 */

typedef std::string AddressLines[4];

 int main()
 {
     std::string* stringArray = new std::string[100];
     std::string* string = new std::string;

     delete string;
     delete [ ] stringArray;

     std::string* ar = new AddressLines;    // 尤其是对于喜欢使用 typedef/using 的人来说，new和delete 成对出现更为重要

     delete [ ] ar;
     return 0;
 }