#include <iostream>
#include "Library.h"
#include "Menu.h"

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(65001);   
    SetConsoleCP(65001);
#endif

    Library lib;
    lib.load();

    runSystem(lib);

    lib.save();
    cout << "数据已经保存，再见！" << endl;
    return 0;
}
