#pragma once
#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include <vector>

#include "Book.h"
#include "Person.h"

// 图书管理系统：负责菜单、图书、读者和借阅
class Library {
public:
    Library();
    ~Library();
    void run();                     // 启动系统

private:
    std::vector<Book> books;        // 所有图书
    std::vector<Person*> readers;   // 所有读者（学生/教师）

    std::string username;           // 管理员账号
    std::string password;           // 管理员密码

    bool login();                   // 登录
    void changePassword();          // 修改密码

    bool loadData();                // 从文件读数据
    void saveData();                // 把数据写回文件

    Book* findBook(std::string id);
    Person* findReader(std::string id);
    int borrowedCount(std::string bookId) const;   // 这本书被借出多少本

    void bookMenu();
    void readerMenu();
    void borrowMenu();
    void statistics();

    void addBook();
    void modifyBook();
    void deleteBook();
    void showBooks();
    void searchBook();

    void addReader();
    void modifyReader();
    void deleteReader();
    void showReaders();

    void borrowBook();
    void returnBook();
    void showReaderBooks();
};

#endif
