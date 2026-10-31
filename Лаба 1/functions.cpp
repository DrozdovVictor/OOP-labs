#include<iostream>
#include<fstream>
#include <sstream> 
#include<iomanip>
#include"classes.h"
#include"functions.h"
#include"inputs.h"

static const int MAX_TEAMS = 50;
static const int MAX_COACHES = 500;

Team* allTeams[MAX_TEAMS] = {};
int teamCount = 0;

Coach* allCoaches[MAX_COACHES] = {};
int coachCount = 0;

Competition competition("Чемпионат мира", "2026-10-16", "Минск");

void showMainMenu() {
    std::cout << "\n========== МЕНЮ ==========\n"
        << "1. Просмотр всего\n"
        << "2. Добавить команду\n"
        << "3. Добавить спортсмена в команду\n"
        << "4. Добавить тренера в команду\n"
        << "5. Удалить команду\n"
        << "6. Удалить спортсмена\n"
        << "7. Удалить тренера из команды\n"
        << "8. Редактировать команду\n"
        << "9. Редактировать спортсмена\n"
        << "10. Редактировать тренера\n"
        << "11. Сортировка спортсменов\n"
        << "12. Поиск спортсмена\n"
        << "13. Сохранить в файл\n"
        << "14. Загрузить из файла\n"
        << "0. Выход\n"
        << "==========================\n";
}

void viewAll() {
    std::cout << "\n===================== ВСЕ ДАННЫЕ =====================\n";

    std::cout.setf(std::ios::left, std::ios::adjustfield);
    std::cout << "Соревнование: " << competition.getName()
        << " | Дата: " << competition.getDate()
        << " | Место: " << competition.getPlace()
        << "\n\n";

    if (teamCount == 0) {
        std::cout << "Команд нет.\n";
        return;
    }

    for (int t = 0; t < teamCount; ++t) {
        Team* team = allTeams[t];

        std::cout << "  +- Команда #" << t << ": "
            << team->getTeamName()
            << " (" << team->getCity() << ")\n";

        std::cout << "  | Спортсмены (" << team->athleteCount_get() << "):\n";

        // ===== Шапка =====
        std::cout << "  | "
            << std::setw(4) << std::left << "#"
            << std::setw(25) << std::left << "ФИО"
            << std::setw(6) << std::right << "Возр"
            << std::setw(5) << std::right << "Пол"
            << std::setw(15) << std::left << "Спорт"
            << std::setw(15) << std::left << "Разряд"
            << std::setw(10) << std::right << "Рез."
            << "\n";
        std::cout << "  | " << std::string(80, '-') << "\n";

        // ===== Флаги =====
        std::cout.setf(std::ios::fixed, std::ios::floatfield);
        std::cout.setf(std::ios::showpoint);

        // ===== Данные =====
        for (int a = 0; a < team->athleteCount_get(); ++a) {
            const Athlete& ath = team->getAthlete(a);

            std::cout << "  | "
                << std::setw(4) << std::left << a
                << std::setw(25) << std::left << ath.getName()
                << std::setw(6) << std::right << ath.getAge()
                << std::setw(5) << std::right << ath.getGender()
                << std::setw(15) << std::left << ath.getSport()
                << std::setw(15) << std::left << ath.getRank()
                << std::setw(10) << std::right << std::setprecision(2) << ath.getResult()
                << "\n";
        }

        // ===== Сброс флагов =====
        std::cout.unsetf(std::ios::fixed);
        std::cout.unsetf(std::ios::showpoint);

        // ===== Тренеры =====
        std::cout << "  | Тренеры (" << team->coachCount_get() << "):\n";
        for (int c = 0; c < team->coachCount_get(); ++c) {
            Coach* co = team->getCoach(c);
            std::cout << "  |    [" << c << "] ";
            co->print(std::cout);
            std::cout << "\n";
        }

        std::cout << "  +---------------------------------------\n\n";
    }
}

void addTeam() {
    if (teamCount >= MAX_TEAMS) {
        std::cout << "Лимит команд исчерпан.\n";
        return;
    }

    std::string name = inputLine("Название команды: ");
    std::string city = inputLine("Город: ");

    Team* newTeam = new Team(name, city);

    if (!competition.addTeam(newTeam)) {
        delete newTeam;
        std::cout << "Соревнование переполнено.\n";
        return;
    }

    allTeams[teamCount++] = newTeam;
    std::cout << "Команда \"" << name << "\" создана.\n";
}

// Возвращает выбранную команду или nullptr, если выбор не удался
Team* selectTeam();
Team* selectTeam() {
    if (teamCount == 0) {
        std::cout << "Нет ни одной команды. Сначала создайте команду.\n";
        return nullptr;
    }

    std::cout << "Доступные команды:\n";
    for (int i = 0; i < teamCount; ++i) {
        std::cout << "  [" << i << "] "
            << allTeams[i]->getTeamName()
            << " (" << allTeams[i]->getCity() << ")\n";
    }

    int idx = inputInt("Введите номер команды: ");
    if (idx < 0 || idx >= teamCount) {
        std::cout << "Неверный номер.\n";
        return nullptr;
    }

    return allTeams[idx];
}

void addAthlete() {
    Team* team = selectTeam();
    if (!team) return;

    // ---- Спрашиваем данные спортсмена ----
    std::string fullName = inputLine("ФИО: ");
    int age = inputInt("Возраст: ");
    char gender = inputChar("Пол (M/F): ");

    // Валидация пола
    if (gender != 'M' && gender != 'F' &&
        gender != 'm' && gender != 'f') {
        std::cout << "Пол должен быть 'M' или 'F'.\n";
        return;
    }
    if (gender == 'm') gender = 'M';
    if (gender == 'f') gender = 'F';

    std::string sport = inputLine("Вид спорта: ");
    std::string rank = inputLine("Разряд: ");
    double result = inputDouble("Результат: ");

    // ---- Создаём и добавляем ----
    Athlete athlete(fullName, age, gender, sport, rank, result);

    if (team->addAthlete(athlete)) {
        std::cout << "Спортсмен добавлен в команду \""
            << team->getTeamName() << "\".\n";
    }
    else {
        std::cout << "Не удалось добавить: команда переполнена.\n";
    }
}
void addCoachToTeam() {
    Team* team = selectTeam();
    if (!team) return;

    // Ограничение: у команды не должно быть больше MAX_COACHES
    if (team->coachCount_get() >= 20) {   // 20 — это MAX_COACHES в Team
        std::cout << "У команды уже максимум тренеров.\n";
        return;
    }

    // ---- Спрашиваем данные ----
    std::string fullName = inputLine("ФИО: ");
    int         age = inputInt("Возраст: ");
    char        gender = inputChar("Пол (M/F): ");
    if (gender == 'm') gender = 'M';
    if (gender == 'f') gender = 'F';

    std::string specialization = inputLine("Специализация: ");
    int         experience = inputInt("Стаж (лет): ");

    // ---- Создаём Coach через new (агрегация!) ----
    Coach* coach = new Coach(fullName, age, gender, specialization, experience);

    // ---- Пытаемся добавить ----
    if (team->addCoach(coach)) {
        // И сохраняем в список владельца в main
        allCoaches[coachCount++] = coach;
        std::cout << "Тренер добавлен в команду \""
            << team->getTeamName() << "\".\n";
    }
    else {
        delete coach;   // не поместился — освобождаем
        std::cout << "Не удалось добавить тренера.\n";
    }
}

void removeTeam() {
    if (teamCount == 0) {
        std::cout << "Нет команд для удаления.\n";
        return;
    }

    // Показываем список
    std::cout << "Команды:\n";
    for (int i = 0; i < teamCount; ++i) {
        std::cout << "  [" << i << "] "
            << allTeams[i]->getTeamName() << "\n";
    }

    int idx = inputInt("Какую команду удалить? ");
    if (idx < 0 || idx >= teamCount) {
        std::cout << "Неверный номер.\n";
        return;
    }

    // 1. Убираем указатель из Competition (агрегация)
    //    Это можно сделать, найдя команду в списке по адресу
    for (int i = 0; i < competition.teamCount_get(); ++i) {
        if (competition.getTeam(i) == allTeams[idx]) {
            competition.removeTeam(i);
            break;
        }
    }

    // 2. Удаляем объект из кучи — main владелец
    delete allTeams[idx];

    // 3. Сдвигаем массив allTeams
    for (int i = idx; i < teamCount - 1; ++i) {
        allTeams[i] = allTeams[i + 1];
    }
    --teamCount;

    std::cout << "Команда удалена.\n";
}
void removeAthlete() {
    Team* team = selectTeam();
    if (!team) return;

    if (team->athleteCount_get() == 0) {
        std::cout << "В этой команде нет спортсменов.\n";
        return;
    }

    // Список спортсменов
    std::cout << "Спортсмены команды \"" << team->getTeamName() << "\":\n";
    for (int i = 0; i < team->athleteCount_get(); ++i) {
        std::cout << "  [" << i << "] " << team->getAthlete(i).getName() << "\n";
    }

    int idx = inputInt("Кого удалить? ");
    if (team->removeAthlete(idx)) {
        std::cout << "Спортсмен удалён.\n";
    }
    else {
        std::cout << "Неверный номер.\n";
    }
}
void removeCoachFromTeam() {
    Team* team = selectTeam();
    if (!team) return;

    if (team->coachCount_get() == 0) {
        std::cout << "В этой команде нет тренеров.\n";
        return;
    }

    // Список тренеров
    std::cout << "Тренеры команды \"" << team->getTeamName() << "\":\n";
    for (int i = 0; i < team->coachCount_get(); ++i) {
        std::cout << "  [" << i << "] " << team->getCoach(i)->getName() << "\n";
    }

    int idx = inputInt("Кого убрать из команды? ");
    if (team->removeCoach(idx)) {
        std::cout << "Тренер убран из команды.\n";
        std::cout << "(Объект тренера жив и доступен в общем списке.)\n";
    }
    else {
        std::cout << "Неверный номер.\n";
    }
}

void editTeam() {
    Team* team = selectTeam();
    if (!team) return;

    std::cout << "Что редактировать?\n"
        << "  1. Название\n"
        << "  2. Город\n"
        << "  0. Отмена\n";

    int field = inputInt("Выбор: ");
    switch (field) {
    case 1: {
        std::string v = inputLine("Новое название: ");
        team->setTeamName(v);
        break;
    }
    case 2: {
        std::string v = inputLine("Новый город: ");
        team->setCity(v);
        break;
    }
    case 0:
        std::cout << "Отмена.\n";
        return;
    default:
        std::cout << "Неверный пункт.\n";
        return;
    }
    std::cout << "Команда обновлена.\n";
}
void editAthlete() {
    Team* team = selectTeam();
    if (!team) return;

    if (team->athleteCount_get() == 0) {
        std::cout << "В команде нет спортсменов.\n";
        return;
    }

    std::cout << "Спортсмены:\n";
    for (int i = 0; i < team->athleteCount_get(); ++i) {
        std::cout << "  [" << i << "] "
            << team->getAthlete(i).getName() << "\n";
    }

    int idx = inputInt("Кого редактировать? ");
    if (idx < 0 || idx >= team->athleteCount_get()) {
        std::cout << "Неверный номер.\n";
        return;
    }

    // Получаем НЕконстантную ссылку — можно менять!
    Athlete& ath = team->getAthlete(idx);

    std::cout << "Что редактировать?\n"
        << "  1. ФИО\n"
        << "  2. Возраст\n"
        << "  3. Пол\n"
        << "  4. Вид спорта\n"
        << "  5. Разряд\n"
        << "  6. Результат\n"
        << "  0. Отмена\n";

    int field = inputInt("Выбор: ");
    switch (field) {
    case 1: ath.setName(inputLine("Новое ФИО: "));        break;
    case 2: ath.setAge(inputInt("Новый возраст: "));    break;
    case 3: ath.setGender(inputChar("Новый пол (M/F): "));  break;
    case 4: ath.setSport(inputLine("Новый вид спорта: ")); break;
    case 5: ath.setRank(inputLine("Новый разряд: "));     break;
    case 6: ath.setResult(inputDouble("Новый результат: "));  break;
    case 0: std::cout << "Отмена.\n"; return;
    default: std::cout << "Неверный пункт.\n"; return;
    }

    std::cout << "Спортсмен обновлён.\n";
}
void editCoach() {
    if (coachCount == 0) {
        std::cout << "Нет тренеров для редактирования.\n";
        return;
    }

    // Список всех тренеров
    std::cout << "Все тренеры:\n";
    for (int i = 0; i < coachCount; ++i) {
        std::cout << "  [" << i << "] " << allCoaches[i]->getName() << "\n";
    }

    int idx = inputInt("Кого редактировать? ");
    if (idx < 0 || idx >= coachCount) {
        std::cout << "Неверный номер.\n";
        return;
    }

    Coach* coach = allCoaches[idx];   // неконстантный указатель!

    // Меню редактирования
    std::cout << "Что редактировать?\n"
        << "  1. ФИО\n"
        << "  2. Возраст\n"
        << "  3. Пол\n"
        << "  4. Специализация\n"
        << "  5. Стаж\n"
        << "  0. Отмена\n";

    int field = inputInt("Выбор: ");
    switch (field) {
    case 1: {
        std::string v = inputLine("Новое ФИО: ");
        coach->setName(v);
        break;
    }
    case 2: {
        int v = inputInt("Новый возраст: ");
        coach->setAge(v);
        break;
    }
    case 3: {
        char v = inputChar("Новый пол (M/F): ");
        coach->setGender(v);
        break;
    }
    case 4: {
        std::string v = inputLine("Новая специализация: ");
        coach->setSpecialization(v);
        break;
    }
    case 5: {
        int v = inputInt("Новый стаж: ");
        coach->setExperience(v);
        break;
    }
    case 0:
        std::cout << "Отмена.\n";
        return;
    default:
        std::cout << "Неверный пункт.\n";
        return;
    }

    std::cout << "Обновлено.\n";
}

void sortAthletes() {
    Team* team = selectTeam();
    if (!team) return;

    if (team->athleteCount_get() < 2) {
        std::cout << "Нечего сортировать: меньше двух спортсменов.\n";
        return;
    }

    team->sortAthletes();

    std::cout << "Спортсмены команды \"" << team->getTeamName()
        << "\" отсортированы по ФИО:\n";

    for (int i = 0; i < team->athleteCount_get(); ++i) {
        std::cout << "  [" << i << "] ";
        team->getAthlete(i).print(std::cout);
        std::cout << "\n";
    }
}
void searchAthlete() {
    Team* team = selectTeam();
    if (!team) return;

    if (team->athleteCount_get() == 0) {
        std::cout << "В команде нет спортсменов.\n";
        return;
    }

    std::string name = inputLine("Введите ФИО для поиска: ");

    std::cout << "\nРезультаты поиска в команде \""
        << team->getTeamName() << "\":\n";

    int found = team->searchAthleteByName(name);

    if (found == 0) {
        std::cout << "  Спортсмен с ФИО \"" << name << "\" не найден.\n";
    }
    else {
        std::cout << "  Найдено: " << found << "\n";
    }
}

void saveToFile() {
    std::string filename = inputLine("Имя файла для сохранения: ");

    std::ofstream fout(filename);
    if (!fout.is_open()) {
        std::cout << "Ошибка: не удалось открыть файл.\n";
        return;
    }

    // ---- Соревнование ----
    fout << "COMPETITION;"
        << competition.getName() << ";"
        << competition.getDate() << ";"
        << competition.getPlace() << "\n";

    // ---- Тренеры ----
    for (int i = 0; i < coachCount; ++i) {
        Coach* c = allCoaches[i];
        fout << "COACH;"
            << c->getName() << ";"
            << c->getAge() << ";"
            << c->getGender() << ";"
            << c->getSpecialization() << ";"
            << c->getExperience() << "\n";
    }

    // ---- Команды ----
    for (int t = 0; t < teamCount; ++t) {
        Team* team = allTeams[t];
        fout << "TEAM;"
            << team->getTeamName() << ";"
            << team->getCity() << "\n";

        // Спортсмены
        for (int a = 0; a < team->athleteCount_get(); ++a) {
            const Athlete& ath = team->getAthlete(a);
            fout << "ATHLETE;"
                << ath.getName() << ";"
                << ath.getAge() << ";"
                << ath.getGender() << ";"
                << ath.getSport() << ";"
                << ath.getRank() << ";"
                << ath.getResult() << "\n";
        }

        // Тренеры — сохраняем ИНДЕКСЫ в allCoaches
        for (int c = 0; c < team->coachCount_get(); ++c) {
            Coach* coach = team->getCoach(c);
            int idx = -1;
            for (int k = 0; k < coachCount; ++k) {
                if (allCoaches[k] == coach) { idx = k; break; }
            }
            fout << "COACH_LINK;" << idx << "\n";
        }

        fout << "END_TEAM\n";
    }
    fout << "END\n";

    fout.close();

    if (fout.fail()) {
        std::cout << "Внимание: при записи произошла ошибка.\n";
    }
    else {
        std::cout << "Данные сохранены в \"" << filename << "\".\n";
    }
}
void loadFromFile() {
    std::string filename = inputLine("Имя файла для загрузки: ");

    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cout << "Ошибка: не удалось открыть \"" << filename << "\".\n";
        return;
    }

    // ---- Очистка текущих данных ----
    for (int i = 0; i < coachCount; ++i) delete allCoaches[i];
    for (int i = 0; i < teamCount; ++i) delete allTeams[i];
    coachCount = 0;
    teamCount = 0;

    Team* currentTeam = nullptr;
    std::string line;

    while (std::getline(fin, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string token;
        std::getline(iss, token, ';');

        if (token == "COMPETITION") {
            std::string name, date, place;
            std::getline(iss, name, ';');
            std::getline(iss, date, ';');
            std::getline(iss, place, ';');
            competition.setName(name);
            competition.setDate(date);
            competition.setPlace(place);
        }
        else if (token == "COACH") {
            std::string name, ageStr, genStr, spec, expStr;
            std::getline(iss, name, ';');
            std::getline(iss, ageStr, ';');
            std::getline(iss, genStr, ';');
            std::getline(iss, spec, ';');
            std::getline(iss, expStr, ';');

            int  age = std::stoi(ageStr);
            char gender = genStr.empty() ? 'M' : genStr[0];
            int  exp = std::stoi(expStr);

            Coach* c = new Coach(name, age, gender, spec, exp);
            allCoaches[coachCount++] = c;
        }
        else if (token == "TEAM") {
            std::string name, city;
            std::getline(iss, name, ';');
            std::getline(iss, city, ';');
            currentTeam = new Team(name, city);
        }
        else if (token == "ATHLETE") {
            std::string name, ageStr, genStr, sport, rank, resStr;
            std::getline(iss, name, ';');
            std::getline(iss, ageStr, ';');
            std::getline(iss, genStr, ';');
            std::getline(iss, sport, ';');
            std::getline(iss, rank, ';');
            std::getline(iss, resStr, ';');

            int    age = std::stoi(ageStr);
            char   gender = genStr.empty() ? 'M' : genStr[0];
            double res = std::stod(resStr);

            if (currentTeam) {
                currentTeam->addAthlete(
                    Athlete(name, age, gender, sport, rank, res));
            }
        }
        else if (token == "COACH_LINK") {
            std::string idxStr;
            std::getline(iss, idxStr, ';');
            int idx = std::stoi(idxStr);
            if (currentTeam && idx >= 0 && idx < coachCount) {
                currentTeam->addCoach(allCoaches[idx]);
            }
        }
        else if (token == "END_TEAM") {
            if (currentTeam) {
                allTeams[teamCount++] = currentTeam;
                competition.addTeam(currentTeam);
                currentTeam = nullptr;
            }
        }
        else if (token == "END") {
            break;
        }
    }

    fin.close();

    if (fin.bad()) {
        std::cout << "Критическая ошибка потока (badbit).\n";
    }
    else if (fin.fail() && !fin.eof()) {
        std::cout << "Ошибка чтения файла.\n";
    }
    else {
        std::cout << "Данные загружены из \"" << filename << "\".\n";
    }
}

void cleanup() {
    // 1. Сначала команды — они держат указатели на тренеров,
    //    но не владеют ими, так что порядок не критичен.
    //    Однако логичнее сначала убрать всех, кто "смотрит" на тренеров.

    for (int i = 0; i < teamCount; ++i) {
        delete allTeams[i];
        allTeams[i] = nullptr;
    }
    teamCount = 0;

    // 2. Потом тренеры — их владелец main
    for (int i = 0; i < coachCount; ++i) {
        delete allCoaches[i];
        allCoaches[i] = nullptr;
    }
    coachCount = 0;

    std::cout << "Память освобождена.\n";
}
