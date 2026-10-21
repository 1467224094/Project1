
#include "LibrarySystem.h"
#include "Logger.h"
#include <iostream>
#include <algorithm>
#include <limits>
#include <iomanip>
#include <vector>
#include <string>

LibrarySystem::LibrarySystem() {

    books.emplace_back("B001", "C++ Primer", "Lippman", 5);
    books.emplace_back("B002", "Design Patterns", "GoF", 3);

    readers.push_back(new Student("S001", "Alice"));
    readers.push_back(new Teacher("T001", "Dr. Bob"));

    Logger::log("System Initialized with test data.");
}

LibrarySystem::~LibrarySystem() {

    for (auto& p : readers) {
        delete p;
    }
    readers.clear();
}

Book* LibrarySystem::findBook(const std::string& id) {
    for (auto& b : books) {
        if (b.id == id) return &b;
    }
    return nullptr;
}

Person* LibrarySystem::findReader(const std::string& id) {
    for (auto& p : readers) {
        if (p->getId() == id) return p;
    }
    return nullptr;
}

void LibrarySystem::clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void LibrarySystem::displayAllReaders() {
    if (readers.empty()) {
        std::cout << "暂无读者。\n";
        return;
    }
    std::cout << "\n--- 读者列表 (多态展示) ---\n";
    for (const auto& p : readers) {
 
        std::cout << "[ID:" << p->getId() << "] " << p->getDescription() << std::endl;
    }
}

void LibrarySystem::readerMenu() {
    int choice;
    while (true) {
        std::cout << "\n--- 读者管理 ---\n";
        std::cout << "1. 添加学生\n2. 添加教师\n3. 删除读者\n4. 显示所有读者\n0. 返回\n";
        std::cout << "请选择: ";
        if (!(std::cin >> choice)) { clearInputBuffer(); continue; }
        if (choice == 0) break;

        std::string id, name;

        switch (choice) {
        case 1: {
            std::cout << "输入学号: "; std::cin >> id;
            if (findReader(id)) { std::cout << "用户已存在!\n"; break; }
            std::cout << "输入姓名: "; std::cin >> name;
            readers.push_back(new Student(id, name));
            Logger::log("Added Student: " + name);
            std::cout << "添加学生成功!\n";
            break;
        }
        case 2: { 
            std::cout << "输入工号: "; std::cin >> id;
            if (findReader(id)) { std::cout << "用户已存在!\n"; break; }
            std::cout << "输入姓名: "; std::cin >> name;
            readers.push_back(new Teacher(id, name));
            Logger::log("Added Teacher: " + name);
            std::cout << "添加教师成功!\n";
            break;
        }
        case 3: {
            std::cout << "输入编号: "; std::cin >> id;
            Person* p = findReader(id);
            if (!p) { std::cout << "用户不存在!\n"; break; }
            if (!p->getBorrowedBooks().empty()) {
                std::cout << "该用户有未还书籍，无法删除!\n";
                break;
            }
            readers.erase(remove(readers.begin(), readers.end(), p), readers.end());
            delete p;
            Logger::log("Deleted User: " + id);
            std::cout << "删除成功!\n";
            break;
        }
        case 4: {
            displayAllReaders();
            break;
        }
        default: {
            std::cout << "无效选项!\n";
        }
        }
    }
}

void LibrarySystem::bookMenu() {
    int choice;
    while (true) {
        std::cout << "\n--- 图书管理 ---\n";
        std::cout << "1. 添加图书\n2. 删除图书\n3. 显示所有图书\n0. 返回\n";
        std::cout << "请选择: ";
        if (!(std::cin >> choice)) { clearInputBuffer(); continue; }
        if (choice == 0) break;

        std::string id, title, author;
        int total;

        switch (choice) {
        case 1: {
            std::cout << "输入ISBN: "; std::cin >> id;
            if (findBook(id)) { std::cout << "图书已存在!\n"; break; }
            std::cout << "输入书名: "; std::cin >> title;
            std::cout << "输入作者: "; std::cin >> author;
            std::cout << "输入总量: "; std::cin >> total;
            books.emplace_back(id, title, author, total);
            Logger::log("Added Book: " + title);
            std::cout << "添加成功!\n";
            break;
        }
        case 2: {
            std::cout << "输入ISBN: "; std::cin >> id;
            bool borrowed = false;
            for (const auto& r : readers) {
                if (r->isBorrowed(id)) { borrowed = true; break; }
            }
            if (borrowed) { std::cout << "该书仍有借阅记录，无法删除!\n"; break; }

            books.erase(remove_if(books.begin(), books.end(), [&](const Book& b) { return b.id == id; }), books.end());
            Logger::log("Deleted Book: " + id);
            std::cout << "删除成功!\n";
            break;
        }
        case 3: {
            if (books.empty()) { std::cout << "暂无图书!\n"; break; }
            std::cout << std::left << std::setw(10) << "ISBN" << std::setw(20) << "Title" << std::setw(15) << "Author" << "Avail/Total\n";
            for (const auto& b : books) {
                std::cout << std::left << std::setw(10) << b.id << std::setw(20) << b.title << std::setw(15) << b.author
                    << b.availableCopies << "/" << b.totalCopies << "\n";
            }
            break;
        }
        default: {
            std::cout << "无效选项!\n";
        }
        }
    }
}

void LibrarySystem::borrowMenu() {
    int choice;
    while (true) {
        std::cout << "\n--- 借阅管理 ---\n";
        std::cout << "1. 借书\n2. 还书\n3. 查询个人借阅\n0. 返回\n";
        std::cout << "请选择: ";
        if (!(std::cin >> choice)) { clearInputBuffer(); continue; }
        if (choice == 0) break;

        std::string readerId, bookId;
        Person* reader = nullptr;

        switch (choice) {
        case 1: { 
            std::cout << "输入读者编号: "; std::cin >> readerId;
            reader = findReader(readerId);
            if (!reader) { std::cout << "读者不存在!\n"; break; }

      
            if (!reader->canBorrow()) {
                std::cout << "已达最大借阅数量 (" << reader->getMaxBorrow() << ")!\n";
                break;
            }

            std::cout << "输入图书ISBN: "; std::cin >> bookId;
            Book* book = findBook(bookId);
            if (!book) { std::cout << "图书不存在!\n"; break; }
            if (book->availableCopies <= 0) { std::cout << "图书已借完!\n"; break; }
            if (reader->isBorrowed(bookId)) { std::cout << "已借阅过该书!\n"; break; }

            book->availableCopies--;
            reader->addBorrowRecord(bookId);
            Logger::log("Borrow: " + reader->getName() + " borrowed " + book->title);
            std::cout << "借书成功!\n";
            break;
        }

        case 2: { 
            std::cout << "输入读者编号: "; std::cin >> readerId;
            reader = findReader(readerId);
            if (!reader) { std::cout << "读者不存在!\n"; break; }

            std::cout << "输入图书ISBN: "; std::cin >> bookId;
            if (!reader->isBorrowed(bookId)) { std::cout << "该读者未借阅此书!\n"; break; }

            Book* retBook = findBook(bookId);
            if (retBook) retBook->availableCopies++;

            reader->removeBorrowRecord(bookId);
            Logger::log("Return: " + reader->getName() + " returned " + bookId);
            std::cout << "还书成功!\n";
            break;
        }

        case 3: { 
            std::cout << "输入读者编号: "; std::cin >> readerId;
            reader = findReader(readerId);
            if (!reader) { std::cout << "读者不存在!\n"; break; }

            std::cout << "\n读者: " << reader->getName() << " (" << reader->getType() << ") 的借阅记录:\n";
            if (reader->getBorrowedBooks().empty()) {
                std::cout << "无借阅记录。\n";
            }
            else {
                for (const auto& bId : reader->getBorrowedBooks()) {
                    Book* b = findBook(bId);
                    if (b) std::cout << "- " << b->getInfo() << std::endl;
                    else std::cout << "- [Unknown Book: " << bId << "]" << std::endl;
                }
            }
            break;
        }
        default: {
               std::cout << "无效选项!\n";
        }
    }
    }
}

void LibrarySystem::run() {
    int choice;
    while (true) {
        std::cout << "\n========================================\n";
        std::cout << "      Swust Library Management System     \n";
        std::cout << "========================================\n";
        std::cout << "1. 图书管理 (Book Mgmt)\n";
        std::cout << "2. 读者管理 (Reader Mgmt - Poly)\n";
        std::cout << "3. 借阅管理 (Circulation)\n";
        std::cout << "0. 退出 (Exit)\n";
        std::cout << "----------------------------------------\n";
        std::cout << "Choice: ";

        if (!(std::cin >> choice)) {
            clearInputBuffer();
            continue;
        }

        if (choice == 0) {
            Logger::log("System Shutdown.");
            break;
        }

        switch (choice) {
        case 1: { bookMenu(); break; }
        case 2: { readerMenu(); break; }
        case 3: { borrowMenu(); break; }
        default: { std::cout << "Invalid Option.\n"; }
        }
    }
}