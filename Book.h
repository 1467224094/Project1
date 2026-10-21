#ifndef BOOK_H
#define BOOK_H

#include <string>

class Book {
public:
    std::string id;
    std::string title;
    std::string author;
    int totalCopies;
    int availableCopies;

    Book(std::string i, std::string t, std::string a, int total);

    std::string getInfo() const;
};

#endif 
