#include <iostream>

using std::cout;
using std::endl;

//  値渡し
void valueFunc(int i)
{
    i++;

    //  この関数内のiの値
    cout << "valueFunc[i]値 : " << i << endl;
    cout << "valueFunc[i]アドレス : " << &i << endl;
}

//  ポインタ渡し
void pointerFunc(int *i)
{
    *i += 1;
    cout << "main[i]アドレス : " << i << endl;
    cout << "この関数内のmain[i]のポインタのアドレス : " << &i << endl;
}

//  参照渡し
void referenceFunc(int &i)
{
    i++;
    cout << "main[i]の値 : " << i << endl;
    cout << "main[i]アドレス : " << &i << endl;
}
int main()
{
    int i = 5;

    cout << "main[i]の値 : " << i << endl;
    cout << "main[i]アドレス : " << &i << endl;

    valueFunc(i);
    pointerFunc(&i);
    referenceFunc(i);
}