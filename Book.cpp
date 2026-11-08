#include "Book.h"
#include <iostream>

using namespace std;


Book::Book(string id, string name, string author,
           string publisher, int total)
    : id(id), name(name), author(author), publisher(publisher)
{
    if (total < 0) total = 0;
    this->total = total;
    this->avail = total;
}

Book::~Book()
{
}

string Book::getId() const
{
    return id;
}

string Book::getName() const
{
    return name;
}

string Book::getAuthor() const
{
    return author;
}

string Book::getPublisher() const
{
    return publisher;
}

int Book::getTotal() const
{
    return total;
}

int Book::getAvail() const
{
    return avail;
}

void Book::setName(string v)
{
    name = v;
}

void Book::setAuthor(string v)
{
    author = v;
}

void Book::setPublisher(string v)
{
    publisher = v;
}


void Book::setTotal(int n)
{
    if (n < 0) n = 0;
    int borrowed = total - avail;
    if (borrowed < 0) borrowed = 0;
    total = n;
    avail = total - borrowed;
    if (avail < 0) avail = 0;
}

void Book::setAvail(int n)
{
    if (n < 0) n = 0;
    if (n > total) n = total;
    avail = n;
}

int Book::getNeedLevel() const
{
    return 1;
}

int Book::getLoanDays() const
{
    return 30;
}

string Book::getExtraName() const
{
    return "";
}

string Book::getExtra() const
{
    return "";
}

void Book::setExtra(string)
{
}

void Book::show() const
{
    cout << id << " | " << getTypeName() << " | " << name
         << " | " << author << " | " << publisher
         << " | 总数 " << total << " 本，可借 " << avail << " 本";
    if (getExtra() != "")
    {
        cout << " | " << getExtraName() << ":" << getExtra();
    }
    cout << endl;
}

bool Book::borrowOne()
{
    if (avail <= 0) return false;
    avail = avail - 1;
    return true;
}

bool Book::returnOne()
{
    if (avail >= total) return false;
    avail = avail + 1;
    return true;
}


PaperBook::PaperBook(string id, string name, string author,
                     string publisher, int total)
    : Book(id, name, author, publisher, total)
{
}

int PaperBook::getType() const
{
    return 1;
}

string PaperBook::getTypeName() const
{
    return "图书";
}


Journal::Journal(string id, string name, string author,
                 string publisher, int total, string issue)
    : Book(id, name, author, publisher, total), issue(issue)
{
}

int Journal::getType() const
{
    return 2;
}

string Journal::getTypeName() const
{
    return "期刊";
}

int Journal::getNeedLevel() const
{
    return 2;
}

int Journal::getLoanDays() const
{
    return 15;
}

string Journal::getExtraName() const
{
    return "期号";
}

string Journal::getExtra() const
{
    return issue;
}

void Journal::setExtra(string v)
{
    issue = v;
}

string Journal::getIssue() const
{
    return issue;
}

void Journal::setIssue(string v)
{
    issue = v;
}


EBook::EBook(string id, string name, string author,
             string publisher, int total, string url)
    : Book(id, name, author, publisher, total), url(url)
{
}

int EBook::getType() const
{
    return 3;
}

string EBook::getTypeName() const
{
    return "电子资源";
}

int EBook::getNeedLevel() const
{
    return 1;
}

int EBook::getLoanDays() const
{
    return 7;
}

string EBook::getExtraName() const
{
    return "访问地址";
}

string EBook::getExtra() const
{
    return url;
}

void EBook::setExtra(string v)
{
    url = v;
}

string EBook::getUrl() const
{
    return url;
}

void EBook::setUrl(string v)
{
    url = v;
}
