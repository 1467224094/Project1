#pragma once
#ifndef LIBRARYSYSTEM_H
#define LIBRARYSYSTEM_H

#include <vector>
#include <string>
#include "Book.h"
#include "Person.h"

class LibrarySystem {
private:
    std::vector<Book> books;
    std::vector<Person*> readers; 

    Book* findBook(const std::string& id);
    Person* findReader(const std::string& id);

    void clearInputBuffer();


    void readerMenu();
    void bookMenu();
    void borrowMenu();
    void displayAllReaders();

public:
    LibrarySystem();
    ~LibrarySystem();

    void run();
};

#endif 