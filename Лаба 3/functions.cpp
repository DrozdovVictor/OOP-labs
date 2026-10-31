#include<iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>

#include"classes.h"
#include"functions.h"

using namespace std;

// global containers
array<Tester, 5> testersArray;
vector<Application*> appsVector;
vector<Bug*> bugsVector;
list<Vulnerability> vulnerabilitiesList;

// helpers
int inputInt(const string& prompt) {
    int x;
    cout << prompt;
    cin >> x;
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

void printSeparator() {
    cout << string(100, '-') << endl;
}

//inition
void initDemoData() {
    // --- ARRAY: 5 тестировщиков ---
    testersArray[0] = Tester(1, "Иванов И.И.", "Manual QA");
    testersArray[1] = Tester(2, "Петров П.П.", "Security QA");
    testersArray[2] = Tester(3, "Сидоров С.С.", "Automation QA");
    testersArray[3] = Tester(4, "Козлова А.А.", "Performance QA");
    testersArray[4] = Tester(5, "Морозов М.М.", "Manual QA");

    // --- VECTOR приложений ---
    appsVector.push_back(new Application(100, "WebShop", "1.2", "Web"));
    appsVector.push_back(new Application(101, "BankApp", "3.0", "Mobile"));

    // --- VECTOR багов ---
    bugsVector.push_back(new Bug(1, "Кнопка не реагирует",
        "Кнопка 'Купить' не работает на странице товара",
        "High", "Open", "2024-05-01",
        "1. Открыть карточку товара\n2. Нажать 'Купить'",
        appsVector[0], &testersArray[0]));

    bugsVector.push_back(new Bug(2, "Падение при запуске",
        "Приложение крашится при старте без интернета",
        "Critical", "In Progress", "2024-05-03",
        "1. Отключить интернет\n2. Запустить приложение",
        appsVector[1], &testersArray[1]));

    // --- LIST уязвимостей ---
    vulnerabilitiesList.push_back(Vulnerability(200, "SQL Injection",
        "SQL-инъекция в форме логина",
        "Critical", "Open", "2024-05-05",
        "1. Ввести в поле логина: ' OR 1=1--",
        appsVector[0], &testersArray[1], 9.8, "SQL Injection"));

    vulnerabilitiesList.push_back(Vulnerability(201, "XSS",
        "Отражённый XSS в строке поиска",
        "High", "Resolved", "2024-05-06",
        "1. Ввести <script>alert(1)</script> в поиск",
        appsVector[1], &testersArray[2], 6.1, "XSS"));
}

// vector<Bug*>
void printBugs(bool toFile) {
    ostream* out = &cout;
    ofstream fout;
    if (toFile) { fout.open("bugs.txt"); out = &fout; }

    *out << left << setw(5) << "ID"
        << setw(28) << "Название"
        << setw(10) << "Приоритет"
        << setw(14) << "Статус"
        << setw(15) << "Приложение"
        << setw(20) << "Тестировщик" << endl;
    *out << string(92, '-') << endl;

    for (auto* b : bugsVector) {
        *out << left << setw(5) << b->getId()
            << setw(28) << b->getName()
            << setw(10) << b->getPriority()
            << setw(14) << b->getStatus()
            << setw(15) << (b->getApplication() ? b->getApplication()->getAppName() : "-")
            << setw(20) << (b->getAssignedTester() ? b->getAssignedTester()->getFullName() : "-")
            << endl;
    }

    if (toFile) {
        fout.close();
        cout << "  [OK] Данные сохранены в файл bugs.txt" << endl;
    }
}

void addBug() {
    int id = inputInt("  Введите ID бага: ");
    string name = inputStr("  Название: ");
    string desc = inputStr("  Описание: ");
    string prio = inputStr("  Приоритет (Low/Medium/High/Critical): ");
    string stat = inputStr("  Статус (Open/In Progress/Resolved/Closed): ");
    string date = inputStr("  Дата обнаружения: ");
    string steps = inputStr("  Шаги воспроизведения: ");

    int appId = inputInt("  ID приложения: ");
    Application* app = findAppById(appId);
    if (!app) { cout << "  [!] Приложение не найдено!" << endl; return; }

    int testerId = inputInt("  ID тестировщика: ");
    Tester* tester = findTesterById(testerId);

    bugsVector.push_back(new Bug(id, name, desc, prio, stat, date, steps, app, tester));
    cout << "  [OK] Баг добавлен в vector." << endl;
}

void removeBug() {
    int id = inputInt("  Введите ID бага для удаления: ");
    auto it = find_if(bugsVector.begin(), bugsVector.end(),
        [id](Bug* b) { return b->getId() == id; });
    if (it != bugsVector.end()) {
        delete* it;
        bugsVector.erase(it);
        cout << "  [OK] Баг удалён." << endl;
    }
    else {
        cout << "  [!] Баг с таким ID не найден." << endl;
    }
}

void editBug() {
    int id = inputInt("  Введите ID бага для редактирования: ");
    auto it = find_if(bugsVector.begin(), bugsVector.end(),
        [id](Bug* b) { return b->getId() == id; });
    if (it == bugsVector.end()) { cout << "  [!] Не найдено." << endl; return; }

    Bug* b = *it;
    b->setName(inputStr("  Новое название (" + b->getName() + "): "));
    b->setPriority(inputStr("  Новый приоритет (" + b->getPriority() + "): "));
    b->setStatus(inputStr("  Новый статус (" + b->getStatus() + "): "));
    int tid = inputInt("  Новый ID тестировщика: ");
    b->setAssignedTester(findTesterById(tid));
    cout << "  [OK] Баг обновлён." << endl;
}

void searchBug() {
    string key = inputStr("  Введите часть названия: ");
    bool found = false;
    for (auto* b : bugsVector) {
        if (b->getName().find(key) != string::npos) {
            b->printInfo();
            found = true;
        }
    }
    if (!found) cout << "  [!] Ничего не найдено." << endl;
}

void sortBugs() {
    cout << "  Сортировка по: 1 - ID | 2 - Приоритету" << endl;
    int ch = inputInt("  Выбор: ");
    if (ch == 1) {
        sort(bugsVector.begin(), bugsVector.end(),
            [](Bug* a, Bug* b) { return a->getId() < b->getId(); });
    }
    else {
        sort(bugsVector.begin(), bugsVector.end(),
            [](Bug* a, Bug* b) { return a->getPriority() < b->getPriority(); });
    }
    cout << "  [OK] Вектор багов отсортирован." << endl;
}

// list<Vulnerability>
void printVulns(bool toFile) {
    ostream* out = &cout;
    ofstream fout;
    if (toFile) { fout.open("vulnerabilities.txt"); out = &fout; }

    *out << left << setw(5) << "ID"
        << setw(20) << "Название"
        << setw(20) << "Тип"
        << setw(8) << "CVSS"
        << setw(14) << "Статус"
        << setw(15) << "Приложение"
        << setw(20) << "Тестировщик" << endl;
    *out << string(102, '-') << endl;

    for (const auto& v : vulnerabilitiesList) {
        *out << left << setw(5) << v.getId()
            << setw(20) << v.getName()
            << setw(20) << v.getVulnType()
            << setw(8) << v.getCvssScore()
            << setw(14) << v.getStatus()
            << setw(15) << (v.getApplication() ? v.getApplication()->getAppName() : "-")
            << setw(20) << (v.getAssignedTester() ? v.getAssignedTester()->getFullName() : "-")
            << endl;
    }

    if (toFile) {
        fout.close();
        cout << "  [OK] Данные сохранены в файл vulnerabilities.txt" << endl;
    }
}

void addVulnerability() {
    int id = inputInt("  ID уязвимости: ");
    string name = inputStr("  Название: ");
    string desc = inputStr("  Описание: ");
    string prio = inputStr("  Приоритет (Low/Medium/High/Critical): ");
    string stat = inputStr("  Статус: ");
    string date = inputStr("  Дата: ");
    string steps = inputStr("  Шаги воспроизведения: ");
    double cvss = stod(inputStr("  CVSS (0.0-10.0): "));
    string type = inputStr("  Тип уязвимости (SQL Injection/XSS/...): ");

    int appId = inputInt("  ID приложения: ");
    Application* app = findAppById(appId);
    if (!app) { cout << "  [!] Приложение не найдено!" << endl; return; }

    int tid = inputInt("  ID тестировщика: ");
    Tester* tester = findTesterById(tid);

    vulnerabilitiesList.push_back(
        Vulnerability(id, name, desc, prio, stat, date, steps, app, tester, cvss, type));
    cout << "  [OK] Уязвимость добавлена в list." << endl;
}

void removeVulnerability() {
    int id = inputInt("  ID уязвимости для удаления: ");
    auto it = find_if(vulnerabilitiesList.begin(), vulnerabilitiesList.end(),
        [id](const Vulnerability& v) { return v.getId() == id; });
    if (it != vulnerabilitiesList.end()) {
        vulnerabilitiesList.erase(it);
        cout << "  [OK] Уязвимость удалена из list." << endl;
    }
    else {
        cout << "  [!] Не найдено." << endl;
    }
}

void editVulnerability() {
    int id = inputInt("  ID уязвимости для редактирования: ");
    for (auto& v : vulnerabilitiesList) {
        if (v.getId() == id) {
            v.setName(inputStr("  Новое название (" + v.getName() + "): "));
            v.setStatus(inputStr("  Новый статус (" + v.getStatus() + "): "));
            v.setCvssScore(stod(inputStr("  Новый CVSS: ")));
            cout << "  [OK] Уязвимость обновлена." << endl;
            return;
        }
    }
    cout << "  [!] Не найдено." << endl;
}

void searchVulnerability() {
    string key = inputStr("  Введите часть названия: ");
    bool found = false;
    for (const auto& v : vulnerabilitiesList) {
        if (v.getName().find(key) != string::npos) {
            v.printInfo();
            found = true;
        }
    }
    if (!found) cout << "  [!] Ничего не найдено." << endl;
}

void sortVulnerabilities() {
    // Используем ВСТРОЕННЫЙ метод list.sort() — преимущество std::list
    vulnerabilitiesList.sort([](const Vulnerability& a, const Vulnerability& b) {
        return a.getCvssScore() > b.getCvssScore(); // по убыванию CVSS
        });
    cout << "  [OK] Список уязвимостей отсортирован по CVSS (убывание)." << endl;
}

// array<Tester, 5>
void printTesters(bool toFile) {
    ostream* out = &cout;
    ofstream fout;
    if (toFile) { fout.open("testers.txt"); out = &fout; }

    *out << left << setw(5) << "ID"
        << setw(25) << "ФИО"
        << setw(20) << "Специализация" << endl;
    *out << string(50, '-') << endl;
    for (const auto& t : testersArray) {
        if (t.getId() == 0) {
            *out << left << setw(5) << "-" << setw(25) << "(пустая ячейка)" << endl;
            continue;
        }
        *out << left << setw(5) << t.getId()
            << setw(25) << t.getFullName()
            << setw(20) << t.getSpecialization() << endl;
    }

    if (toFile) {
        fout.close();
        cout << "  [OK] Данные сохранены в файл testers.txt" << endl;
    }
}

void addTester() {
    // Ищем первую "пустую" ячейку (id == 0)
    for (auto& t : testersArray) {
        if (t.getId() == 0) {
            int id = inputInt("  ID тестировщика: ");
            string nm = inputStr("  ФИО: ");
            string sp = inputStr("  Специализация: ");
            t = Tester(id, nm, sp);
            cout << "  [OK] Тестировщик добавлен в array." << endl;
            return;
        }
    }
    cout << "  [!] Массив заполнен (фиксированный размер 5). Сначала удалите кого-то." << endl;
}

void removeTester() {
    int id = inputInt("  ID тестировщика для удаления: ");
    for (auto& t : testersArray) {
        if (t.getId() == id) {
            t = Tester(); // сброс ячейки
            cout << "  [OK] Тестировщик удалён (ячейка очищена)." << endl;
            return;
        }
    }
    cout << "  [!] Не найдено." << endl;
}

void editTester() {
    int id = inputInt("  ID тестировщика для редактирования: ");
    for (auto& t : testersArray) {
        if (t.getId() == id) {
            string nm = inputStr("  Новое ФИО (" + t.getFullName() + "): ");
            string sp = inputStr("  Новая специализация (" + t.getSpecialization() + "): ");
            t = Tester(id, nm, sp);
            cout << "  [OK] Обновлено." << endl;
            return;
        }
    }
    cout << "  [!] Не найдено." << endl;
}

void searchTester() {
    string key = inputStr("  Введите часть ФИО: ");
    bool found = false;
    for (const auto& t : testersArray) {
        if (t.getId() != 0 && t.getFullName().find(key) != string::npos) {
            t.printInfo();
            cout << endl;
            found = true;
        }
    }
    if (!found) cout << "  [!] Ничего не найдено." << endl;
}

void sortTesters() {
    sort(testersArray.begin(), testersArray.end(),
        [](const Tester& a, const Tester& b) { return a.getId() < b.getId(); });
    cout << "  [OK] Массив тестировщиков отсортирован по ID." << endl;
}

// menu functions
void menuVector() {
    int ch;
    do {
        printSeparator();
        cout << "  >>> VECTOR<Bug*> — База багов (текущий размер: "
            << bugsVector.size() << ")\n";
        cout << "  1. Добавить баг\n"
            << "  2. Удалить баг\n"
            << "  3. Редактировать баг\n"
            << "  4. Вывести на экран\n"
            << "  5. Сохранить в файл bugs.txt\n"
            << "  6. Поиск по названию\n"
            << "  7. Сортировать\n"
            << "  0. Назад\n";
        ch = inputInt("  Выбор: ");
        switch (ch) {
        case 1: addBug();         break;
        case 2: removeBug();      break;
        case 3: editBug();        break;
        case 4: printBugs();      break;
        case 5: printBugs(true);  break;
        case 6: searchBug();      break;
        case 7: sortBugs();       break;
        }
    } while (ch != 0);
}

void menuList() {
    int ch;
    do {
        printSeparator();
        cout << "  >>> LIST<Vulnerability> — Уязвимости (размер: "
            << vulnerabilitiesList.size() << ")\n";
        cout << "  1. Добавить\n"
            << "  2. Удалить\n"
            << "  3. Редактировать\n"
            << "  4. Вывести на экран\n"
            << "  5. Сохранить в файл vulnerabilities.txt\n"
            << "  6. Поиск по названию\n"
            << "  7. Сортировать (по CVSS)\n"
            << "  0. Назад\n";
        ch = inputInt("  Выбор: ");
        switch (ch) {
        case 1: addVulnerability();    break;
        case 2: removeVulnerability(); break;
        case 3: editVulnerability();   break;
        case 4: printVulns();          break;
        case 5: printVulns(true);      break;
        case 6: searchVulnerability(); break;
        case 7: sortVulnerabilities(); break;
        }
    } while (ch != 0);
}

void menuArray() {
    int ch;
    do {
        printSeparator();
        cout << "  >>> ARRAY<Tester, 5> — Штат тестировщиков\n";
        cout << "  1. Добавить (в свободную ячейку)\n"
            << "  2. Удалить (очистить ячейку)\n"
            << "  3. Редактировать\n"
            << "  4. Вывести на экран\n"
            << "  5. Сохранить в файл testers.txt\n"
            << "  6. Поиск по ФИО\n"
            << "  7. Сортировать по ID\n"
            << "  0. Назад\n";
        ch = inputInt("  Выбор: ");
        switch (ch) {
        case 1: addTester();    break;
        case 2: removeTester(); break;
        case 3: editTester();   break;
        case 4: printTesters(); break;
        case 5: printTesters(true); break;
        case 6: searchTester(); break;
        case 7: sortTesters();  break;
        }
    } while (ch != 0);
}