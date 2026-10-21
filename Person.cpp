#include "Person.h"
#include <algorithm>



Person::Person(std::string i, std::string n) : id(i), name(n) {}

Person::~Person() {}

std::string Person::getId() const { return id; }
std::string Person::getName() const { return name; }

bool Person::canBorrow() const {
    return borrowedBookIds.size() < static_cast<size_t>(getMaxBorrow());
}

bool Person::isBorrowed(const std::string& bookId) const {
    return std::find(borrowedBookIds.begin(), borrowedBookIds.end(), bookId) != borrowedBookIds.end();
}

void Person::addBorrowRecord(const std::string& bookId) {
    borrowedBookIds.push_back(bookId);
}

void Person::removeBorrowRecord(const std::string& bookId) {
    borrowedBookIds.erase(
        std::remove(borrowedBookIds.begin(), borrowedBookIds.end(), bookId),
        borrowedBookIds.end()
    );
}

const std::vector<std::string>& Person::getBorrowedBooks() const {
    return borrowedBookIds;
}

std::string Person::getDescription() const {
    return "Name: " + name + ", Type: " + getType() + ", Borrowed: " +
        std::to_string(borrowedBookIds.size()) + "/" + std::to_string(getMaxBorrow());
}


Student::Student(std::string i, std::string n) : Person(i, n) {}

int Student::getMaxBorrow() const {
    return 5;
}

std::string Student::getType() const {
    return "Student";
}


Teacher::Teacher(std::string i, std::string n) : Person(i, n) {}

int Teacher::getMaxBorrow() const {
    return 10;
}

std::string Teacher::getType() const {
    return "Teacher";
}

std::string Teacher::getDescription() const {
    return "Name: " + name + ", Type: " + getType() + " (Privileged), Borrowed: " +
        std::to_string(getBorrowedBooks().size()) + "/" + std::to_string(getMaxBorrow());
}