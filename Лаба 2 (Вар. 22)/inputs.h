#pragma once
#include <iostream>
#include <string>

inline int inputInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;

        if (std::cin.good()) {
            std::cin.ignore(10000, '\n');
            return value;
        }

        std::cout << "Ошибка: введите целое число.\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
}

inline double inputDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.good()) {
            std::cin.ignore(10000, '\n');
            return value;
        }
        std::cout << "Ошибка: введите число (можно с точкой).\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
}

inline std::string inputLine(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}

inline char inputChar(const std::string& prompt) {
    char value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.good()) {
            std::cin.ignore(10000, '\n');
            return value;
        }
        std::cout << "Ошибка: введите один символ.\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
}