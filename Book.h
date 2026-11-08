#pragma once
#ifndef BOOK_H
#define BOOK_H

#include <string>

// 图书类：保存图书的基本信息
class Book {
public:
    std::string id;         // 图书编号
    std::string title;      // 书名
    std::string author;     // 作者
    std::string publisher;  // 出版社
    int total;              // 总数量
    int available;          // 可借数量

    Book();
    Book(std::string id, std::string title, std::string author,
         std::string publisher, int total);

    void show() const;      // 输出一行图书信息
};

#endif
