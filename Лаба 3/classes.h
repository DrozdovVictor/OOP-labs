#pragma once
#include <string>
#include <iostream>

class BaseEntity {
protected:
    int id;
    std::string name;

public:
    BaseEntity(int id = 0, const std::string& name = "") : id(id), name(name) {}
    virtual ~BaseEntity() = default;

    int getId() const { return id; }
    std::string getName() const { return name; }
    void setName(const std::string& newName) { name = newName; }

    virtual void printInfo() const {
        std::cout << "ID: " << id << ", Название: " << name;
    }
};

class Application {
private:
    int id;
    std::string appName;
    std::string version;
    std::string platform;

public:
    Application(int id = 0, const std::string& name = "", const std::string& ver = "", const std::string& plat = "")
        : id(id), appName(name), version(ver), platform(plat) {
    }

    int getId() const { return id; }
    std::string getAppName() const { return appName; }
    std::string getVersion() const { return version; }

    void printInfo() const {
        std::cout << "Приложение: " << appName << " v" << version << " (" << platform << ")";
    }
};

class Tester {
private:
    int id;
    std::string fullName;
    std::string specialization;

public:
    Tester(int id = 0, const std::string& name = "", const std::string& spec = "")
        : id(id), fullName(name), specialization(spec) {
    }

    int getId() const { return id; }
    std::string getFullName() const { return fullName; }
    std::string getSpecialization() const { return specialization; }

    void printInfo() const {
        std::cout << fullName << " (" << specialization << ")";
    }

    // Для работы с контейнером std::array
    bool operator<(const Tester& other) const { return id < other.id; }
    bool operator==(const Tester& other) const { return id == other.id; }
};

class Bug : public BaseEntity {
protected:
    std::string description;
    std::string priority;  // "Low", "Medium", "High", "Critical"
    std::string status;    // "Open", "In Progress", "Resolved", "Closed"
    std::string dateFound;
    std::string reproductionSteps; // Шаги воспроизведения (вместо TestCase)

    Application* application; // Композиция
    Tester* assignedTester;   // Агрегация

public:
    Bug(int id = 0, const std::string& title = "", const std::string& desc = "",
        const std::string& prio = "Medium", const std::string& stat = "Open",
        const std::string& date = "", const std::string& steps = "",
        Application* app = nullptr, Tester* tester = nullptr)
        : BaseEntity(id, title), description(desc), priority(prio), status(stat),
        dateFound(date), reproductionSteps(steps), application(app), assignedTester(tester) {
    }

    // Геттеры
    std::string getPriority() const { return priority; }
    std::string getStatus() const { return status; }
    std::string getDescription() const { return description; }
    std::string getReproductionSteps() const { return reproductionSteps; }
    Application* getApplication() const { return application; }
    Tester* getAssignedTester() const { return assignedTester; }

    // Сеттеры
    void setPriority(const std::string& p) { priority = p; }
    void setStatus(const std::string& s) { status = s; }
    void setAssignedTester(Tester* t) { assignedTester = t; }

    // Перегрузка операторов для STL (сортировка)
    bool operator<(const Bug& other) const { return id < other.id; }
    bool operator==(const Bug& other) const { return id == other.id; }

    virtual void printInfo() const override {
        std::cout << "[" << id << "] " << name
            << " | Приоритет: " << priority
            << " | Статус: " << status << "\n";
        if (application) std::cout << "   " << "Приложение: " << application->getAppName() << "\n";
        if (assignedTester) std::cout << "   " << "Тестировщик: " << assignedTester->getFullName() << "\n";
    }
};

class Vulnerability : public Bug {
private:
    double cvssScore;       // Оценка от 0.0 до 10.0
    std::string vulnType;   // "SQL Injection", "XSS", "Buffer Overflow"

public:
    Vulnerability(int id = 0, const std::string& title = "", const std::string& desc = "",
        const std::string& prio = "Critical", const std::string& stat = "Open",
        const std::string& date = "", const std::string& steps = "",
        Application* app = nullptr, Tester* tester = nullptr,
        double cvss = 0.0, const std::string& type = "")
        : Bug(id, title, desc, prio, stat, date, steps, app, tester), cvssScore(cvss), vulnType(type) {
    }

    double getCvssScore() const { return cvssScore; }
    std::string getVulnType() const { return vulnType; }

    void setCvssScore(double score) { cvssScore = score; }
    void setVulnType(const std::string& type) { vulnType = type; }

    void printInfo() const override {
        std::cout << "[УЯЗВИМОСТЬ-" << id << "] " << name
            << " | CVSS: " << cvssScore
            << " | Тип: " << vulnType << "\n";
        if (application) std::cout << "   " << "Приложение: " << application->getAppName() << "\n";
        if (assignedTester) std::cout << "   " << "Тестировщик: " << assignedTester->getFullName() << "\n";
    }
};