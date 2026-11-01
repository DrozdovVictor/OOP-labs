#include <iostream>
#include <windows.h>

#include "classes.h"
#include "functions.h"

using namespace std;

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");

    initDemoData();

    int ch;
    do {
        cout << "\n=====================================================\n";
        cout << "   УЧЁТ БАГОВ И УЯЗВИМОСТЕЙ (ЛР №4 — STL)\n";
        cout << "=====================================================\n";
        cout << "  1. map<int, Bug*>\n";
        cout << "  2. set<Bug*>\n";
        cout << "  3. multimap<string, Bug*>\n";
        cout << "  4. multiset<Bug*>\n";
        cout << "  5. unordered_map<int, Bug*>\n";
        cout << "  6. unordered_set<Bug*>\n";
        cout << "  7. unordered_multimap<string, Bug*>\n";
        cout << "  8. unordered_multiset<Bug*>\n";
        cout << "  9. Посмотреть список приложений (ID)\n";
        cout << " 10. Посмотреть список тестировщиков (ID)\n";
        cout << "  0. Выход\n";
        ch = inputInt("  Выбор: ");

        switch (ch) {
        case 1:  menuMap(); break;
        case 2:  menuSet(); break;
        case 3:  menuMultimap(); break;
        case 4:  menuMultiset(); break;
        case 5:  menuUnorderedMap(); break;
        case 6:  menuUnorderedSet(); break;
        case 7:  menuUnorderedMultimap(); break;
        case 8:  menuUnorderedMultiset(); break;
        case 9:  printApplications(); break;
        case 10: printTesters(); break;
        }
    } while (ch != 0);

    // освобождение памяти
    for (auto& kv : mapById)           delete kv.second;
    for (auto p : setById)           delete p;
    for (auto& kv : multimapByTester)  delete kv.second;
    for (auto p : multisetById)      delete p;
    for (auto& kv : umapById)          delete kv.second;
    for (auto p : usetById)          delete p;
    for (auto& kv : umultimapByTester) delete kv.second;
    for (auto p : umultisetById)     delete p;

    for (auto* a : appsVector) delete a;
    // testersArray — по значению, чистить не надо

    cout << "\nПрограмма завершена. До свидания!" << endl;
    return 0;
}