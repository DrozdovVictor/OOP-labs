#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>

#include "functions.h"

using namespace std;

// global containers
array<Tester, 5> testersArray;
vector<Application*> appsVector;

map<int, Bug*> mapById;
set<Bug*, BugPtrLess> setById;
multimap<string, Bug*> multimapByTester;
multiset<Bug*, BugPtrLess> multisetById;
unordered_map<int, Bug*> umapById;
unordered_set<Bug*, BugPtrHash, BugPtrEqual> usetById;
unordered_multimap<string, Bug*> umultimapByTester;
unordered_multiset<Bug*, BugPtrHash, BugPtrEqual> umultisetById;

// helpers
int inputInt(const string& prompt) {
    int x;
    cout << prompt;
    while (!(cin >> x)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "  [!] Ошибка! Введите ЧИСЛО: ";
    }
    cin.ignore();
    return x;
}

string inputStr(const string& prompt) {
    string s;
    cout << prompt;
    getline(cin, s);
    return s;
}

Tester* findTesterById(int id) {
    for (auto& t : testersArray)
        if (t.getId() == id) return &t;
    return nullptr;
}

Application* findAppById(int id) {
    for (auto* a : appsVector)
        if (a->getId() == id) return a;
    return nullptr;
}

void printSeparator() { cout << string(70, '-') << endl; }

Bug* inputNewBug() {
    int    id = inputInt("  ID бага: ");
    string name = inputStr("  Название: ");
    string desc = inputStr("  Описание: ");
    string prio = inputStr("  Приоритет (Low/Medium/High/Critical): ");
    string stat = inputStr("  Статус (Open/In Progress/Resolved/Closed): ");
    string date = inputStr("  Дата обнаружения: ");
    string steps = inputStr("  Шаги воспроизведения: ");

    int appId = inputInt("  ID приложения (100/101): ");
    Application* app = findAppById(appId);
    if (!app) { cout << "  [!] Приложение не найдено\n"; return nullptr; }

    int tid = inputInt("  ID тестировщика (1..5): ");
    Tester* tester = findTesterById(tid);
    if (!tester) { cout << "  [!] Тестировщик не найден\n"; return nullptr; }

    return new Bug(id, name, desc, prio, stat, date, steps, app, tester);
}

// ============================================================
//  Обобщённые операции над «диапазонами» итераторов.
//  Работают и для map/multimap (итератор на pair<K,Bug*>),
//  и для set/multiset (итератор на Bug*).
// ============================================================
template<typename MapIt>
void printMapRange(MapIt b, MapIt e, ostream& out) {
    for (auto it = b; it != e; ++it) it->second->printInfo(out);
}

template<typename SetIt>
void printSetRange(SetIt b, SetIt e, ostream& out) {
    for (auto it = b; it != e; ++it) (*it)->printInfo(out);
}

template<typename MapIt>
void filterMapRange(MapIt b, MapIt e, const string& prio, ostream& out) {
    for (auto it = b; it != e; ++it)
        if (it->second->getPriority() == prio) it->second->printInfo(out);
}

template<typename SetIt>
void filterSetRange(SetIt b, SetIt e, const string& prio, ostream& out) {
    for (auto it = b; it != e; ++it)
        if ((*it)->getPriority() == prio) (*it)->printInfo(out);
}

// Сортировка «взгляда» для unordered-контейнеров: собираем
// указатели в vector, сортируем по id, выводим.
template<typename MapIt>
void sortPrintMap(MapIt b, MapIt e, ostream& out) {
    vector<Bug*> v;
    for (auto it = b; it != e; ++it) v.push_back(it->second);
    sort(v.begin(), v.end(), [](Bug* a, Bug* b) { return a->getId() < b->getId(); });
    for (auto* p : v) p->printInfo(out);
}

template<typename SetIt>
void sortPrintSet(SetIt b, SetIt e, ostream& out) {
    vector<Bug*> v(b, e);
    sort(v.begin(), v.end(), [](Bug* a, Bug* b) { return a->getId() < b->getId(); });
    for (auto* p : v) p->printInfo(out);
}

template<typename MapIt>
bool editInMapRange(MapIt b, MapIt e, int id) {
    for (auto it = b; it != e; ++it) {
        Bug* bug = it->second;
        if (bug->getId() != id) continue;
        bug->setName(inputStr("  Новое название:   "));
        bug->setPriority(inputStr("  Новый приоритет:  "));
        bug->setStatus(inputStr("  Новый статус:     "));
        return true;
    }
    return false;
}

template<typename SetIt>
bool editInSetRange(SetIt b, SetIt e, int id) {
    for (auto it = b; it != e; ++it) {
        Bug* bug = *it;
        if (bug->getId() != id) continue;
        bug->setName(inputStr("  Новое название:   "));
        bug->setPriority(inputStr("  Новый приоритет:  "));
        bug->setStatus(inputStr("  Новый статус:     "));
        return true;
    }
    return false;
}

//  Демо-данные
static Bug* makeBug(int n) {
    Application* a0 = findAppById(100);
    Application* a1 = findAppById(101);
    Tester* t0 = findTesterById(1);
    Tester* t1 = findTesterById(2);
    if (n == 1)
        return new Bug(1, "Кнопка не реагирует",
            "Кнопка 'Купить' не работает", "High", "Open",
            "2024-05-01", "1. Открыть товар\n2. Нажать 'Купить'", a0, t0);
    if (n == 2)
        return new Bug(2, "Падение при запуске",
            "Краш без интернета", "Critical", "In Progress",
            "2024-05-03", "1. Отключить сеть\n2. Запустить", a1, t1);
    return new Bug(3, "Опечатка в меню",
        "'Отмена' -> 'Отменна'", "Low", "Resolved",
        "2024-05-04", "1. Открыть меню", a0, t0);
}

void initDemoData() {
    testersArray[0] = Tester(1, "Иванов И.И.", "Manual QA");
    testersArray[1] = Tester(2, "Петров П.П.", "Security QA");
    testersArray[2] = Tester(3, "Сидоров С.С.", "Automation QA");
    testersArray[3] = Tester(4, "Козлова А.А.", "Performance QA");
    testersArray[4] = Tester(5, "Морозов М.М.", "Manual QA");

    appsVector.push_back(new Application(100, "WebShop", "1.2", "Web"));
    appsVector.push_back(new Application(101, "BankApp", "3.0", "Mobile"));

    // map<int,Bug*>
    mapById[1] = makeBug(1);
    mapById[2] = makeBug(2);
    mapById[3] = makeBug(3);

    // unordered_map<int,Bug*>
    umapById[1] = makeBug(1);
    umapById[2] = makeBug(2);
    umapById[3] = makeBug(3);

    // multimap<string,Bug*> (Иванов дважды → демонстрация equal_range)
    multimapByTester.insert({ "Иванов И.И.", makeBug(1) });
    multimapByTester.insert({ "Петров П.П.", makeBug(2) });
    multimapByTester.insert({ "Иванов И.И.", makeBug(3) });

    // unordered_multimap<string,Bug*>
    umultimapByTester.insert({ "Иванов И.И.", makeBug(1) });
    umultimapByTester.insert({ "Петров П.П.", makeBug(2) });
    umultimapByTester.insert({ "Иванов И.И.", makeBug(3) });

    // set<Bug*>
    setById.insert(makeBug(1));
    setById.insert(makeBug(2));
    setById.insert(makeBug(3));

    // unordered_set<Bug*>
    usetById.insert(makeBug(1));
    usetById.insert(makeBug(2));
    usetById.insert(makeBug(3));

    // multiset<Bug*>
    multisetById.insert(makeBug(1));
    multisetById.insert(makeBug(2));
    multisetById.insert(makeBug(3));

    // unordered_multiset<Bug*>
    umultisetById.insert(makeBug(1));
    umultisetById.insert(makeBug(2));
    umultisetById.insert(makeBug(3));
}

//  МЕНЮ: map-like (map, unordered_map, multimap, unordered_multimap)
// уникальные ключи (map / unordered_map), ключ = int (id)
template<typename MapType>
void menuUniqueMap(MapType& m, const char* name, bool isUnordered) {
    int ch;
    do {
        printSeparator();
        cout << ">>> " << name << "  (размер: " << m.size() << ")\n"
            << "  1. Добавить\n"
            << "  2. Удалить по ID\n"
            << "  3. Редактировать по ID\n"
            << "  4. Вывести на экран\n"
            << "  5. Сохранить в файл\n"
            << "  6. Фильтр по приоритету\n"
            << "  7. Найти по ID\n"
            << "  8. Сортированный вывод\n"
            << "  0. Назад\n";
        ch = inputInt("  Выбор: ");
        switch (ch) {
        case 1: {
            Bug* b = inputNewBug();
            if (!b) break;
            auto r = m.insert({ b->getId(), b });
            if (r.second) cout << "  [OK] добавлено\n";
            else { cout << "  [!] баг с таким ID уже есть\n"; delete b; }
            break;
        }
        case 2: {
            int id = inputInt("  ID: ");
            auto it = m.find(id);
            if (it == m.end()) { cout << "  [!] не найдено\n"; break; }
            delete it->second;
            m.erase(it);
            cout << "  [OK] удалено\n";
            break;
        }
        case 3: {
            int id = inputInt("  ID: ");
            if (editInMapRange(m.begin(), m.end(), id)) cout << "  [OK]\n";
            else cout << "  [!] не найдено\n";
            break;
        }
        case 4: printMapRange(m.begin(), m.end(), cout); break;
        case 5: {
            ofstream f("map_out.txt");
            printMapRange(m.begin(), m.end(), f);
            cout << "  [OK] сохранено в map_out.txt\n";
            break;
        }
        case 6: {
            string p = inputStr("  Приоритет: ");
            filterMapRange(m.begin(), m.end(), p, cout);
            break;
        }
        case 7: {
            int id = inputInt("  ID: ");
            auto it = m.find(id);
            if (it == m.end()) cout << "  [!] не найдено\n";
            else it->second->printInfo();
            break;
        }
        case 8:
            if (isUnordered) {
                cout << "  (unordered — выводим сортированный взгляд по ID)\n";
                sortPrintMap(m.begin(), m.end(), cout);
            }
            else {
                cout << "  (уже отсортировано по ключу-ID)\n";
                printMapRange(m.begin(), m.end(), cout);
            }
            break;
        }
    } while (ch != 0);
}

// дубликаты ключей (multimap / unordered_multimap), ключ = string
template<typename MapType>
void menuMultiMap(MapType& m, const char* name, bool isUnordered) {
    int ch;
    do {
        printSeparator();
        cout << ">>> " << name << "  (размер: " << m.size() << ")\n"
            << "  1. Добавить\n"
            << "  2. Удалить по ID бага\n"
            << "  3. Редактировать по ID бага\n"
            << "  4. Вывести на экран\n"
            << "  5. Сохранить в файл\n"
            << "  6. Фильтр по приоритету\n"
            << "  7. Найти по тестировщику (equal_range)\n"
            << "  8. Сортированный вывод\n"
            << "  0. Назад\n";
        ch = inputInt("  Выбор: ");
        switch (ch) {
        case 1: {
            Bug* b = inputNewBug();
            if (!b) break;
            string key = b->getAssignedTester()->getFullName();
            m.insert({ key, b });
            cout << "  [OK] вставлено под ключ '" << key << "'\n";
            break;
        }
        case 2: {
            int id = inputInt("  ID бага: ");
            for (auto it = m.begin(); it != m.end(); ++it) {
                if (it->second->getId() == id) {
                    delete it->second;
                    m.erase(it);
                    cout << "  [OK]\n";
                    goto done_remove;
                }
            }
            cout << "  [!] не найдено\n";
        done_remove:;
            break;
        }
        case 3: {
            int id = inputInt("  ID бага: ");
            if (editInMapRange(m.begin(), m.end(), id)) cout << "  [OK]\n";
            else cout << "  [!] не найдено\n";
            break;
        }
        case 4: printMapRange(m.begin(), m.end(), cout); break;
        case 5: {
            ofstream f("multimap_out.txt");
            printMapRange(m.begin(), m.end(), f);
            cout << "  [OK] сохранено в multimap_out.txt\n";
            break;
        }
        case 6: {
            string p = inputStr("  Приоритет: ");
            filterMapRange(m.begin(), m.end(), p, cout);
            break;
        }
        case 7: {
            string name = inputStr("  ФИО тестировщика: ");
            auto r = m.equal_range(name);
            bool found = false;
            for (auto it = r.first; it != r.second; ++it) {
                it->second->printInfo();
                found = true;
            }
            if (!found) cout << "  [!] нет записей для '" << name << "'\n";
            break;
        }
        case 8:
            if (isUnordered) {
                cout << "  (unordered — выводим сортированный взгляд по ID)\n";
            }
            else {
                cout << "  (уже отсортировано по ключу — ФИО тестировщика)\n";
            }
            sortPrintMap(m.begin(), m.end(), cout);
            break;
        }
    } while (ch != 0);
}

//  МЕНЮ: set-like (set, unordered_set, multiset, unordered_multiset)
// уникальные значения (set / unordered_set)
template<typename SetType>
void menuUniqueSet(SetType& s, const char* name, bool isUnordered) {
    int ch;
    do {
        printSeparator();
        cout << ">>> " << name << "  (размер: " << s.size() << ")\n"
            << "  1. Добавить\n"
            << "  2. Удалить по ID\n"
            << "  3. Редактировать по ID\n"
            << "  4. Вывести на экран\n"
            << "  5. Сохранить в файл\n"
            << "  6. Фильтр по приоритету\n"
            << "  7. Найти по ID\n"
            << "  8. Сортированный вывод\n"
            << "  0. Назад\n";
        ch = inputInt("  Выбор: ");
        switch (ch) {
        case 1: {
            Bug* b = inputNewBug();
            if (!b) break;
            auto r = s.insert(b);
            if (r.second) cout << "  [OK]\n";
            else { cout << "  [!] такой ID уже есть\n"; delete b; }
            break;
        }
        case 2: {
            int id = inputInt("  ID: ");
            for (auto it = s.begin(); it != s.end(); ++it) {
                if ((*it)->getId() == id) {
                    delete* it;
                    s.erase(it);
                    cout << "  [OK]\n";
                    goto done2;
                }
            }
            cout << "  [!] не найдено\n";
        done2:;
            break;
        }
        case 3: {
            int id = inputInt("  ID: ");
            if (editInSetRange(s.begin(), s.end(), id)) cout << "  [OK]\n";
            else cout << "  [!] не найдено\n";
            break;
        }
        case 4: printSetRange(s.begin(), s.end(), cout); break;
        case 5: {
            ofstream f("set_out.txt");
            printSetRange(s.begin(), s.end(), f);
            cout << "  [OK] сохранено в set_out.txt\n";
            break;
        }
        case 6: {
            string p = inputStr("  Приоритет: ");
            filterSetRange(s.begin(), s.end(), p, cout);
            break;
        }
        case 7: {
            int id = inputInt("  ID: ");
            Bug probe(id);
            auto it = s.find(&probe);
            if (it == s.end()) cout << "  [!] не найдено\n";
            else (*it)->printInfo();
            break;
        }
        case 8:
            if (isUnordered) {
                cout << "  (unordered — выводим сортированный взгляд по ID)\n";
                sortPrintSet(s.begin(), s.end(), cout);
            }
            else {
                cout << "  (уже отсортировано по компаратору — ID)\n";
                printSetRange(s.begin(), s.end(), cout);
            }
            break;
        }
    } while (ch != 0);
}

// ---- дубликаты значений (multiset / unordered_multiset) ----
template<typename SetType>
void menuMultiSet(SetType& s, const char* name, bool isUnordered) {
    int ch;
    do {
        printSeparator();
        cout << ">>> " << name << "  (размер: " << s.size() << ")\n"
            << "  1. Добавить (дубликаты разрешены)\n"
            << "  2. Удалить один по ID\n"
            << "  3. Редактировать по ID\n"
            << "  4. Вывести на экран\n"
            << "  5. Сохранить в файл\n"
            << "  6. Фильтр по приоритету\n"
            << "  7. count() — сколько с таким ID\n"
            << "  8. Сортированный вывод\n"
            << "  0. Назад\n";
        ch = inputInt("  Выбор: ");
        switch (ch) {
        case 1: {
            Bug* b = inputNewBug();
            if (!b) break;
            s.insert(b);
            cout << "  [OK] (в multiset дубликаты разрешены)\n";
            break;
        }
        case 2: {
            int id = inputInt("  ID: ");
            for (auto it = s.begin(); it != s.end(); ++it) {
                if ((*it)->getId() == id) {
                    delete* it;
                    s.erase(it);
                    cout << "  [OK]\n";
                    goto done3;
                }
            }
            cout << "  [!] не найдено\n";
        done3:;
            break;
        }
        case 3: {
            int id = inputInt("  ID: ");
            if (editInSetRange(s.begin(), s.end(), id)) cout << "  [OK]\n";
            else cout << "  [!] не найдено\n";
            break;
        }
        case 4: printSetRange(s.begin(), s.end(), cout); break;
        case 5: {
            ofstream f("multiset_out.txt");
            printSetRange(s.begin(), s.end(), f);
            cout << "  [OK] сохранено в multiset_out.txt\n";
            break;
        }
        case 6: {
            string p = inputStr("  Приоритет: ");
            filterSetRange(s.begin(), s.end(), p, cout);
            break;
        }
        case 7: {
            int id = inputInt("  ID: ");
            Bug probe(id);
            cout << "  count = " << s.count(&probe) << "\n";
            break;
        }
        case 8:
            if (isUnordered) {
                cout << "  (unordered — выводим сортированный взгляд по ID)\n";
            }
            else {
                cout << "  (уже отсортировано по компаратору — ID)\n";
            }
            sortPrintSet(s.begin(), s.end(), cout);
            break;
        }
    } while (ch != 0);
}

// Обёртки для каждого из 8 контейнеров
void menuMap() { menuUniqueMap(mapById, "map<int,Bug*>", false); }
void menuUnorderedMap() { menuUniqueMap(umapById, "unordered_map<int,Bug*>", true); }
void menuMultimap() { menuMultiMap(multimapByTester, "multimap<string,Bug*>", false); }
void menuUnorderedMultimap() { menuMultiMap(umultimapByTester, "unordered_multimap<string,Bug*>", true); }

void menuSet() { menuUniqueSet(setById, "set<Bug*>", false); }
void menuUnorderedSet() { menuUniqueSet(usetById, "unordered_set<Bug*>", true); }
void menuMultiset() { menuMultiSet(multisetById, "multiset<Bug*>", false); }
void menuUnorderedMultiset() { menuMultiSet(umultisetById, "unordered_multiset<Bug*>", true); }

void printApplications() {
    cout << "  ID\tПриложение\tВерсия\n";
    cout << string(50, '-') << "\n";
    for (auto* a : appsVector) {
        cout << "  " << a->getId() << "\t"
            << a->getAppName() << "\t\t"
            << "v" << a->getVersion() << "\n";
    }
}
void printTesters() {
    cout << "  ID\tФИО\t\t\tСпециализация\n";
    cout << string(60, '-') << "\n";
    for (const auto& t : testersArray) {
        if (t.getId() == 0) continue; // Пропускаем пустые ячейки массива
        cout << "  " << t.getId() << "\t"
            << t.getFullName() << "\t"
            << t.getSpecialization() << "\n";
    }
}