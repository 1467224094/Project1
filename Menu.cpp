#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cctype>

#include "Library.h"
#include "Menu.h"


using namespace std;

bool readLine(string& s)
{
    if (!getline(cin, s)) return false;
    while (!s.empty() && (s[s.size() - 1] == '\r' || s[s.size() - 1] == '\n'))
        s.erase(s.size() - 1);
    return true;
}

bool parseInt(const string& s, int& v)
{
    string t = s;
    while (!t.empty() && (t[0] == ' ' || t[0] == '\t')) t.erase(0, 1);
    while (!t.empty() && (t[t.size() - 1] == ' ' || t[t.size() - 1] == '\t')) t.erase(t.size() - 1);

    if (t.empty()) return false;
    for (size_t i = 0; i < t.size(); i++)
        if (!isdigit((unsigned char)t[i])) return false;
    v = atoi(t.c_str());
    return true;
}

int inputInt(const string& tip)
{
    while (true)
    {
        cout << tip;
        string s;
        if (!readLine(s)) return -1;
        int v;
        if (parseInt(s, v)) return v;
        cout << "  输入有误，请输入一个整数！" << endl;
    }
}

int inputIntDefault(const string& tip, int def)
{
    while (true)
    {
        cout << tip;
        string s;
        if (!readLine(s)) return def;
        if (s.empty()) return def;
        int v;
        if (parseInt(s, v)) return v;
        cout << "  输入有误，请重新输入！" << endl;
    }
}

string inputString(const string& tip)
{
    while (true)
    {
        cout << tip;
        string s;
        if (!readLine(s)) return "";
        if (!s.empty()) return s;
        cout << "  不能为空，请重新输入！" << endl;
    }
}

string inputKeepString(const string& tip, const string& old)
{
    cout << tip << "（当前：" << old << "，回车不修改）：";
    string s;
    if (!readLine(s)) return old;
    if (s.empty()) return old;
    return s;
}

int inputKeepInt(const string& tip, int old)
{
    while (true)
    {
        cout << tip << "（当前：" << old << "，回车不修改）：";
        string s;
        if (!readLine(s)) return old;
        if (s.empty()) return old;
        int v;
        if (parseInt(s, v)) return v;
        cout << "  输入有误，请重新输入！" << endl;
    }
}


void personHeader()
{
    cout << "编号 | 姓名 | 院系 | 类型 | 借阅情况 | 专业/职称" << endl;
    cout << "------------------------------------------------------------" << endl;
}

void bookHeader()
{
    cout << "编号 | 类型 | 书名 | 作者 | 出版社 | 数量 | 附加信息" << endl;
    cout << "------------------------------------------------------------" << endl;
}

void recordHeader()
{
    cout << "借阅人 | 图书 | 借出日期 | 应还日期 | 状态" << endl;
    cout << "------------------------------------------------------------" << endl;
}


void addPerson(Library& lib)
{
    cout << "  1. 本科生   2. 研究生   3. 教师" << endl;
    int type = inputInt("请选择类型：");
    if (type < 0) return;
    if (type < 1 || type > 3)
    {
        cout << "  没有这个类型！" << endl;
        return;
    }

    string id = inputString("学号/工号：");
    if (id == "") return;
    if (lib.findPerson(id) != NULL)
    {
        cout << "  这个编号已经存在了！" << endl;
        return;
    }

    string name = inputString("姓名：");
    string dept = inputString("院系：");

    int defMax = 5;
    if (type == 2) defMax = 8;
    if (type == 3) defMax = 15;
    int maxNum = inputIntDefault("最大借阅数量（直接回车默认为 " + to_string(defMax) + "）：", defMax);

    string extra;
    if (type == 3) extra = inputString("职称：");
    else extra = inputString("专业：");

    Person* p = NULL;
    if (type == 1) p = new Undergraduate(id, name, dept, maxNum, extra);
    else if (type == 2) p = new Graduate(id, name, dept, maxNum, extra);
    else p = new Teacher(id, name, dept, maxNum, extra);

    lib.addPerson(p);
    cout << "  添加借阅人成功！" << endl;
}

void editPerson(Library& lib)
{
    string id = inputString("请输入要修改的借阅人编号：");
    if (id == "") return;

    Person* p = lib.findPerson(id);
    if (p == NULL)
    {
        cout << "  没有找到这个借阅人！" << endl;
        return;
    }

    cout << "  当前信息：" << endl;
    p->show();
    cout << "  （括号里是原来的值，直接回车表示不修改）" << endl;

    p->setName(inputKeepString("姓名", p->getName()));
    p->setDept(inputKeepString("院系", p->getDept()));
    p->setMaxNum(inputKeepInt("最大借阅数量", p->getMaxNum()));
    p->setExtra(inputKeepString(p->getExtraName(), p->getExtra()));

    cout << "  修改成功！" << endl;
}

void deletePerson(Library& lib)
{
    string id = inputString("请输入要删除的借阅人编号：");
    if (id == "") return;

    Person* p = lib.findPerson(id);
    if (p == NULL)
    {
        cout << "  没有找到这个借阅人！" << endl;
        return;
    }

    cout << "  要删除的是：";
    p->show();

    string ok = inputString("确定删除吗？（输入 y 删除，其他键取消）：");
    if (ok != "y" && ok != "Y")
    {
        cout << "  已经取消。" << endl;
        return;
    }

    string msg;
    lib.removePerson(id, msg);
    cout << "  " << msg << endl;
}

void listPersons(Library& lib)
{
    const vector<Person*>& all = lib.allPersons();
    if (all.empty())
    {
        cout << "  还没有借阅人信息。" << endl;
        return;
    }
    personHeader();
    for (size_t i = 0; i < all.size(); i++) all[i]->show();
    cout << "  共 " << all.size() << " 个借阅人。" << endl;
}

void searchPersonMenu(Library& lib)
{
    string key = inputString("请输入学号、姓名或院系中的关键字：");
    if (key == "") return;

    vector<Person*> result = lib.searchPersons(key);
    if (result.empty())
    {
        cout << "  没有找到符合条件的借阅人。" << endl;
        return;
    }
    personHeader();
    for (size_t i = 0; i < result.size(); i++) result[i]->show();
    cout << "  共找到 " << result.size() << " 条记录。" << endl;
}

void personMenu(Library& lib)
{
    while (true)
    {
        cout << endl;
        cout << "------ 借阅人管理 ------" << endl;
        cout << "  1. 添加借阅人" << endl;
        cout << "  2. 修改借阅人" << endl;
        cout << "  3. 删除借阅人" << endl;
        cout << "  4. 显示全部借阅人" << endl;
        cout << "  5. 查询借阅人" << endl;
        cout << "  0. 返回上一级" << endl;

        int c = inputInt("请选择：");
        if (c <= 0) return;

        switch (c)
        {
        case 1: addPerson(lib); break;
        case 2: editPerson(lib); break;
        case 3: deletePerson(lib); break;
        case 4: listPersons(lib); break;
        case 5: searchPersonMenu(lib); break;
        default: cout << "  没有这个选项！" << endl;
        }
    }
}


void addBook(Library& lib)
{
    cout << "  1. 图书   2. 期刊   3. 电子资源" << endl;
    int type = inputInt("请选择类型：");
    if (type < 0) return;
    if (type < 1 || type > 3)
    {
        cout << "  没有这个类型！" << endl;
        return;
    }

    string id = inputString("图书编号：");
    if (id == "") return;
    if (lib.findBook(id) != NULL)
    {
        cout << "  这个编号已经存在了！" << endl;
        return;
    }

    string name = inputString("书名：");
    string author = inputString("作者：");
    string publisher = inputString("出版社：");
    int total = inputIntDefault("数量（直接回车默认为 1）：", 1);
    if (total <= 0) total = 1;

    string extra;
    if (type == 2) extra = inputString("期号：");
    if (type == 3) extra = inputString("访问地址：");

    Book* b = NULL;
    if (type == 1) b = new PaperBook(id, name, author, publisher, total);
    else if (type == 2) b = new Journal(id, name, author, publisher, total, extra);
    else b = new EBook(id, name, author, publisher, total, extra);

    lib.addBook(b);
    cout << "  添加图书成功！" << endl;
}

void editBook(Library& lib)
{
    string id = inputString("请输入要修改的图书编号：");
    if (id == "") return;

    Book* b = lib.findBook(id);
    if (b == NULL)
    {
        cout << "  没有找到这本书！" << endl;
        return;
    }

    cout << "  当前信息：" << endl;
    b->show();
    cout << "  （括号里是原来的值，直接回车表示不修改）" << endl;

    b->setName(inputKeepString("书名", b->getName()));
    b->setAuthor(inputKeepString("作者", b->getAuthor()));
    b->setPublisher(inputKeepString("出版社", b->getPublisher()));

    int newTotal = inputKeepInt("数量", b->getTotal());
    int borrowed = b->getTotal() - b->getAvail();
    if (newTotal < borrowed)
    {
        cout << "  现在还有 " << borrowed << " 本没有还回来，数量不能小于这个数！" << endl;
    }
    else
    {
        b->setTotal(newTotal);
    }

    if (b->getExtra() != "" || b->getExtraName() != "")
        b->setExtra(inputKeepString(b->getExtraName(), b->getExtra()));

    cout << "  修改成功！" << endl;
}

void deleteBook(Library& lib)
{
    string id = inputString("请输入要删除的图书编号：");
    if (id == "") return;

    Book* b = lib.findBook(id);
    if (b == NULL)
    {
        cout << "  没有找到这本书！" << endl;
        return;
    }

    cout << "  要删除的是：";
    b->show();

    string ok = inputString("确定删除吗？（输入 y 删除，其他键取消）：");
    if (ok != "y" && ok != "Y")
    {
        cout << "  已经取消。" << endl;
        return;
    }

    string msg;
    lib.removeBook(id, msg);
    cout << "  " << msg << endl;
}

void listBooks(Library& lib)
{
    const vector<Book*>& all = lib.allBooks();
    if (all.empty())
    {
        cout << "  还没有图书信息。" << endl;
        return;
    }
    bookHeader();
    for (size_t i = 0; i < all.size(); i++) all[i]->show();
    cout << "  共 " << all.size() << " 种资源。" << endl;
}

void searchBookMenu(Library& lib)
{
    string key = inputString("请输入图书编号、书名、作者或出版社中的关键字：");
    if (key == "") return;

    vector<Book*> result = lib.searchBooks(key);
    if (result.empty())
    {
        cout << "  没有找到符合条件的图书。" << endl;
        return;
    }
    bookHeader();
    for (size_t i = 0; i < result.size(); i++) result[i]->show();
    cout << "  共找到 " << result.size() << " 条记录。" << endl;
}

void bookMenu(Library& lib)
{
    while (true)
    {
        cout << endl;
        cout << "------ 图书资源管理 ------" << endl;
        cout << "  1. 添加图书/资源" << endl;
        cout << "  2. 修改图书/资源" << endl;
        cout << "  3. 删除图书/资源" << endl;
        cout << "  4. 显示全部图书/资源" << endl;
        cout << "  5. 查询图书/资源" << endl;
        cout << "  0. 返回上一级" << endl;

        int c = inputInt("请选择：");
        if (c <= 0) return;

        switch (c)
        {
        case 1: addBook(lib); break;
        case 2: editBook(lib); break;
        case 3: deleteBook(lib); break;
        case 4: listBooks(lib); break;
        case 5: searchBookMenu(lib); break;
        default: cout << "  没有这个选项！" << endl;
        }
    }
}

void showRecords(Library& lib, const vector<BorrowRecord>& list)
{
    if (list.empty())
    {
        cout << "  没有借阅记录。" << endl;
        return;
    }
    recordHeader();
    for (size_t i = 0; i < list.size(); i++)
    {
        const BorrowRecord& r = list[i];
        Person* p = lib.findPerson(r.personId);
        Book* b = lib.findBook(r.bookId);

        string pname = (p == NULL ? "(已删除)" : p->getName());
        string bname = (b == NULL ? "(已删除)" : b->getName());

        cout << r.personId << " " << pname << " | "
             << r.bookId << " " << bname << " | "
             << "借出 " << r.borrowDate << " | "
             << "应还 " << r.dueDate << " | ";

        if (r.returned)
        {
            cout << "已归还（" << r.returnDate << "）";
        }
        else
        {
            int late = Library::daysBetween(r.dueDate, Library::today());
            if (late > 0) cout << "未归还，超期 " << late << " 天";
            else cout << "未归还";
        }
        cout << endl;
    }
}

void doBorrow(Library& lib, const string& personId = "")
{
    string pid = personId;
    if (pid == "")
    {
        pid = inputString("借阅人编号：");
        if (pid == "") return;
    }
    string bookId = inputString("图书编号：");
    if (bookId == "") return;

    cout << "  " << lib.borrowBook(pid, bookId) << endl;
}

void doReturn(Library& lib, const string& personId = "")
{
    string pid = personId;
    if (pid == "")
    {
        pid = inputString("借阅人编号：");
        if (pid == "") return;
    }
    string bookId = inputString("图书编号：");
    if (bookId == "") return;

    cout << "  " << lib.returnBook(pid, bookId) << endl;
}

void showPersonRecords(Library& lib, const string& personId = "")
{
    string id = personId;
    if (id == "")
    {
        id = inputString("请输入借阅人编号：");
        if (id == "") return;
    }

    Person* p = lib.findPerson(id);
    if (p == NULL)
    {
        cout << "  没有找到这个借阅人！" << endl;
        return;
    }

    cout << "  " << p->getName() << "（" << p->getTypeName() << "）的借阅记录："
         << "在借 " << p->getBorrowedNum() << " 本，最多可以借 " << p->getMaxNum() << " 本" << endl;
    showRecords(lib, lib.recordsOf(id));
}

void borrowMenu(Library& lib)
{
    while (true)
    {
        cout << endl;
        cout << "------ 借阅与归还 ------" << endl;
        cout << "  1. 借书" << endl;
        cout << "  2. 还书" << endl;
        cout << "  3. 查看某个借阅人的记录" << endl;
        cout << "  4. 查看全部借阅记录" << endl;
        cout << "  0. 返回上一级" << endl;

        int c = inputInt("请选择：");
        if (c <= 0) return;

        switch (c)
        {
        case 1: doBorrow(lib); break;
        case 2: doReturn(lib); break;
        case 3: showPersonRecords(lib); break;
        case 4: showRecords(lib, lib.allRecords()); break;
        default: cout << "  没有这个选项！" << endl;
        }
    }
}


void showOverdue(Library& lib)
{
    vector<BorrowRecord> list = lib.allRecords();
    bool found = false;

    recordHeader();
    for (size_t i = 0; i < list.size(); i++)
    {
        if (list[i].returned) continue;
        int late = Library::daysBetween(list[i].dueDate, Library::today());
        if (late <= 0) continue;

        found = true;
        Person* p = lib.findPerson(list[i].personId);
        Book* b = lib.findBook(list[i].bookId);
        cout << list[i].personId << " "
             << (p == NULL ? "(已删除)" : p->getName()) << " | "
             << list[i].bookId << " "
             << (b == NULL ? "(已删除)" : b->getName()) << " | "
             << "应还 " << list[i].dueDate << " | 超期 " << late << " 天" << endl;
    }

    if (!found)
    {
        cout << "  现在没有超期的记录。" << endl;
    }
}

void showStat(Library& lib)
{
    int undergraduate = 0, graduate = 0, teacher = 0;
    const vector<Person*>& ps = lib.allPersons();
    for (size_t i = 0; i < ps.size(); i++)
    {
        if (ps[i]->getType() == 1) undergraduate++;
        else if (ps[i]->getType() == 2) graduate++;
        else teacher++;
    }

    int kinds = 0;
    int totalBooks = 0;
    int availBooks = 0;
    const vector<Book*>& bs = lib.allBooks();
    for (size_t i = 0; i < bs.size(); i++)
    {
        kinds++;
        totalBooks += bs[i]->getTotal();
        availBooks += bs[i]->getAvail();
    }

    int recordCount = 0;
    int active = 0;
    int overdue = 0;
    const vector<BorrowRecord>& rs = lib.allRecords();
    for (size_t i = 0; i < rs.size(); i++)
    {
        recordCount++;
        if (!rs[i].returned)
        {
            active++;
            if (Library::daysBetween(rs[i].dueDate, Library::today()) > 0) overdue++;
        }
    }

    cout << "  ---------- 统计信息 ----------" << endl;
    cout << "  本科生人数：" << undergraduate << endl;
    cout << "  研究生人数：" << graduate << endl;
    cout << "  教师人数：  " << teacher << endl;
    cout << "  借阅人合计：" << ps.size() << endl;
    cout << "  资源种类：  " << kinds << endl;
    cout << "  资源总册数：" << totalBooks << endl;
    cout << "  可借册数：  " << availBooks << endl;
    cout << "  借阅记录数：" << recordCount << endl;
    cout << "  未归还：    " << active << endl;
    cout << "  已超期：    " << overdue << endl;
}

void queryMenu(Library& lib)
{
    while (true)
    {
        cout << endl;
        cout << "------ 查询与统计 ------" << endl;
        cout << "  1. 按关键字查询图书" << endl;
        cout << "  2. 按关键字查询借阅人" << endl;
        cout << "  3. 查看超期未还的记录" << endl;
        cout << "  4. 统计信息" << endl;
        cout << "  0. 返回上一级" << endl;

        int c = inputInt("请选择：");
        if (c <= 0) return;

        switch (c)
        {
        case 1: searchBookMenu(lib); break;
        case 2: searchPersonMenu(lib); break;
        case 3: showOverdue(lib); break;
        case 4: showStat(lib); break;
        default: cout << "  没有这个选项！" << endl;
        }
    }
}

bool adminLogin(Library& lib)
{
    for (int i = 0; i < 3; i++)
    {
        string user = inputString("管理员账号：");
        if (user == "") return false;
        string pass = inputString("密码：");
        if (pass == "") return false;

        if (lib.checkAdmin(user, pass))
        {
            cout << "  登录成功！" << endl;
            return true;
        }
        cout << "  账号或密码不对！";
        if (i < 2) cout << "还能再试 " << (2 - i) << " 次";
        cout << endl;
    }
    cout << "  登录失败，回到登录界面。" << endl;
    return false;
}

Person* personLogin(Library& lib)
{
    for (int i = 0; i < 3; i++)
    {
        string id = inputString("学号/工号：");
        if (id == "") return NULL;
        string pass = inputString("密码：");
        if (pass == "") return NULL;

        if (lib.checkPerson(id, pass))
        {
            Person* p = lib.findPerson(id);
            cout << "  " << p->getName() << " 你好，登录成功！" << endl;
            return p;
        }
        cout << "  编号或密码不对！";
        if (i < 2) cout << "还能再试 " << (2 - i) << " 次";
        cout << endl;
    }
    cout << "  登录失败，回到登录界面。" << endl;
    return NULL;
}

void changeAdminPassword(Library& lib)
{
    string oldPass = inputString("原密码：");
    if (oldPass == "") return;
    if (oldPass != lib.getAdminPassword())
    {
        cout << "  原密码不对！" << endl;
        return;
    }

    string newPass = inputString("新密码（至少 4 位）：");
    if (newPass.size() < 4)
    {
        cout << "  密码太短了！" << endl;
        return;
    }
    string again = inputString("再输一次：");
    if (again != newPass)
    {
        cout << "  两次输入的不一样！" << endl;
        return;
    }

    lib.setAdminPassword(newPass);
    cout << "  密码修改成功！" << endl;
}

void changePersonPassword(Person* p)
{
    string oldPass = inputString("原密码：");
    if (oldPass == "") return;
    if (oldPass != p->getPassword())
    {
        cout << "  原密码不对！" << endl;
        return;
    }

    string newPass = inputString("新密码（至少 4 位）：");
    if (newPass.size() < 4)
    {
        cout << "  密码太短了！" << endl;
        return;
    }
    string again = inputString("再输一次：");
    if (again != newPass)
    {
        cout << "  两次输入的不一样！" << endl;
        return;
    }

    p->setPassword(newPass);
    cout << "  密码修改成功！" << endl;
}

void adminMenu(Library& lib)
{
    while (true)
    {
        cout << endl;
        cout << "========== 管理员菜单 ==========" << endl;
        cout << "  1. 借阅人管理" << endl;
        cout << "  2. 图书资源管理" << endl;
        cout << "  3. 借阅与归还" << endl;
        cout << "  4. 查询与统计" << endl;
        cout << "  5. 修改密码" << endl;
        cout << "  0. 注销（回到登录界面）" << endl;
        cout << "================================" << endl;

        int c = inputInt("请选择：");
        if (c <= 0) return;

        switch (c)
        {
        case 1: personMenu(lib); break;
        case 2: bookMenu(lib); break;
        case 3: borrowMenu(lib); break;
        case 4: queryMenu(lib); break;
        case 5: changeAdminPassword(lib); break;
        default: cout << "  没有这个选项！" << endl;
        }
    }
}

void readerMenu(Library& lib, Person* p)
{
    while (true)
    {
        cout << endl;
        cout << "========== 借阅人菜单 ==========" << endl;
        cout << "  1. 查看我的信息" << endl;
        cout << "  2. 借书" << endl;
        cout << "  3. 还书" << endl;
        cout << "  4. 查看我的借阅记录" << endl;
        cout << "  5. 查询图书" << endl;
        cout << "  6. 修改密码" << endl;
        cout << "  0. 注销（回到登录界面）" << endl;
        cout << "================================" << endl;

        int c = inputInt("请选择：");
        if (c <= 0) return;

        switch (c)
        {
        case 1:
            personHeader();
            p->show();
            break;
        case 2: doBorrow(lib, p->getId()); break;
        case 3: doReturn(lib, p->getId()); break;
        case 4: showPersonRecords(lib, p->getId()); break;
        case 5: searchBookMenu(lib); break;
        case 6: changePersonPassword(p); break;
        default: cout << "  没有这个选项！" << endl;
        }
    }
}


void runSystem(Library& lib)
{
    cout << "欢迎使用图书管理系统！" << endl;

    while (true)
    {
        cout << endl;
        cout << "========== 图书管理系统 ==========" << endl;
        cout << "  1. 管理员登录" << endl;
        cout << "  2. 借阅人登录" << endl;
        cout << "  0. 保存并退出" << endl;
        cout << "==================================" << endl;

        int c = inputInt("请选择：");
        if (c <= 0) break;

        if (c == 1)
        {
            if (adminLogin(lib)) adminMenu(lib);
        }
        else if (c == 2)
        {
            Person* p = personLogin(lib);
            if (p != NULL) readerMenu(lib, p);
        }
        else
        {
            cout << "  没有这个选项！" << endl;
        }

        lib.save();
    }
}
