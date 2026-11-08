#ifndef PERSON_H
#define PERSON_H

#include <string>

class Book;  


class Person
{
protected:
    std::string id;        
    std::string name;    
    std::string dept;    
    int maxNum;          
    int borrowedNum;       
    std::string password;   

public:
    Person(std::string id, std::string name, std::string dept, int maxNum);
    virtual ~Person();

    std::string getId() const;
    std::string getName() const;
    std::string getDept() const;
    int getMaxNum() const;
    int getBorrowedNum() const;
    std::string getPassword() const;

    void setName(std::string v);
    void setDept(std::string v);
    void setMaxNum(int n);
    void setBorrowedNum(int n);
    void setPassword(std::string v);


    virtual int getType() const = 0;
    virtual std::string getTypeName() const = 0;
    virtual int getLevel() const = 0;
    virtual int getBorrowDays() const = 0;
    virtual std::string getExtraName() const = 0;
    virtual std::string getExtra() const = 0;
    virtual void setExtra(std::string v) = 0;

    virtual void show() const;

    bool canBorrow(Book* b) const;
    bool isFull() const;
};

class Student : public Person
{
protected:
    std::string major;  

public:
    Student(std::string id, std::string name, std::string dept,
            int maxNum, std::string major);

    std::string getMajor() const;
    void setMajor(std::string v);

    std::string getExtraName() const;
    std::string getExtra() const;
    void setExtra(std::string v);
};

class Undergraduate : public Student
{
public:
    Undergraduate(std::string id, std::string name, std::string dept,
                  int maxNum, std::string major);

    int getType() const;
    std::string getTypeName() const;
    int getLevel() const;
    int getBorrowDays() const;
};


class Graduate : public Student
{
public:
    Graduate(std::string id, std::string name, std::string dept,
             int maxNum, std::string major);

    int getType() const;
    std::string getTypeName() const;
    int getLevel() const;
    int getBorrowDays() const;
};

class Teacher : public Person
{
private:
    std::string title;   

public:
    Teacher(std::string id, std::string name, std::string dept,
            int maxNum, std::string title);

    std::string getTitle() const;
    void setTitle(std::string v);

    int getType() const;
    std::string getTypeName() const;
    int getLevel() const;
    int getBorrowDays() const;
    std::string getExtraName() const;
    std::string getExtra() const;
    void setExtra(std::string v);
};

#endif
