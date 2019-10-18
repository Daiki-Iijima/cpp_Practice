#include <iostream>

using std::cout;
using std::endl;

//  ポインタ渡し
void func(int *a)
{
    cout << a[0] << endl;
}

int main()
{
    int a[] = {3, 4, 5, 6};
    func(a);
}