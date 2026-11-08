#include <iostream>
#include <windows.h>

#include "Library.h"

using namespace std;

int main() {
    SetConsoleOutputCP(CP_UTF8);   // 控制台显示中文

    Library lib;
    lib.run();

    return 0;
}
