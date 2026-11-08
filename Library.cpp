#include "Library.h"

#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

using namespace std;

Library::Library()
{
    admin.user = "admin";
    admin.pass = "123456";
}

Library::~Library()
{
    for (size_t i = 0; i < persons.size(); i++) delete persons[i];
    for (size_t i = 0; i < books.size(); i++) delete books[i];
}

void Library::addPerson(Person* p)
{
    persons.push_back(p);
}

void Library::addBook(Book* b)
{
    books.push_back(b);
}

const vector<Person*>& Library::allPersons() const
{
    return persons;
}

const vector<Book*>& Library::allBooks() const
{
    return books;
}

const vector<BorrowRecord>& Library::allRecords() const
{
    return records;
}

string Library::getAdminUser() const
{
    return admin.user;
}

string Library::getAdminPassword() const
{
    return admin.pass;
}

void Library::setAdminPassword(const string& p)
{
    admin.pass = p;
}


Person* Library::findPerson(const string& id)
{
    for (size_t i = 0; i < persons.size(); i++)
        if (persons[i]->getId() == id) return persons[i];
    return NULL;
}

Book* Library::findBook(const string& id)
{
    for (size_t i = 0; i < books.size(); i++)
        if (books[i]->getId() == id) return books[i];
    return NULL;
}


bool Library::checkAdmin(const string& user, const string& pass)
{
    return admin.user == user && admin.pass == pass;
}

bool Library::checkPerson(const string& id, const string& pass)
{
    Person* p = findPerson(id);
    if (p == NULL) return false;
    return p->getPassword() == pass;
}

string Library::today()
{
    time_t t = time(NULL);
    tm* lt = localtime(&t);
    char buf[16];
    sprintf(buf, "%04d-%02d-%02d", lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday);
    return string(buf);
}

static time_t toTime(const string& date)
{
    int y = 1970, m = 1, d = 1;
    sscanf(date.c_str(), "%d-%d-%d", &y, &m, &d);

    tm t;
    memset(&t, 0, sizeof(t));
    t.tm_year = y - 1900;
    t.tm_mon = m - 1;
    t.tm_mday = d;
    t.tm_hour = 12;
    return mktime(&t);
}

string Library::addDays(const string& date, int days)
{
    time_t t = toTime(date) + (time_t)days * 24 * 60 * 60;
    tm* lt = localtime(&t);
    char buf[16];
    sprintf(buf, "%04d-%02d-%02d", lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday);
    return string(buf);
}

int Library::daysBetween(const string& a, const string& b)
{
    double sec = difftime(toTime(b), toTime(a));
    return (int)(sec / (24 * 60 * 60) + (sec >= 0 ? 0.5 : -0.5));
}


bool Library::removePerson(const string& id, string& msg)
{
    if (activeCount(id) > 0)
    {
        msg = "该借阅人还有书没有归还，不能删除";
        return false;
    }
    for (size_t i = 0; i < persons.size(); i++)
    {
        if (persons[i]->getId() == id)
        {
            delete persons[i];
            persons.erase(persons.begin() + i);
            msg = "删除成功";
            return true;
        }
    }
    msg = "没有找到这个借阅人";
    return false;
}

vector<Person*> Library::searchPersons(const string& key) const
{
    vector<Person*> result;
    for (size_t i = 0; i < persons.size(); i++)
    {
        if (persons[i]->getId().find(key) != string::npos ||
            persons[i]->getName().find(key) != string::npos ||
            persons[i]->getDept().find(key) != string::npos)
        {
            result.push_back(persons[i]);
        }
    }
    return result;
}


bool Library::removeBook(const string& id, string& msg)
{
    for (size_t i = 0; i < records.size(); i++)
    {
        if (records[i].bookId == id && !records[i].returned)
        {
            msg = "这本书还有借阅记录没归还，不能删除";
            return false;
        }
    }
    for (size_t i = 0; i < books.size(); i++)
    {
        if (books[i]->getId() == id)
        {
            delete books[i];
            books.erase(books.begin() + i);
            msg = "删除成功";
            return true;
        }
    }
    msg = "没有找到这本书";
    return false;
}

vector<Book*> Library::searchBooks(const string& key) const
{
    vector<Book*> result;
    for (size_t i = 0; i < books.size(); i++)
    {
        if (books[i]->getId().find(key) != string::npos ||
            books[i]->getName().find(key) != string::npos ||
            books[i]->getAuthor().find(key) != string::npos ||
            books[i]->getPublisher().find(key) != string::npos)
        {
            result.push_back(books[i]);
        }
    }
    return result;
}


string Library::borrowBook(const string& personId, const string& bookId)
{
    Person* p = findPerson(personId);
    if (p == NULL) return "借阅失败：没有找到编号为 " + personId + " 的借阅人";

    Book* b = findBook(bookId);
    if (b == NULL) return "借阅失败：没有找到编号为 " + bookId + " 的图书";

    if (b->getAvail() <= 0) return "借阅失败：《" + b->getName() + "》已经全部借出去了";

    if (p->isFull())
    {
        ostringstream os;
        os << "借阅失败：已经借了 " << p->getBorrowedNum()
           << " 本，达到最大借阅数量 " << p->getMaxNum() << " 本";
        return os.str();
    }

    if (!p->canBorrow(b))
    {
        return "借阅失败：" + p->getTypeName() + "不能借阅" + b->getTypeName() +
               "（该资源需要权限等级 " + to_string(b->getNeedLevel()) + "）";
    }

    for (size_t i = 0; i < records.size(); i++)
    {
        if (!records[i].returned && records[i].personId == personId && records[i].bookId == bookId)
            return "借阅失败：这本书已经在手上，还没有归还";
    }

    b->borrowOne();
    p->setBorrowedNum(p->getBorrowedNum() + 1);

    BorrowRecord r;
    r.personId = personId;
    r.bookId = bookId;
    r.borrowDate = today();
    int days = p->getBorrowDays();
    if (b->getLoanDays() < days) days = b->getLoanDays();
    r.dueDate = addDays(r.borrowDate, days);
    r.returnDate = "";
    r.returned = false;
    records.push_back(r);

    return "借阅成功：应还日期 " + r.dueDate + "（可借 " + to_string(days) + " 天）";
}



string Library::returnBook(const string& personId, const string& bookId)
{
    Person* p = findPerson(personId);
    if (p == NULL) return "归还失败：没有找到编号为 " + personId + " 的借阅人";

    Book* b = findBook(bookId);
    if (b == NULL) return "归还失败：没有找到编号为 " + bookId + " 的图书";

    for (size_t i = 0; i < records.size(); i++)
    {
        if (!records[i].returned && records[i].personId == personId && records[i].bookId == bookId)
        {
            records[i].returned = true;
            records[i].returnDate = today();
            b->returnOne();
            if (p->getBorrowedNum() > 0) p->setBorrowedNum(p->getBorrowedNum() - 1);

            string msg = "归还成功：应还日期 " + records[i].dueDate +
                         "，实际归还 " + records[i].returnDate;
            int late = daysBetween(records[i].dueDate, records[i].returnDate);
            if (late > 0) msg += "，已经超期 " + to_string(late) + " 天";
            return msg;
        }
    }
    return "归还失败：没有找到这本书的借阅记录";
}


int Library::activeCount(const string& personId) const
{
    int count = 0;
    for (size_t i = 0; i < records.size(); i++)
        if (!records[i].returned && records[i].personId == personId) count++;
    return count;
}

vector<BorrowRecord> Library::recordsOf(const string& personId) const
{
    vector<BorrowRecord> result;
    for (size_t i = 0; i < records.size(); i++)
        if (records[i].personId == personId) result.push_back(records[i]);
    return result;
}


void Library::save() const
{
    ofstream fp("persons.txt");
    for (size_t i = 0; i < persons.size(); i++)
    {
        Person* p = persons[i];
        fp << p->getType() << '\t' << p->getId() << '\t' << p->getName() << '\t'
           << p->getDept() << '\t' << p->getMaxNum() << '\t' << p->getBorrowedNum()
           << '\t' << p->getExtra() << '\t' << p->getPassword() << '\n';
    }
    fp.close();

    fp.open("books.txt");
    for (size_t i = 0; i < books.size(); i++)
    {
        Book* b = books[i];
        fp << b->getType() << '\t' << b->getId() << '\t' << b->getName() << '\t'
           << b->getAuthor() << '\t' << b->getPublisher() << '\t' << b->getTotal()
           << '\t' << b->getAvail() << '\t' << b->getExtra() << '\n';
    }
    fp.close();

    fp.open("records.txt");
    for (size_t i = 0; i < records.size(); i++)
    {
        fp << records[i].personId << '\t' << records[i].bookId << '\t'
           << records[i].borrowDate << '\t' << records[i].dueDate << '\t'
           << records[i].returnDate << '\t' << (records[i].returned ? 1 : 0) << '\n';
    }
    fp.close();

    fp.open("admin.txt");
    fp << admin.user << '\t' << admin.pass << '\n';
    fp.close();
}


static vector<string> splitLine(const string& line)
{
    vector<string> fields;
    string cur;
    for (size_t i = 0; i < line.size(); i++)
    {
        if (line[i] == '\t')
        {
            fields.push_back(cur);
            cur = "";
        }
        else cur += line[i];
    }
    fields.push_back(cur);
    return fields;
}

void Library::load()
{
    ifstream af("admin.txt");
    string adminLine;
    if (getline(af, adminLine))
    {
        if (adminLine.size() > 0 && adminLine[adminLine.size() - 1] == '\r')
            adminLine.erase(adminLine.size() - 1);
        vector<string> f = splitLine(adminLine);
        if (f.size() >= 2 && f[0] != "")
        {
            admin.user = f[0];
            admin.pass = f[1];
        }
    }
    af.close();

    ifstream fp("persons.txt");
    string line;
    while (getline(fp, line))
    {
        if (line.size() > 0 && line[line.size() - 1] == '\r') line.erase(line.size() - 1);
        if (line == "") continue;

        vector<string> f = splitLine(line);
        if (f.size() < 7) continue;

        int type = atoi(f[0].c_str());
        int maxNum = atoi(f[4].c_str());
        int borrowed = atoi(f[5].c_str());
        string pw = (f.size() >= 8 ? f[7] : "123456");

        Person* p = NULL;
        if (type == 1) p = new Undergraduate(f[1], f[2], f[3], maxNum, f[6]);
        else if (type == 2) p = new Graduate(f[1], f[2], f[3], maxNum, f[6]);
        else if (type == 3) p = new Teacher(f[1], f[2], f[3], maxNum, f[6]);

        if (p != NULL)
        {
            p->setBorrowedNum(borrowed);
            p->setPassword(pw);
            persons.push_back(p);
        }
    }
    fp.close();

    fp.open("books.txt");
    while (getline(fp, line))
    {
        if (line.size() > 0 && line[line.size() - 1] == '\r') line.erase(line.size() - 1);
        if (line == "") continue;

        vector<string> f = splitLine(line);
        if (f.size() < 8) continue;

        int type = atoi(f[0].c_str());
        int total = atoi(f[5].c_str());
        int avail = atoi(f[6].c_str());

        Book* b = NULL;
        if (type == 1) b = new PaperBook(f[1], f[2], f[3], f[4], total);
        else if (type == 2) b = new Journal(f[1], f[2], f[3], f[4], total, f[7]);
        else if (type == 3) b = new EBook(f[1], f[2], f[3], f[4], total, f[7]);

        if (b != NULL)
        {
            b->setAvail(avail);
            books.push_back(b);
        }
    }
    fp.close();

    fp.open("records.txt");
    while (getline(fp, line))
    {
        if (line.size() > 0 && line[line.size() - 1] == '\r') line.erase(line.size() - 1);
        if (line == "") continue;

        vector<string> f = splitLine(line);
        if (f.size() < 6) continue;

        BorrowRecord r;
        r.personId = f[0];
        r.bookId = f[1];
        r.borrowDate = f[2];
        r.dueDate = f[3];
        r.returnDate = f[4];
        r.returned = (f[5] == "1");
        records.push_back(r);
    }
    fp.close();
}
