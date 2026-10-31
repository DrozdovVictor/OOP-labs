#include<iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include"functions.h"
#include"classes.h"
#include"inputs.h"

using namespace std;

const int MAX_TEAMS = 50;
const int MAX_COACHES = 100;

Team* allTeams[MAX_TEAMS] = {};
int teamCount = 0;

Coach* allCoaches[MAX_COACHES] = {};
int coachCount = 0;

Competition competition("Чемпионат мира", "2026-10-16", "Минск");

void showMenu() {
    cout << "\n========== МЕНЮ ==========\n"
        << " 1. Просмотр всех данных\n"
        << " 2. Добавить команду\n"
        << " 3. Добавить спортсмена в команду\n"
        << " 4. Добавить тренера в команду\n"
        << " 5. Удалить команду\n"
        << " 6. Удалить спортсмена\n"
        << " 7. Удалить тренера из команды\n"
        << " 8. Редактировать команду\n"
        << " 9. Редактировать спортсмена\n"
        << "10. Редактировать тренера\n"
        << "11. Сортировать спортсменов по ФИО\n"
        << "12. Поиск спортсмена по ФИО\n"
        << "13. [ФАЙЛ] Добавить спортсмена в файл\n"
        << "14. [ФАЙЛ] Просмотреть спортсменов из файла\n"
        << "15. [ФАЙЛ] Редактировать спортсмена в файле\n"
        << "16. [ФАЙЛ] Удалить спортсмена из файла\n"
        << "17. [ФАЙЛ] Поиск спортсмена в файле\n"
        << " 0. Выход\n"
        << "==========================\n";
}

Team* selectTeam() {
    if (teamCount == 0) {
        cout << "Нет ни одной команды.\n";
        return nullptr;
    }
    cout << "Доступные команды:\n";
    for (int i = 0; i < teamCount; ++i) {
        cout << "  [" << i << "] " << allTeams[i]->getTeamName()
            << " (" << allTeams[i]->getCity() << ")\n";
    }
    int idx = inputInt("Введите номер команды: ");
    if (idx < 0 || idx >= teamCount) {
        cout << "Неверный номер.\n";
        return nullptr;
    }
    return allTeams[idx];
}

void viewAll() {
    cout << "\n===================== ВСЕ ДАННЫЕ =====================\n";

    // Флаги выравнивания
    cout.setf(ios::left, ios::adjustfield);
    cout << "Соревнование: " << competition.getName()
        << " | Дата: " << competition.getDate()
        << " | Место: " << competition.getPlace() << "\n\n";

    if (teamCount == 0) {
        cout << "Команд нет.\n";
        return;
    }

    for (int t = 0; t < teamCount; ++t) {
        Team* team = allTeams[t];
        cout << "  +- Команда #" << t << ": " << team->getTeamName()
            << " (" << team->getCity() << ")\n";

        cout << "  | Спортсмены (" << team->athleteCount_get() << "):\n";
        cout << "  | "
            << setw(4) << left << "#"
            << setw(25) << left << "ФИО"
            << setw(6) << right << "Возр"
            << setw(5) << right << "Пол"
            << setw(15) << left << "Спорт"
            << setw(15) << left << "Разряд"
            << setw(10) << right << "Рез." << "\n";
        cout << "  | " << string(80, '-') << "\n";

        // Флаги для вывода чисел
        cout.setf(ios::fixed, ios::floatfield);
        cout.setf(ios::showpoint);

        for (int a = 0; a < team->athleteCount_get(); ++a) {
            const Athlete& ath = team->getAthlete(a);
            cout << "  | "
                << setw(4) << left << a
                << setw(25) << left << ath.getName()
                << setw(6) << right << ath.getAge()
                << setw(5) << right << ath.getGender()
                << setw(15) << left << ath.getSport()
                << setw(15) << left << ath.getRank()
                << setw(10) << right << setprecision(2) << ath.getResult() << "\n";
        }

        cout.unsetf(ios::fixed);
        cout.unsetf(ios::showpoint);

        cout << "  | Тренеры (" << team->coachCount_get() << "):\n";
        for (int c = 0; c < team->coachCount_get(); ++c) {
            cout << "  |    [" << c << "] ";
            team->getCoach(c)->print(cout);
            cout << "\n";
        }
        cout << "  +---------------------------------------\n\n";
    }
}

void addTeam() {
    if (teamCount >= MAX_TEAMS) {
        cout << "Достигнут лимит команд.\n";
        return;
    }
    string name = inputLine("Название команды: ");
    string city = inputLine("Город: ");
    Team* newTeam = new Team(name, city);
    if (!competition.addTeam(newTeam)) {
        delete newTeam;
        cout << "Не удалось добавить в соревнование.\n";
        return;
    }
    allTeams[teamCount++] = newTeam;
    cout << "Команда создана.\n";
}

void addAthlete() {
    Team* team = selectTeam();
    if (!team) return;
    string name = inputLine("ФИО: ");
    int age = inputInt("Возраст: ");
    char gender = inputChar("Пол (M/F): ");
    string sport = inputLine("Спорт: ");
    string rank = inputLine("Разряд: ");
    double result = inputDouble("Результат: ");
    Athlete a(name, age, gender, sport, rank, result);
    if (team->addAthlete(a)) cout << "Спортсмен добавлен.\n";
    else cout << "Переполнение.\n";
}

void addCoachToTeam() {
    Team* team = selectTeam();
    if (!team) return;
    string name = inputLine("ФИО: ");
    int age = inputInt("Возраст: ");
    char gender = inputChar("Пол (M/F): ");
    string spec = inputLine("Специализация: ");
    int exp = inputInt("Стаж: ");
    Coach* c = new Coach(name, age, gender, spec, exp);
    if (team->addCoach(c)) {
        allCoaches[coachCount++] = c;
        cout << "Тренер добавлен.\n";
    }
    else {
        delete c;
        cout << "Ошибка добавления.\n";
    }
}

void removeTeam() {
    if (teamCount == 0) { cout << "Нет команд.\n"; return; }
    for (int i = 0; i < teamCount; ++i)
        cout << "[" << i << "] " << allTeams[i]->getTeamName() << "\n";
    int idx = inputInt("Номер: ");
    if (idx < 0 || idx >= teamCount) { cout << "Неверно.\n"; return; }

    for (int i = 0; i < competition.teamCount_get(); ++i) {
        if (competition.getTeam(i) == allTeams[idx]) {
            competition.removeTeam(i);
            break;
        }
    }
    delete allTeams[idx];
    for (int i = idx; i < teamCount - 1; ++i)
        allTeams[i] = allTeams[i + 1];
    --teamCount;
    cout << "Удалено.\n";
}

void removeAthlete() {
    Team* team = selectTeam();
    if (!team) return;
    for (int i = 0; i < team->athleteCount_get(); ++i)
        cout << "[" << i << "] " << team->getAthlete(i).getName() << "\n";
    int idx = inputInt("Номер: ");
    if (team->removeAthlete(idx)) cout << "Удалено.\n";
    else cout << "Ошибка.\n";
}

void removeCoachFromTeam() {
    Team* team = selectTeam();
    if (!team) return;
    for (int i = 0; i < team->coachCount_get(); ++i)
        cout << "[" << i << "] " << team->getCoach(i)->getName() << "\n";
    int idx = inputInt("Номер: ");
    if (team->removeCoach(idx)) cout << "Убран.\n";
    else cout << "Ошибка.\n";
}

void editTeam() {
    Team* team = selectTeam();
    if (!team) return;
    cout << "1. Название\n2. Город\n";
    int f = inputInt("Что менять: ");
    if (f == 1) team->setTeamName(inputLine("Новое: "));
    else if (f == 2) team->setCity(inputLine("Новое: "));
    cout << "Готово.\n";
}

void editAthlete() {
    Team* team = selectTeam();
    if (!team) return;
    for (int i = 0; i < team->athleteCount_get(); ++i)
        cout << "[" << i << "] " << team->getAthlete(i).getName() << "\n";
    int idx = inputInt("Номер: ");
    if (idx < 0 || idx >= team->athleteCount_get()) return;
    Athlete& a = team->getAthlete(idx);

    cout << "1.ФИО 2.Возраст 3.Пол 4.Спорт 5.Разряд 6.Результат\n";
    int f = inputInt("Поле: ");
    switch (f) {
    case 1: a.setName(inputLine("Новое: ")); break;
    case 2: a.setAge(inputInt("Новое: ")); break;
    case 3: a.setGender(inputChar("Новое: ")); break;
    case 4: a.setSport(inputLine("Новое: ")); break;
    case 5: a.setRank(inputLine("Новое: ")); break;
    case 6: a.setResult(inputDouble("Новое: ")); break;
    }
    cout << "Готово.\n";
}

void editCoach() {
    if (coachCount == 0) { cout << "Нет тренеров.\n"; return; }
    for (int i = 0; i < coachCount; ++i)
        cout << "[" << i << "] " << allCoaches[i]->getName() << "\n";
    int idx = inputInt("Номер: ");
    if (idx < 0 || idx >= coachCount) return;
    Coach* c = allCoaches[idx];
    cout << "1.ФИО 2.Возраст 3.Пол 4.Спец 5.Стаж\n";
    int f = inputInt("Поле: ");
    switch (f) {
    case 1: c->setName(inputLine("Новое: ")); break;
    case 2: c->setAge(inputInt("Новое: ")); break;
    case 3: c->setGender(inputChar("Новое: ")); break;
    case 4: c->setSpecialization(inputLine("Новое: ")); break;
    case 5: c->setExperience(inputInt("Новое: ")); break;
    }
    cout << "Готово.\n";
}

void sortAthletes() {
    Team* team = selectTeam();
    if (!team) return;
    team->sortAthletes();
    cout << "Отсортировано.\n";
    for (int i = 0; i < team->athleteCount_get(); ++i) {
        cout << "[" << i << "] " << team->getAthlete(i).getName() << "\n";
    }
}

void searchAthlete() {
    Team* team = selectTeam();
    if (!team) return;
    string name = inputLine("ФИО: ");
    int found = team->searchAthleteByName(name);
    if (found == 0) cout << "Не найдено.\n";
    else cout << "Найдено: " << found << "\n";
}

void fileAddAthlete() {
    ofstream fout;
    fout.open("athletes.dat", ios::binary | ios::app);
    if (!fout.is_open()) {
        cout << "Ошибка открытия файла!\n";
        return;
    }
    string name = inputLine("ФИО: ");
    int age = inputInt("Возраст: ");
    char gender = inputChar("Пол (M/F): ");
    string sport = inputLine("Спорт: ");
    string rank = inputLine("Разряд: ");
    double result = inputDouble("Результат: ");

    Athlete a(name, age, gender, sport, rank, result);
    fout << a;
    fout.close();

    if (fout.good()) cout << "Успешно записано.\n";
    else cout << "Ошибка записи.\n";
}

void fileViewAthletes() {
    ifstream fin;
    fin.open("athletes.dat", ios::binary);
    if (!fin.is_open()) {
        cout << "Файл пуст или не существует.\n";
        return;
    }

    // seekg + tellg для размера
    fin.seekg(0, ios::end);
    int fileSize = fin.tellg();
    fin.seekg(0, ios::beg);

    int count = fileSize / sizeof(AthleteRecord);
    cout << "Всего записей: " << count << "\n\n";

    AthleteRecord rec;
    for (int i = 0; i < count; ++i) {
        fin.read(reinterpret_cast<char*>(&rec), sizeof(AthleteRecord));
        if (fin.eof()) break;
        if (!rec.isDeleted) {
            cout << "[" << i << "] " << rec.name
                << " | Спорт: " << rec.sport
                << " | Результат: " << rec.result << "\n";
        }
    }
    fin.close();
}

void fileEditAthlete() {
    fstream file;
    file.open("athletes.dat", ios::binary | ios::in | ios::out);
    if (!file.is_open()) {
        cout << "Ошибка файла.\n";
        return;
    }

    int index = inputInt("Индекс для редактирования: ");

    // seekp для перемещения указателя записи
    file.seekp(index * sizeof(AthleteRecord), ios::beg);

    string name = inputLine("Новое ФИО: ");
    int age = inputInt("Возраст: ");
    char gender = inputChar("Пол: ");
    string sport = inputLine("Спорт: ");
    string rank = inputLine("Разряд: ");
    double result = inputDouble("Результат: ");

    Athlete a(name, age, gender, sport, rank, result);
    file << a;
    if (file.good()) cout << "Отредактировано.\n";
    if (file.bad()) cout << "Критическая ошибка.\n";
    file.close();
}

void fileDeleteAthlete() {
    fstream file;
    file.open("athletes.dat", ios::binary | ios::in | ios::out);
    if (!file.is_open()) return;

    int index = inputInt("Индекс для удаления: ");
    file.seekp(index * sizeof(AthleteRecord), ios::beg);

    AthleteRecord rec{};
    rec.isDeleted = true; // логическое удаление
    file.write(reinterpret_cast<char*>(&rec), sizeof(AthleteRecord));

    if (file.good()) cout << "Помечено как удалённое.\n";
    file.close();
}

void fileSearchAthlete() {
    ifstream fin("athletes.dat", ios::binary);
    if (!fin.is_open()) return;

    string searchName = inputLine("ФИО для поиска: ");
    AthleteRecord rec;
    bool found = false;

    while (fin.read(reinterpret_cast<char*>(&rec), sizeof(AthleteRecord))) {
        if (fin.fail()) break;
        if (!rec.isDeleted && searchName == rec.name) {
            cout << "НАЙДЕН! Спорт: " << rec.sport
                << ", Разряд: " << rec.rank
                << ", Результат: " << rec.result << "\n";
            found = true;
        }
    }
    if (fin.eof()) cout << "(Достигнут конец файла)\n";
    if (!found) cout << "Не найден.\n";
    fin.close();
}

void cleanup() {
    for (int i = 0; i < teamCount; ++i) { delete allTeams[i]; allTeams[i] = nullptr; }
    teamCount = 0;
    for (int i = 0; i < coachCount; ++i) { delete allCoaches[i]; allCoaches[i] = nullptr; }
    coachCount = 0;
    cout << "Память освобождена.\n";
}