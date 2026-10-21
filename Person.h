#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <vector>

class Person {
protected:
    std::string id;
    std::string name;
    std::vector<std::string> borrowedBookIds;

public:
    Person(std::string i, std::string n);
    virtual ~Person();


    virtual int getMaxBorrow() const = 0;
    virtual std::string getType() const = 0;


    std::string getId() const;
    std::string getName() const;


    bool canBorrow() const;
    bool isBorrowed(const std::string& bookId) const;
    void addBorrowRecord(const std::string& bookId);
    void removeBorrowRecord(const std::string& bookId);
    const std::vector<std::string>& getBorrowedBooks() const;

    virtual std::string getDescription() const;
};

class Student : public Person {
public:
    Student(std::string i, std::string n);
    int getMaxBorrow() const override;
    std::string getType() const override;
};

class Teacher : public Person {
public:
    Teacher(std::string i, std::string n);
    int getMaxBorrow() const override;
    std::string getType() const override;
    std::string getDescription() const override; 
};
#endif

