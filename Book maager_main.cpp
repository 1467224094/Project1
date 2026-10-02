#include "Bookmanager.h"

#include <iostream>

int main() {
    BookManager manager;
    if (manager.loadFromFile()) {
        std::cout << "已从 books.txt 载入图书数据。\n";
    } else {
        std::cout << "未找到数据文件，将以空库启动（首次运行正常）。\n";
    }

    manager.menu();
    return 0;
}
