#ifndef BORROWRECORD_H
#define BORROWRECORD_H

#include <string>

struct BorrowRecord
{
    std::string personId;  
    std::string bookId;     
    std::string borrowDate;  
    std::string dueDate;    
    std::string returnDate;  
    bool returned;         
    BorrowRecord()
    {
        personId = "";
        bookId = "";
        borrowDate = "";
        dueDate = "";
        returnDate = "";
        returned = false;
    }
};

#endif
