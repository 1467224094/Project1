#pragma once

#include <string>
#include <vector>


struct Book {
    int id = 0;              
    std::string title;      
    std::string author;      
    std::string publisher;  
    double price = 0.0;      
    int total = 0;           
    int available = 0;       
}; 

class BookManager {
public:
  
    bool loadFromFile();
    bool saveToFile() const;


    void addBook();
    void displayBooks() const;
    void searchBook() const;
    void modifyBook();
    void deleteBook();
    void borrowBook();
    void returnBook();
    void menu();

private:
    std::vector<Book> books; 

 
    Book* find(int id);
    const Book* find(int id) const;


    static void printBook(const Book& book);
};
