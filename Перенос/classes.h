#pragma once
#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstring>
#include <limits>
#include <sstream>
#include <cctype>

struct AthleteRecord {
    char name[50];
    int  age;
    char gender;
    char sport[50];
    char rank[50];
    double result;
    bool isDeleted;
};

struct CoachRecord {
    char name[50];
    int  age;
    char gender;
    char specialization[50];
    int  experienceYears;
    bool isDeleted;
};

inline void safeCopy(char* dst, const std::string& src, size_t size) {
    size_t len = src.length();
    if (len >= size) {
        len = size - 1;
    }
    for (size_t i = 0; i < len; ++i) {
        dst[i] = src[i];
    }
    dst[len] = '\0';
}

class Person {
protected:
    std::string fullName;
    int age;
    char gender;

public:
    Person() : age(0), gender('M') {}
    Person(const std::string& fullName, int age, char gender)
        : fullName(fullName), age(age), gender(gender) {
    }
    virtual ~Person() {}

    std::string getName() const { return fullName; }
    int getAge() const { return age; }
    char getGender() const { return gender; }

    void setName(const std::string& n) { fullName = n; }
    void setAge(int a) { age = a; }
    void setGender(char g) { gender = g; }

    virtual void print(std::ostream& os) const {
        os << "ФИО: " << fullName << ", Возраст: " << age << ", Пол: " << gender;
    }
};

class Athlete : public Person {
private:
    std::string sport;
    std::string rank;
    double result;

public:
    Athlete() : result(0.0) {}
    Athlete(const std::string& fullName, int age, char gender,
        const std::string& sport, const std::string& rank, double result)
        : Person(fullName, age, gender), sport(sport), rank(rank), result(result) {
    }

    std::string getSport() const { return sport; }
    std::string getRank() const { return rank; }
    double getResult() const { return result; }

    void setSport(const std::string& s) { sport = s; }
    void setRank(const std::string& r) { rank = r; }
    void setResult(double r) { result = r; }

    void print(std::ostream& os) const override {
        Person::print(os);
        os << ", Спорт: " << sport << ", Разряд: " << rank << ", Результат: " << result;
    }

    friend std::ostream& operator<<(std::ostream& os, const Athlete& a) {
        AthleteRecord rec{};
        safeCopy(rec.name, a.getName(), sizeof(rec.name));
        rec.age = a.getAge();
        rec.gender = a.getGender();
        safeCopy(rec.sport, a.getSport(), sizeof(rec.sport));
        safeCopy(rec.rank, a.getRank(), sizeof(rec.rank));
        rec.result = a.getResult();
        rec.isDeleted = false;
        os.write(reinterpret_cast<const char*>(&rec), sizeof(AthleteRecord));
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Athlete& a) {
        AthleteRecord rec{};
        is.read(reinterpret_cast<char*>(&rec), sizeof(AthleteRecord));
        if (is.good()) {
            a.setName(rec.name);
            a.setAge(rec.age);
            a.setGender(rec.gender);
            a.setSport(rec.sport);
            a.setRank(rec.rank);
            a.setResult(rec.result);
        }
        return is;
    }
};

class Coach : public Person {
private:
    std::string specialization;
    int experienceYears;

public:
    Coach() : experienceYears(0) {}
    Coach(const std::string& fullName, int age, char gender,
        const std::string& specialization, int experienceYears)
        : Person(fullName, age, gender),
        specialization(specialization),
        experienceYears(experienceYears) {
    }

    std::string getSpecialization() const { return specialization; }
    int getExperience() const { return experienceYears; }

    void setSpecialization(const std::string& s) { specialization = s; }
    void setExperience(int e) { experienceYears = e; }

    void print(std::ostream& os) const override {
        Person::print(os);
        os << ", Специализация: " << specialization << ", Стаж: " << experienceYears;
    }

    friend std::ostream& operator<<(std::ostream& os, const Coach& c) {
        CoachRecord rec{};
        safeCopy(rec.name, c.getName(), sizeof(rec.name));
        rec.age = c.getAge();
        rec.gender = c.getGender();
        safeCopy(rec.specialization, c.getSpecialization(), sizeof(rec.specialization));
        rec.experienceYears = c.getExperience();
        rec.isDeleted = false;
        os.write(reinterpret_cast<const char*>(&rec), sizeof(CoachRecord));
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Coach& c) {
        CoachRecord rec{};
        is.read(reinterpret_cast<char*>(&rec), sizeof(CoachRecord));
        if (is.good()) {
            c.setName(rec.name);
            c.setAge(rec.age);
            c.setGender(rec.gender);
            c.setSpecialization(rec.specialization);
            c.setExperience(rec.experienceYears);
        }
        return is;
    }
};

class Team {
private:
    static const int MAX_ATHLETES = 100;
    static const int MAX_COACHES = 20;

    std::string teamName;
    std::string city;
    Athlete athletes[MAX_ATHLETES];
    int athletesCount = 0;
    Coach* coaches[MAX_COACHES] = {};
    int coachesCount = 0;

public:
    Team() = default;
    Team(const std::string& name, const std::string& city)
        : teamName(name), city(city) {
    }

    std::string getTeamName() const { return teamName; }
    std::string getCity() const { return city; }
    int athleteCount_get() const { return athletesCount; }
    int coachCount_get() const { return coachesCount; }

    void setTeamName(const std::string& n) { teamName = n; }
    void setCity(const std::string& c) { city = c; }

    bool addAthlete(const Athlete& a) {
        if (athletesCount >= MAX_ATHLETES) return false;
        athletes[athletesCount++] = a;
        return true;
    }

    bool removeAthlete(int index) {
        if (index < 0 || index >= athletesCount) return false;
        for (int i = index; i < athletesCount - 1; ++i)
            athletes[i] = athletes[i + 1];
        --athletesCount;
        return true;
    }

    Athlete& getAthlete(int i) { return athletes[i]; }
    const Athlete& getAthlete(int i) const { return athletes[i]; }

    bool addCoach(Coach* c) {
        if (!c || coachesCount >= MAX_COACHES) return false;
        coaches[coachesCount++] = c;
        return true;
    }

    bool removeCoach(int index) {
        if (index < 0 || index >= coachesCount) return false;
        for (int i = index; i < coachesCount - 1; ++i)
            coaches[i] = coaches[i + 1];
        --coachesCount;
        return true;
    }

    Coach* getCoach(int i) const {
        return (i >= 0 && i < coachesCount) ? coaches[i] : nullptr;
    }

    void sortAthletes() {
        for (int i = 0; i < athletesCount - 1; ++i) {
            for (int j = 0; j < athletesCount - 1 - i; ++j) {
                if (athletes[j].getName() > athletes[j + 1].getName()) {
                    Athlete tmp = athletes[j];
                    athletes[j] = athletes[j + 1];
                    athletes[j + 1] = tmp;
                }
            }
        }
    }

    int searchAthleteByName(const std::string& name) const {
        int found = 0;
        for (int i = 0; i < athletesCount; ++i) {
            if (athletes[i].getName() == name) {
                std::cout << "  [" << i << "] ";
                athletes[i].print(std::cout);
                std::cout << "\n";
                ++found;
            }
        }
        return found;
    }

    void print(std::ostream& os) const {
        os << "Команда: " << teamName << " (" << city << ")\n";
        os << "  Спортсмены (" << athletesCount << "):\n";
        for (int i = 0; i < athletesCount; ++i) {
            os << "    [" << i << "] ";
            athletes[i].print(os);
            os << "\n";
        }
        os << "  Тренеры (" << coachesCount << "):\n";
        for (int i = 0; i < coachesCount; ++i) {
            os << "    [" << i << "] ";
            coaches[i]->print(os);
            os << "\n";
        }
    }
};

class Competition {
private:
    static const int MAX_TEAMS = 50;
    std::string name;
    std::string date;
    std::string place;
    Team* teams[MAX_TEAMS] = {};
    int teamsCount = 0;

public:
    Competition() = default;
    Competition(const std::string& name, const std::string& date, const std::string& place)
        : name(name), date(date), place(place) {
    }

    std::string getName() const { return name; }
    std::string getDate() const { return date; }
    std::string getPlace() const { return place; }
    int teamCount_get() const { return teamsCount; }

    void setName(const std::string& n) { name = n; }
    void setDate(const std::string& d) { date = d; }
    void setPlace(const std::string& p) { place = p; }

    bool addTeam(Team* t) {
        if (!t || teamsCount >= MAX_TEAMS) return false;
        teams[teamsCount++] = t;
        return true;
    }

    bool removeTeam(int index) {
        if (index < 0 || index >= teamsCount) return false;
        for (int i = index; i < teamsCount - 1; ++i)
            teams[i] = teams[i + 1];
        --teamsCount;
        return true;
    }

    Team* getTeam(int i) const {
        return (i >= 0 && i < teamsCount) ? teams[i] : nullptr;
    }

    void print(std::ostream& os) const {
        os << "Соревнование: " << name << " | Дата: " << date << " | Место: " << place << "\n";
        for (int i = 0; i < teamsCount; ++i) {
            os << "--- Команда #" << i << " ---\n";
            teams[i]->print(os);
        }
    }
};