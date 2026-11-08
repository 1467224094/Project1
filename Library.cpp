#include "Library.h"

#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

// 数据保存的文件名
static const char* DATA_FILE = "library_data.txt";

// 读取一行文字（可以带空格）
static string readLine(const string& tip) {
    cout << tip;
    string s;
    getline(cin, s);
    return s;
}

// 读取一个整数，输入有误时返回 0
static int readInt(const string& tip) {
    string line = readLine(tip);
    stringstream ss(line);
    int value = 0;
    ss >> value;
    return value;
}

// 把字符串按分隔符拆开
static vector<string> split(const string& s, char ch) {
    vector<string> result;
    string cur;
    for (int i = 0; i < (int)s.size(); i++) {
        if (s[i] == ch) {
            result.push_back(cur);
            cur.clear();
        } else {
            cur += s[i];
        }
    }
    result.push_back(cur);
    return result;
}

Library::Library() {
    username = "admin";
    password = "123456";

    // 如果数据文件存在就读取，否则放几本图书和两个读者方便演示
    if (!loadData()) {
        books.push_back(Book("B001", "C++ Primer", "Lippman", "人民邮电出版社", 5));
        books.push_back(Book("B002", "数据结构", "严蔚敏", "清华大学出版社", 3));
        books.push_back(Book("B003", "算法导论", "Cormen", "机械工业出版社", 2));

        readers.push_back(new Student("S001", "张三", "计算机学院"));
        readers.push_back(new Teacher("T001", "李老师", "数学学院"));

        saveData();
    }
}

Library::~Library() {
    saveData();
    for (int i = 0; i < (int)readers.size(); i++) {
        delete readers[i];
    }
}

// ---------------- 数据保存与读取 ----------------
void Library::saveData() {
    ofstream out(DATA_FILE);
    if (!out) return;

    out << "ADMIN|" << username << "|" << password << "\n";

    for (int i = 0; i < (int)books.size(); i++) {
        Book& b = books[i];
        out << "BOOK|" << b.id << "|" << b.title << "|" << b.author << "|"
            << b.publisher << "|" << b.total << "|" << b.available << "\n";
    }

    for (int i = 0; i < (int)readers.size(); i++) {
        Person* p = readers[i];
        out << "READER|" << (p->getType() == "学生" ? 1 : 2) << "|"
            << p->getId() << "|" << p->getName() << "|" << p->getDept() << "|"
            << p->getMaxBorrow() << "|";
        vector<string> list = p->getBooks();
        for (int j = 0; j < (int)list.size(); j++) {
            if (j > 0) out << ",";
            out << list[j];
        }
        out << "\n";
    }
}

bool Library::loadData() {
    ifstream in(DATA_FILE);
    if (!in) return false;

    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        vector<string> f = split(line, '|');
        if (f.size() < 2) continue;

        if (f[0] == "ADMIN" && f.size() >= 3) {
            username = f[1];
            password = f[2];
        } else if (f[0] == "BOOK" && f.size() >= 7) {
            Book b(f[1], f[2], f[3], f[4], atoi(f[5].c_str()));
            b.available = atoi(f[6].c_str());
            books.push_back(b);
        } else if (f[0] == "READER" && f.size() >= 7) {
            int type = atoi(f[1].c_str());
            int maxBorrow = atoi(f[5].c_str());
            Person* p = NULL;
            if (type == 1) p = new Student(f[2], f[3], f[4], maxBorrow);
            else p = new Teacher(f[2], f[3], f[4], maxBorrow);

            if (!f[6].empty()) {
                vector<string> list = split(f[6], ',');
                for (int j = 0; j < (int)list.size(); j++) {
                    p->borrowBook(list[j]);
                }
            }
            readers.push_back(p);
        }
    }
    return (books.size() > 0 || readers.size() > 0);
}

// ---------------- 查找 ----------------
Book* Library::findBook(string id) {
    for (int i = 0; i < (int)books.size(); i++) {
        if (books[i].id == id) return &books[i];
    }
    return NULL;
}

Person* Library::findReader(string id) {
    for (int i = 0; i < (int)readers.size(); i++) {
        if (readers[i]->getId() == id) return readers[i];
    }
    return NULL;
}

int Library::borrowedCount(string bookId) const {
    int count = 0;
    for (int i = 0; i < (int)readers.size(); i++) {
        if (readers[i]->hasBorrowed(bookId)) count++;
    }
    return count;
}

// ---------------- 登录 ----------------
bool Library::login() {
    cout << "\n========== 系统登录 ==========\n";
    cout << "默认账号：admin    默认密码：123456\n";

    for (int i = 0; i < 3; i++) {
        string user = readLine("账号: ");
        string pwd = readLine("密码: ");
        if (user == username && pwd == password) {
            cout << "登录成功，欢迎使用！\n";
            return true;
        }
        cout << "账号或密码错误，还有 " << (2 - i) << " 次机会。\n";
    }
    return false;
}

void Library::changePassword() {
    string oldPwd = readLine("请输入原密码: ");
    if (oldPwd != password) {
        cout << "原密码不正确！\n";
        return;
    }
    string newPwd = readLine("请输入新密码: ");
    if (newPwd.empty()) {
        cout << "新密码不能为空！\n";
        return;
    }
    string again = readLine("请再输入一次新密码: ");
    if (newPwd != again) {
        cout << "两次输入的密码不一样！\n";
        return;
    }
    password = newPwd;
    saveData();
    cout << "密码修改成功！\n";
}

// ---------------- 主菜单 ----------------
void Library::run() {
    if (!login()) {
        cout << "登录失败，程序退出。\n";
        return;
    }

    int choice;
    while (true) {
        cout << "\n========== 图书管理系统 ==========\n";
        cout << "1. 图书管理\n";
        cout << "2. 读者管理\n";
        cout << "3. 借阅管理\n";
        cout << "4. 统计信息\n";
        cout << "5. 修改密码\n";
        cout << "0. 退出\n";
        choice = readInt("请选择: ");

        if (choice == 1) bookMenu();
        else if (choice == 2) readerMenu();
        else if (choice == 3) borrowMenu();
        else if (choice == 4) statistics();
        else if (choice == 5) changePassword();
        else if (choice == 0) {
            saveData();
            cout << "数据已保存，再见！\n";
            return;
        } else {
            cout << "输入有误，请重新选择！\n";
        }
    }
}

// ---------------- 图书管理 ----------------
void Library::bookMenu() {
    int choice;
    while (true) {
        cout << "\n---- 图书管理 ----\n";
        cout << "1. 添加图书\n";
        cout << "2. 修改图书\n";
        cout << "3. 删除图书\n";
        cout << "4. 显示全部图书\n";
        cout << "5. 查询图书\n";
        cout << "0. 返回\n";
        choice = readInt("请选择: ");

        if (choice == 1) addBook();
        else if (choice == 2) modifyBook();
        else if (choice == 3) deleteBook();
        else if (choice == 4) showBooks();
        else if (choice == 5) searchBook();
        else if (choice == 0) return;
        else cout << "输入有误！\n";
    }
}

void Library::addBook() {
    string id = readLine("请输入图书编号: ");
    if (findBook(id) != NULL) {
        cout << "该编号已存在！\n";
        return;
    }
    string title = readLine("请输入书名: ");
    string author = readLine("请输入作者: ");
    string publisher = readLine("请输入出版社: ");
    int total = readInt("请输入数量: ");
    if (total <= 0) {
        cout << "数量必须大于 0！\n";
        return;
    }
    books.push_back(Book(id, title, author, publisher, total));
    saveData();
    cout << "添加成功！\n";
}

void Library::modifyBook() {
    string id = readLine("请输入要修改的图书编号: ");
    Book* b = findBook(id);
    if (b == NULL) {
        cout << "没有找到这本书！\n";
        return;
    }
    cout << "当前信息：\n";
    b->show();

    string s = readLine("新书名（直接回车不修改）: ");
    if (!s.empty()) b->title = s;

    s = readLine("新作者（直接回车不修改）: ");
    if (!s.empty()) b->author = s;

    s = readLine("新出版社（直接回车不修改）: ");
    if (!s.empty()) b->publisher = s;

    s = readLine("新数量（直接回车不修改）: ");
    if (!s.empty()) {
        int total = atoi(s.c_str());
        int borrowed = borrowedCount(b->id);
        if (total < borrowed) {
            cout << "数量不能小于已借出的数量（" << borrowed << " 本）！\n";
            return;
        }
        b->total = total;
        b->available = total - borrowed;
    }
    saveData();
    cout << "修改成功！\n";
}

void Library::deleteBook() {
    string id = readLine("请输入要删除的图书编号: ");
    Book* b = findBook(id);
    if (b == NULL) {
        cout << "没有找到这本书！\n";
        return;
    }
    if (borrowedCount(id) > 0) {
        cout << "这本书还有人在借，不能删除！\n";
        return;
    }
    for (int i = 0; i < (int)books.size(); i++) {
        if (books[i].id == id) {
            books.erase(books.begin() + i);
            break;
        }
    }
    saveData();
    cout << "删除成功！\n";
}

void Library::showBooks() {
    if (books.empty()) {
        cout << "还没有图书。\n";
        return;
    }
    cout << left << setw(10) << "编号" << setw(22) << "书名"
         << setw(14) << "作者" << setw(16) << "出版社" << "可借/总数\n";
    for (int i = 0; i < (int)books.size(); i++) {
        books[i].show();
    }
}

void Library::searchBook() {
    string key = readLine("请输入书名或作者中的关键字: ");
    int count = 0;
    for (int i = 0; i < (int)books.size(); i++) {
        if (books[i].title.find(key) != string::npos ||
            books[i].author.find(key) != string::npos) {
            if (count == 0) {
                cout << left << setw(10) << "编号" << setw(22) << "书名"
                     << setw(14) << "作者" << setw(16) << "出版社" << "可借/总数\n";
            }
            books[i].show();
            count++;
        }
    }
    if (count == 0) cout << "没有找到符合条件的图书。\n";
}

// ---------------- 读者管理 ----------------
void Library::readerMenu() {
    int choice;
    while (true) {
        cout << "\n---- 读者管理 ----\n";
        cout << "1. 添加读者\n";
        cout << "2. 修改读者\n";
        cout << "3. 删除读者\n";
        cout << "4. 显示全部读者\n";
        cout << "0. 返回\n";
        choice = readInt("请选择: ");

        if (choice == 1) addReader();
        else if (choice == 2) modifyReader();
        else if (choice == 3) deleteReader();
        else if (choice == 4) showReaders();
        else if (choice == 0) return;
        else cout << "输入有误！\n";
    }
}

void Library::addReader() {
    cout << "1. 学生   2. 教师\n";
    int type = readInt("请选择类型: ");
    if (type != 1 && type != 2) {
        cout << "类型有误！\n";
        return;
    }
    string id = readLine("请输入学号/工号: ");
    if (findReader(id) != NULL) {
        cout << "该编号已存在！\n";
        return;
    }
    string name = readLine("请输入姓名: ");
    string dept = readLine("请输入院系: ");

    if (type == 1) readers.push_back(new Student(id, name, dept));
    else readers.push_back(new Teacher(id, name, dept));
    saveData();
    cout << "添加成功！\n";
}

void Library::modifyReader() {
    string id = readLine("请输入要修改的读者编号: ");
    Person* p = findReader(id);
    if (p == NULL) {
        cout << "没有找到这个读者！\n";
        return;
    }
    cout << "当前信息：\n";
    p->show();

    string s = readLine("新姓名（直接回车不修改）: ");
    if (!s.empty()) p->setName(s);

    s = readLine("新院系（直接回车不修改）: ");
    if (!s.empty()) p->setDept(s);
    saveData();
    cout << "修改成功！\n";
}

void Library::deleteReader() {
    string id = readLine("请输入要删除的读者编号: ");
    Person* p = findReader(id);
    if (p == NULL) {
        cout << "没有找到这个读者！\n";
        return;
    }
    if (!p->getBooks().empty()) {
        cout << "这个读者还有书没还，不能删除！\n";
        return;
    }
    for (int i = 0; i < (int)readers.size(); i++) {
        if (readers[i]->getId() == id) {
            delete readers[i];
            readers.erase(readers.begin() + i);
            break;
        }
    }
    saveData();
    cout << "删除成功！\n";
}

void Library::showReaders() {
    if (readers.empty()) {
        cout << "还没有读者。\n";
        return;
    }
    cout << left << setw(10) << "编号" << setw(8) << "身份"
         << setw(10) << "姓名" << setw(16) << "院系" << "已借/最多\n";
    for (int i = 0; i < (int)readers.size(); i++) {
        readers[i]->show();
    }
}

// ---------------- 借阅管理 ----------------
void Library::borrowMenu() {
    int choice;
    while (true) {
        cout << "\n---- 借阅管理 ----\n";
        cout << "1. 借书\n";
        cout << "2. 还书\n";
        cout << "3. 查看某读者的借阅情况\n";
        cout << "0. 返回\n";
        choice = readInt("请选择: ");

        if (choice == 1) borrowBook();
        else if (choice == 2) returnBook();
        else if (choice == 3) showReaderBooks();
        else if (choice == 0) return;
        else cout << "输入有误！\n";
    }
}

void Library::borrowBook() {
    string readerId = readLine("请输入读者编号: ");
    Person* p = findReader(readerId);
    if (p == NULL) {
        cout << "没有找到这个读者！\n";
        return;
    }
    string bookId = readLine("请输入图书编号: ");
    Book* b = findBook(bookId);
    if (b == NULL) {
        cout << "没有找到这本书！\n";
        return;
    }
    if (!p->canBorrow()) {
        cout << "该读者借书已达到上限（" << p->getMaxBorrow() << " 本）！\n";
        return;
    }
    if (p->hasBorrowed(bookId)) {
        cout << "该读者已经借过这本书了！\n";
        return;
    }
    if (b->available <= 0) {
        cout << "这本书已经借完了！\n";
        return;
    }

    b->available--;
    p->borrowBook(bookId);
    saveData();
    cout << "借书成功！《" << b->title << "》\n";
}

void Library::returnBook() {
    string readerId = readLine("请输入读者编号: ");
    Person* p = findReader(readerId);
    if (p == NULL) {
        cout << "没有找到这个读者！\n";
        return;
    }
    string bookId = readLine("请输入图书编号: ");
    if (!p->hasBorrowed(bookId)) {
        cout << "该读者没有借这本书！\n";
        return;
    }

    Book* b = findBook(bookId);
    if (b != NULL) b->available++;
    p->returnBook(bookId);
    saveData();
    cout << "还书成功！\n";
}

void Library::showReaderBooks() {
    string id = readLine("请输入读者编号: ");
    Person* p = findReader(id);
    if (p == NULL) {
        cout << "没有找到这个读者！\n";
        return;
    }
    vector<string> list = p->getBooks();
    if (list.empty()) {
        cout << p->getName() << " 当前没有借书。\n";
        return;
    }
    cout << p->getName() << " 借了 " << list.size() << " 本书：\n";
    for (int i = 0; i < (int)list.size(); i++) {
        Book* b = findBook(list[i]);
        if (b != NULL) cout << "  " << b->id << "  " << b->title << endl;
        else cout << "  " << list[i] << "  （图书已删除）" << endl;
    }
}

// ---------------- 统计 ----------------
void Library::statistics() {
    int total = 0, available = 0;
    for (int i = 0; i < (int)books.size(); i++) {
        total += books[i].total;
        available += books[i].available;
    }

    int students = 0, teachers = 0, borrowing = 0;
    for (int i = 0; i < (int)readers.size(); i++) {
        if (readers[i]->getType() == "学生") students++;
        else teachers++;
        borrowing += (int)readers[i]->getBooks().size();
    }

    cout << "\n---- 统计信息 ----\n";
    cout << "图书：" << books.size() << " 种，共 " << total << " 本，可借 " << available << " 本\n";
    cout << "读者：" << readers.size() << " 人（学生 " << students << "，教师 " << teachers << "）\n";
    cout << "当前借出：" << borrowing << " 本\n";
}
