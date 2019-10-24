# int => string
- to_string(**int型** or **double型**)を使用する

```cpp
#include <stdlib.h>
using namespace std;

int main(int argc, char *argv[])
{
    int num = 1234;
    string str;

    str = to_string(num);    //  =>1234
}
```

# string => int
- stoi(**string型**)を使用する

```cpp
#include <stdlib.h>
using namespace std;

int main(int argc, char *argv[])
{
    string st = "1234";
    int num;

    int num = stoi(str);    //  =>1234
}
```
