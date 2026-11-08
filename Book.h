#ifndef BOOK_H
#define BOOK_H

#include <string>

class Book
{
protected:
    std::string id;      
    std::string name;     
    std::string author;     
    std::string publisher;  
    int total;              
    int avail;            

public:
    Book(std::string id, std::string name, std::string author,
         std::string publisher, int total);
    virtual ~Book();

    std::string getId() const;
    std::string getName() const;
    std::string getAuthor() const;
    std::string getPublisher() const;
    int getTotal() const;
    int getAvail() const;

    void setName(std::string v);
    void setAuthor(std::string v);
    void setPublisher(std::string v);
    void setTotal(int n);
    void setAvail(int n);

    virtual int getType() const = 0;
    virtual std::string getTypeName() const = 0;

    virtual int getNeedLevel() const;
    virtual int getLoanDays() const;
    virtual std::string getExtraName() const;
    virtual std::string getExtra() const;
    virtual void setExtra(std::string v);

    virtual void show() const;

    bool borrowOne();
    bool returnOne();
};

class PaperBook : public Book
{
public:
    PaperBook(std::string id, std::string name, std::string author,
              std::string publisher, int total);

    int getType() const;
    std::string getTypeName() const;
};

class Journal : public Book
{
private:
    std::string issue;  

public:
    Journal(std::string id, std::string name, std::string author,
            std::string publisher, int total, std::string issue);

    int getType() const;
    std::string getTypeName() const;
    int getNeedLevel() const;
    int getLoanDays() const;
    std::string getExtraName() const;
    std::string getExtra() const;
    void setExtra(std::string v);

    std::string getIssue() const;
    void setIssue(std::string v);
};

class EBook : public Book
{
private:
    std::string url;   

public:
    EBook(std::string id, std::string name, std::string author,
          std::string publisher, int total, std::string url);

    int getType() const;
    std::string getTypeName() const;
    int getNeedLevel() const;
    int getLoanDays() const;
    std::string getExtraName() const;
    std::string getExtra() const;
    void setExtra(std::string v);

    std::string getUrl() const;
    void setUrl(std::string v);
};

#endif
