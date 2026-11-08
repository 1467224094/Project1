#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include <vector>
#include "Person.h"
#include "Book.h"
#include "BorrowRecord.h"


struct Admin
{
    std::string user;  
    std::string pass;  
};


class Library
{
private:
    std::vector<Person*> persons;     
    std::vector<Book*> books;        
    std::vector<BorrowRecord> records;
    Admin admin;                     

public:
    Library();
    ~Library();

    void addPerson(Person* p);
    void addBook(Book* b);

    Person* findPerson(const std::string& id);
    Book* findBook(const std::string& id);


    bool checkAdmin(const std::string& user, const std::string& pass);
    bool checkPerson(const std::string& id, const std::string& pass);
    std::string getAdminUser() const;
    std::string getAdminPassword() const;
    void setAdminPassword(const std::string& p);

    bool removePerson(const std::string& id, std::string& msg);
    std::vector<Person*> searchPersons(const std::string& key) const;
    const std::vector<Person*>& allPersons() const;

    bool removeBook(const std::string& id, std::string& msg);
    std::vector<Book*> searchBooks(const std::string& key) const;
    const std::vector<Book*>& allBooks() const;

    std::string borrowBook(const std::string& personId, const std::string& bookId);
    std::string returnBook(const std::string& personId, const std::string& bookId);

    int activeCount(const std::string& personId) const;
    std::vector<BorrowRecord> recordsOf(const std::string& personId) const;
    const std::vector<BorrowRecord>& allRecords() const;

    void save() const;
    void load();

    static std::string today();
    static std::string addDays(const std::string& date, int days);
    static int daysBetween(const std::string& a, const std::string& b);  // b - a
};

#endif
