#pragma once
#define _CRT_SECURE_NO_WARNINGS
#ifndef BOOK_H
#define BOOK_H

#define FILENAME "books.dat"
#define MAX_NAME 100


typedef struct Book {
    int id;
    char title[MAX_NAME];
    char author[MAX_NAME];
    char publisher[MAX_NAME];
    float price;
    int total;
    int available;
    struct Book* next;
} Book;

extern Book* head;


void clearInput(void);
int  readInt(const char* prompt);
float readFloat(const char* prompt);
void readString(const char* prompt, char* buf, int size);
void printBookDetail(const Book* book);
Book* findBookById(int id);
void freeAllBooks(void);


void addBook(void);
void displayBooks(void);
void searchBook(void);
void modifyBook(void);
void deleteBook(void);
void borrowBook(void);
void returnBook(void);


void saveToFile(void);
void loadFromFile(void);


void menu(void);

#endif