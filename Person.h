#pragma once
#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <vector>

// 借阅人类：抽象基类，学生和教师都从它派生
class Person {
protected:
    std::string id;                 // 学号 / 工号
    std::string name;               // 姓名
    std::string dept;               // 院系
    int maxBorrow;                  // 最多可借数量
    std::vector<std::string> books; // 已借图书编号

public:
    Person(std::string id, std::string name, std::string dept, int maxBorrow);
    virtual ~Person() {}

    std::string getId() const;
    std::string getName() const;
    std::string getDept() const;
    int getMaxBorrow() const;
    std::vector<std::string> getBooks() const;

    void setName(std::string name);
    void setDept(std::string dept);

    virtual std::string getType() const = 0;  // 学生 / 教师
    virtual void show() const;                // 显示一行信息

    bool canBorrow() const;                   // 是否还能继续借
    bool hasBorrowed(std::string bookId) const;
    void borrowBook(std::string bookId);
    void returnBook(std::string bookId);
};

// 学生：最多借 3 本
class Student : public Person {
public:
    Student(std::string id, std::string name, std::string dept, int maxBorrow = 3);
    std::string getType() const override;
};

// 教师：最多借 10 本
class Teacher : public Person {
public:
    Teacher(std::string id, std::string name, std::string dept, int maxBorrow = 10);
    std::string getType() const override;
};

#endif
