include "Bookmanager.h"

#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

namespace {

    constexpr const char* kFileName = "books.txt";
    constexpr char kFieldSep = '\t';

    void skipLine() {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    int readInt(const std::string& prompt, int fallback) {
        std::cout << prompt;
        int value = fallback;
        if (std::cin >> value) {
            skipLine();
            return value;
        }
        std::cin.clear();
        skipLine();
        std::cout << "输入无效，已使用默认值 " << fallback << "。\n";
        return fallback;
    }

    double readDouble(const std::string& prompt, double fallback) {
        std::cout << prompt;
        double value = fallback;
        if (std::cin >> value) {
            skipLine();
            return value;
        }
        std::cin.clear();
        skipLine();
        std::cout << "输入无效，已使用默认值 " << fallback << "。\n";
        return fallback;
    }

  
    std::string readLine(const std::string& prompt) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        return line;
    }


    bool contains(const std::string& text, const std::string& keyword) {
        if (keyword.empty()) {
            return true;
        }
        auto lower = [](unsigned char c) {
            return static_cast<char>(std::tolower(c));
            };
        for (std::size_t i = 0; i + keyword.size() <= text.size(); ++i) {
            bool hit = true;
            for (std::size_t j = 0; j < keyword.size(); ++j) {
                if (lower(static_cast<unsigned char>(text[i + j])) !=
                    lower(static_cast<unsigned char>(keyword[j]))) {
                    hit = false;
                    break;
                }
            }
            if (hit) {
                return true;
            }
        }
        return false;
    }

} 

Book* BookManager::find(int id) {
    for (Book& book : books) {
        if (book.id == id) {
            return &book;
        }
    }
    return nullptr;
}

const Book* BookManager::find(int id) const {
    for (const Book& book : books) {
        if (book.id == id) {
            return &book;
        }
    }
    return nullptr;
}
