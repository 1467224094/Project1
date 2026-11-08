#include "Person.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

using namespace std;

Person::Person(string id, string name, string dept, int maxBorrow)
    : id(id), name(name), dept(dept), maxBorrow(maxBorrow) {}

string Person::getId() const { return id; }
string Person::getName() const { return name; }
string Person::getDept() const { return dept; }
int Person::getMaxBorrow() const { return maxBorrow; }
vector<string> Person::getBooks() const { return books; }

void Person::setName(string name) { this->name = name; }
void Person::setDept(string dept) { this->dept = dept; }

void Person::show() const {
    cout << left << setw(10) << id
         << setw(8) << getType()
         << setw(10) << name
         << setw(16) << dept
         << books.size() << "/" << maxBorrow << endl;
}

bool Person::canBorrow() const {
    return (int)books.size() < maxBorrow;
}

bool Person::hasBorrowed(string bookId) const {
    for (int i = 0; i < (int)books.size(); i++) {
        if (books[i] == bookId) return true;
    }
    return false;
}

void Person::borrowBook(string bookId) {
    books.push_back(bookId);
}

void Person::returnBook(string bookId) {
    for (int i = 0; i < (int)books.size(); i++) {
        if (books[i] == bookId) {
            books.erase(books.begin() + i);
            return;
        }
    }
}

Student::Student(string id, string name, string dept, int maxBorrow)
    : Person(id, name, dept, maxBorrow) {}

string Student::getType() const { return "学生"; }

Teacher::Teacher(string id, string name, string dept, int maxBorrow)
    : Person(id, name, dept, maxBorrow) {}

string Teacher::getType() const { return "教师"; }
