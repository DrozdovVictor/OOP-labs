#include <iostream>
#include <windows.h>

#include"classes.h"
#include"functions.h"

using namespace std;

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");

    initDemoData();

    int ch;
    do {
        cout << "\n=====================================================\n";
        cout << "   УЧЁТ БАГОВ И УЯЗВИМОСТЕЙ\n";
        cout << "=====================================================\n";
        cout << "  1. Работа с VECTOR<Bug*>\n";
        cout << "  2. Работа с LIST<Vulnerability>\n";
        cout << "  3. Работа с ARRAY<Tester, 5>\n";
        cout << "  0. Выход\n";
        ch = inputInt("  Выбор: ");

        switch (ch) {
        case 1: menuVector(); break;
        case 2: menuList();   break;
        case 3: menuArray();  break;
        }
    } while (ch != 0);

    // Освобождение памяти указателей
    for (auto* b : bugsVector) delete b;
    for (auto* a : appsVector) delete a;

    cout << "\nПрограмма завершена. До свидания!" << endl;
    return 0;
}