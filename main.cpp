#include <iostream>
#include <windows.h> 
#include "LibrarySystem.h" 

using namespace std;

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    cout << "正在启动...\n";

    LibrarySystem lib;
    lib.run();

    return 0;
}