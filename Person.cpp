#include "Person.h"
#include "Book.h"
#include <iostream>

using namespace std;


Person::Person(string id, string name, string dept, int maxNum)
    : id(id), name(name), dept(dept), borrowedNum(0)
{
    if (maxNum < 0) maxNum = 0;
    this->maxNum = maxNum;
    password = "123456";
}

Person::~Person()
{
}

string Person::getId() const
{
    return id;
}

string Person::getName() const
{
    return name;
}

string Person::getDept() const
{
    return dept;
}

int Person::getMaxNum() const
{
    return maxNum;
}

int Person::getBorrowedNum() const
{
    return borrowedNum;
}

string Person::getPassword() const
{
    return password;
}

void Person::setName(string v)
{
    name = v;
}

void Person::setDept(string v)
{
    dept = v;
}

void Person::setMaxNum(int n)
{
    if (n >= 0) maxNum = n;
}

void Person::setBorrowedNum(int n)
{
    if (n >= 0) borrowedNum = n;
}

void Person::setPassword(string v)
{
    password = v;
}

void Person::show() const
{
    cout << id << " | " << name << " | " << dept << " | " << getTypeName()
         << " | 已借 " << borrowedNum << "/" << maxNum
         << " | " << getExtraName() << ":" << getExtra()
         << endl;
}

bool Person::canBorrow(Book* b) const
{
    return getLevel() >= b->getNeedLevel();
}

bool Person::isFull() const
{
    return borrowedNum >= maxNum;
}


Student::Student(string id, string name, string dept,
                 int maxNum, string major)
    : Person(id, name, dept, maxNum), major(major)
{
}

string Student::getMajor() const
{
    return major;
}

void Student::setMajor(string v)
{
    major = v;
}

string Student::getExtraName() const
{
    return "专业";
}

string Student::getExtra() const
{
    return major;
}

void Student::setExtra(string v)
{
    major = v;
}


Undergraduate::Undergraduate(string id, string name, string dept,
                             int maxNum, string major)
    : Student(id, name, dept, maxNum, major)
{
}

int Undergraduate::getType() const
{
    return 1;
}

string Undergraduate::getTypeName() const
{
    return "本科生";
}

int Undergraduate::getLevel() const
{
    return 1;
}

int Undergraduate::getBorrowDays() const
{
    return 30;
}


Graduate::Graduate(string id, string name, string dept,
                   int maxNum, string major)
    : Student(id, name, dept, maxNum, major)
{
}

int Graduate::getType() const
{
    return 2;
}

string Graduate::getTypeName() const
{
    return "研究生";
}

int Graduate::getLevel() const
{
    return 2;
}

int Graduate::getBorrowDays() const
{
    return 45;
}


Teacher::Teacher(string id, string name, string dept,
                 int maxNum, string title)
    : Person(id, name, dept, maxNum), title(title)
{
}

string Teacher::getTitle() const
{
    return title;
}

void Teacher::setTitle(string v)
{
    title = v;
}

int Teacher::getType() const
{
    return 3;
}

string Teacher::getTypeName() const
{
    return "教师";
}

int Teacher::getLevel() const
{
    return 3;
}

int Teacher::getBorrowDays() const
{
    return 60;
}

string Teacher::getExtraName() const
{
    return "职称";
}

string Teacher::getExtra() const
{
    return title;
}

void Teacher::setExtra(string v)
{
    title = v;
}
