#include "Book.h"

Book::Book(std::string i, std::string t, std::string a, int total)
    : id(i), title(t), author(a), totalCopies(total), availableCopies(total) {
}

std::string Book::getInfo() const {
    return "《" + title + "》 by " + author + " [ID:" + id + "]";
}