#pragma once
#include <string>
#include <iostream>

class BaseEntity {
protected:
    int id;
    std::string name;

public:
    BaseEntity(int id = 0, const std::string& name = "")
        : id(id), name(name) {
    }
    virtual ~BaseEntity() = default;

    int getId() const { return id; }
    std::string getName() const { return name; }
    void setName(const std::string& n) { name = n; }

    // параметр os нужен для вывода в файл
    virtual void printInfo(std::ostream& os = std::cout) const {
        os << "ID: " << id << ", Название: " << name;
    }
};

class Application {
    int id;
    std::string appName;
    std::string version;
    std::string platform;

public:
    Application(int id = 0,
        const std::string& name = "",
        const std::string& ver = "",
        const std::string& plat = "")
        : id(id), appName(name), version(ver), platform(plat) {
    }

    int getId() const { return id; }
    std::string getAppName() const { return appName; }
    std::string getVersion() const { return version; }

    void printInfo(std::ostream& os = std::cout) const {
        os << "Приложение: " << appName
            << " v" << version << " (" << platform << ")";
    }
};

class Tester {
    int id;
    std::string fullName;
    std::string specialization;

public:
    Tester(int id = 0, const std::string& n = "", const std::string& s = "")
        : id(id), fullName(n), specialization(s) {
    }

    int getId() const { return id; }
    std::string getFullName() const { return fullName; }
    std::string getSpecialization() const { return specialization; }

    // пригодится, если Tester будет ключом в set/map
    bool operator<(const Tester& o) const { return id < o.id; }
    bool operator==(const Tester& o) const { return id == o.id; }

    void printInfo(std::ostream& os = std::cout) const {
        os << fullName << " (" << specialization << ")";
    }
};

class Bug : public BaseEntity {
protected:
    std::string description;
    std::string priority;           // Low / Medium / High / Critical
    std::string status;             // Open / In Progress / Resolved / Closed
    std::string dateFound;
    std::string reproductionSteps;

    Application* application;       // композиция (не владеет)
    Tester* assignedTester;    // агрегация (не владеет)

public:
    Bug(int id = 0,
        const std::string& title = "",
        const std::string& desc = "",
        const std::string& prio = "Medium",
        const std::string& stat = "Open",
        const std::string& date = "",
        const std::string& steps = "",
        Application* app = nullptr,
        Tester* tester = nullptr)
        : BaseEntity(id, title),
        description(desc), priority(prio), status(stat),
        dateFound(date), reproductionSteps(steps),
        application(app), assignedTester(tester) {
    }

    std::string getPriority() const { return priority; }
    std::string getStatus() const { return status; }
    std::string getDescription() const { return description; }
    Application* getApplication() const { return application; }
    Tester* getAssignedTester() const { return assignedTester; }

    void setPriority(const std::string& p) { priority = p; }
    void setStatus(const std::string& s) { status = s; }

    bool operator<(const Bug& o) const { return id < o.id; }
    bool operator==(const Bug& o) const { return id == o.id; }

    void printInfo(std::ostream& os = std::cout) const override {
        os << "[" << id << "] " << name
            << " | Приоритет: " << priority
            << " | Статус: " << status << "\n";
        if (application) os << "   Приложение: " << application->getAppName() << "\n";
        if (assignedTester) os << "   Тестировщик: " << assignedTester->getFullName() << "\n";
    }
};

class Vulnerability : public Bug {
    double cvssScore;
    std::string vulnType;

public:
    Vulnerability(int id = 0,
        const std::string& title = "",
        const std::string& desc = "",
        const std::string& prio = "Critical",
        const std::string& stat = "Open",
        const std::string& date = "",
        const std::string& steps = "",
        Application* app = nullptr,
        Tester* tester = nullptr,
        double cvss = 0.0,
        const std::string& type = "")
        : Bug(id, title, desc, prio, stat, date, steps, app, tester),
        cvssScore(cvss), vulnType(type) {
    }

    double getCvssScore() const { return cvssScore; }
    std::string getVulnType() const { return vulnType; }

    void printInfo(std::ostream& os = std::cout) const override {
        os << "[УЯЗВИМОСТЬ-" << id << "] " << name
            << " | CVSS: " << cvssScore
            << " | Тип: " << vulnType << "\n";
        if (application) os << "   Приложение: " << application->getAppName() << "\n";
        if (assignedTester) os << "   Тестировщик: " << assignedTester->getFullName() << "\n";
    }
};

// ============================================================
//  Хеш и равенство для unordered_set / unordered_multiset.
//  Без <functional>: для int-ключа хватает static_cast.
// ============================================================
struct BugHash {
    std::size_t operator()(const Bug& b) const {
        return static_cast<std::size_t>(b.getId());
    }
};

struct BugEqual {
    bool operator()(const Bug& a, const Bug& b) const {
        return a.getId() == b.getId();
    }
};

// ============================================================
//  Компаратор для std::set<Bug*> / std::multiset<Bug*>
//  (по умолчанию std::less<Bug*> сравнивает адреса, а не id)
// ============================================================
struct BugPtrLess {
    bool operator()(Bug* a, Bug* b) const {
        return a->getId() < b->getId();
    }
};

// ============================================================
//  Хеш и равенство для std::unordered_set<Bug*> /
//  std::unordered_multiset<Bug*>.  <functional> не нужен,
//  т.к. id — int, и static_cast<std::size_t> достаточно.
// ============================================================
struct BugPtrHash {
    std::size_t operator()(Bug* p) const {
        return static_cast<std::size_t>(p->getId());
    }
};

struct BugPtrEqual {
    bool operator()(Bug* a, Bug* b) const {
        return a->getId() == b->getId();
    }
};