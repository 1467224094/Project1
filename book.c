#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include <string.h>

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

Book* head = NULL;

void addBook();
void displayBooks();
void searchBook();
void modifyBook();
void deleteBook();
void borrowBook();
void returnBook();
void saveToFile();
void loadFromFile();
void menu();
void clearInput();

int main() {
    loadFromFile();
    menu();
    system("pause");
    return 0;
}

void clearInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


void addBook() {
    Book* newBook = (Book*)malloc(sizeof(Book));
    if (newBook == NULL) {
        printf("内存分配失败！\n");
        return;
    }

    printf("请输入图书ID：");
    scanf("%d", &newBook->id);

    clearInput();

    printf("请输入书名：");
    fgets(newBook->title, MAX_NAME, stdin);
    newBook->title[strcspn(newBook->title, "\n")] = '\0';

    printf("请输入作者：");
    fgets(newBook->author, MAX_NAME, stdin);
    newBook->author[strcspn(newBook->author, "\n")] = '\0';

    printf("请输入出版社：");
    fgets(newBook->publisher, MAX_NAME, stdin);
    newBook->publisher[strcspn(newBook->publisher, "\n")] = '\0';

    printf("请输入价格：");
    scanf("%f", &newBook->price);

    printf("请输入总数量：");
    scanf("%d", &newBook->total);

    newBook->available = newBook->total;
    newBook->next = NULL;


    Book* current = head;
    while (current != NULL) {
        if (current->id == newBook->id) {
            printf("该图书ID已存在！\n");
            free(newBook);
            return;
        }
        current = current->next;
    }


    newBook->next = head;
    head = newBook;

    printf("图书添加成功！\n");
    saveToFile();
}


void displayBooks() {
    if (head == NULL) {
        printf("当前没有图书！\n");
        return;
    }

    printf("\n%-10s %-20s %-15s %-20s %-10s %-8s %-8s\n",
        "ID", "书名", "作者", "出版社", "价格", "总数", "可借");
    printf("---------------------------------------------------------------\n");

    Book* current = head;
    while (current != NULL) {
        printf("%-10d %-20s %-15s %-20s %-10.2f %-8d %-8d\n",
            current->id,
            current->title,
            current->author,
            current->publisher,
            current->price,
            current->total,
            current->available);
        current = current->next;
    }
}

void searchBook() {
    if (head == NULL) {
        printf("当前没有图书！\n");
        return;
    }

    int choice;
    printf("1. 按ID查询\n");
    printf("2. 按书名查询\n");
    printf("3. 按作者查询\n");
    printf("请选择查询方式：");
    scanf("%d", &choice);
    clearInput();

    Book* current = head;
    int found = 0;

    if (choice == 1) {
        int searchId;
        printf("请输入图书ID：");
        scanf("%d", &searchId);

        while (current != NULL) {
            if (current->id == searchId) {
                printf("\nID：%d\n", current->id);
                printf("书名：%s\n", current->title);
                printf("作者：%s\n", current->author);
                printf("出版社：%s\n", current->publisher);
                printf("价格：%.2f\n", current->price);
                printf("总数：%d\n", current->total);
                printf("可借：%d\n", current->available);
                found = 1;
                break;
            }
            current = current->next;
        }

    }
    else if (choice == 2) {
        char keyword[MAX_NAME];
        printf("请输入书名关键字：");
        fgets(keyword, MAX_NAME, stdin);
        keyword[strcspn(keyword, "\n")] = '\0';

        while (current != NULL) {
            if (strstr(current->title, keyword) != NULL) {
                printf("\nID：%d\n", current->id);
                printf("书名：%s\n", current->title);
                printf("作者：%s\n", current->author);
                printf("出版社：%s\n", current->publisher);
                printf("价格：%.2f\n", current->price);
                printf("总数：%d\n", current->total);
                printf("可借：%d\n", current->available);
                found = 1;
            }
            current = current->next;
        }

    }
    else if (choice == 3) {
        char keyword[MAX_NAME];
        printf("请输入作者关键字：");
        fgets(keyword, MAX_NAME, stdin);
        keyword[strcspn(keyword, "\n")] = '\0';

        while (current != NULL) {
            if (strstr(current->author, keyword) != NULL) {
                printf("\nID：%d\n", current->id);
                printf("书名：%s\n", current->title);
                printf("作者：%s\n", current->author);
                printf("出版社：%s\n", current->publisher);
                printf("价格：%.2f\n", current->price);
                printf("总数：%d\n", current->total);
                printf("可借：%d\n", current->available);
                found = 1;
            }
            current = current->next;
        }

    }
    else {
        printf("无效选择！\n");
        return;
    }

    if (!found) {
        printf("未找到相关图书！\n");
    }
}


void modifyBook() {
    if (head == NULL) {
        printf("当前没有图书！\n");
        return;
    }

    int id;
    printf("请输入要修改的图书ID：");
    scanf("%d", &id);
    clearInput();

    Book* current = head;
    while (current != NULL) {
        if (current->id == id) {
            printf("\n当前图书信息：\n");
            printf("ID：%d\n", current->id);
            printf("书名：%s\n", current->title);
            printf("作者：%s\n", current->author);
            printf("出版社：%s\n", current->publisher);
            printf("价格：%.2f\n", current->price);
            printf("总数：%d\n", current->total);
            printf("可借：%d\n", current->available);

            char temp[MAX_NAME];
            float price;
            int total;

            printf("\n输入新书名（留空不修改）：");
            fgets(temp, MAX_NAME, stdin);
            temp[strcspn(temp, "\n")] = '\0';
            if (strlen(temp) > 0) {
                strcpy(current->title, temp);
            }

            printf("输入新作者（留空不修改）：");
            fgets(temp, MAX_NAME, stdin);
            temp[strcspn(temp, "\n")] = '\0';
            if (strlen(temp) > 0) {
                strcpy(current->author, temp);
            }

            printf("输入新出版社（留空不修改）：");
            fgets(temp, MAX_NAME, stdin);
            temp[strcspn(temp, "\n")] = '\0';
            if (strlen(temp) > 0) {
                strcpy(current->publisher, temp);
            }

            printf("输入新价格（输入0不修改）：");
            scanf("%f", &price);
            if (price > 0) {
                current->price = price;
            }

            printf("输入新总数（输入0不修改）：");
            scanf("%d", &total);
            if (total > 0) {
                int borrowed = current->total - current->available;
                current->total = total;
                if (total < borrowed) {
                    current->available = 0;
                }
                else {
                    current->available = total - borrowed;
                }
            }

            printf("图书修改成功！\n");
            saveToFile();
            return;
        }
        current = current->next;
    }

    printf("未找到该图书！\n");
}


void deleteBook() {
    if (head == NULL) {
        printf("当前没有图书！\n");
        return;
    }

    int id;
    printf("请输入要删除的图书ID：");
    scanf("%d", &id);

    Book* current = head;
    Book* prev = NULL;

    while (current != NULL) {
        if (current->id == id) {
            if (prev == NULL) {
                head = current->next;
            }
            else {
                prev->next = current->next;
            }
            free(current);
            printf("图书删除成功！\n");
            saveToFile();
            return;
        }
        prev = current;
        current = current->next;
    }

    printf("未找到该图书！\n");
}


void borrowBook() {
    if (head == NULL) {
        printf("当前没有图书！\n");
        return;
    }

    int id;
    printf("请输入要借阅的图书ID：");
    scanf("%d", &id);

    Book* current = head;
    while (current != NULL) {
        if (current->id == id) {
            if (current->available > 0) {
                current->available--;
                printf("借书成功！\n");
                saveToFile();
            }
            else {
                printf("该书已全部借出！\n");
            }
            return;
        }
        current = current->next;
    }

    printf("未找到该图书！\n");
}


void returnBook() {
    if (head == NULL) {
        printf("当前没有图书！\n");
        return;
    }

    int id;
    printf("请输入要归还的图书ID：");
    scanf("%d", &id);

    Book* current = head;
    while (current != NULL) {
        if (current->id == id) {
            if (current->available < current->total) {
                current->available++;
                printf("还书成功！\n");
                saveToFile();
            }
            else {
                printf("该书库存正常，无需归还！\n");
            }
            return;
        }
        current = current->next;
    }

    printf("未找到该图书！\n");
}


void saveToFile() {
    FILE* fp = fopen(FILENAME, "wb");
    if (fp == NULL) {
        printf("文件保存失败！\n");
        return;
    }

    Book* current = head;
    while (current != NULL) {
        fwrite(current, sizeof(Book), 1, fp);
        current = current->next;
    }

    fclose(fp);
}

void loadFromFile() {
    FILE* fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        return;
    }

    Book book;
    while (fread(&book, sizeof(Book), 1, fp) == 1) {
        Book* newBook = (Book*)malloc(sizeof(Book));
        if (newBook == NULL) {
            printf("内存分配失败！\n");
            break;
        }
        *newBook = book;
        newBook->next = head;
        head = newBook;
    }

    fclose(fp);
}

void menu() {
    int choice;

    do {
        printf("\n========== 图书管理系统 ==========\n");
        printf("1. 添加图书\n");
        printf("2. 显示所有图书\n");
        printf("3. 查询图书\n");
        printf("4. 修改图书\n");
        printf("5. 删除图书\n");
        printf("6. 借书\n");
        printf("7. 还书\n");
        printf("0. 退出系统\n");
        printf("==================================\n");
        printf("请选择功能：");
        scanf("%d", &choice);

        switch (choice) {
        case 1: addBook(); break;
        case 2: displayBooks(); break;
        case 3: searchBook(); break;
        case 4: modifyBook(); break;
        case 5: deleteBook(); break;
        case 6: borrowBook(); break;
        case 7: returnBook(); break;
        case 0:
            saveToFile();
            printf("感谢使用图书管理系统！\n");
            break;
        default:
            printf("无效选择，请重新输入！\n");
        }

    } while (choice != 0);
}