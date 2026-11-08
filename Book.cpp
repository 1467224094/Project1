#include "Book.h"

#include <iomanip>
#include <iostream>

using namespace std;

Book::Book() : total(0), available(0) {}

Book::Book(string id, string title, string author, string publisher, int total)
    : id(id), title(title), author(author), publisher(publisher),
      total(total), available(total) {}

void Book::show() const {
    cout << left << setw(10) << id
         << setw(22) << title
         << setw(14) << author
         << setw(16) << publisher
         << available << "/" << total << endl;
}
