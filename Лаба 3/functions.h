#pragma once
#include<vector>
#include<list>
#include<array>
#include <string>

// global containers
extern std::array<Tester, 5> testersArray; // array: 5 testers
extern std::vector<Application*> appsVector; // vector: apps
extern std::vector<Bug*> bugsVector; // vector: bugs
extern std::list<Vulnerability> vulnerabilitiesList; // list: Vulnerabilities

// helpers
int inputInt(const std::string& prompt);
std::string inputStr(const std::string& prompt);
Tester* findTesterById(int id);
Application* findAppById(int id);
void printSeparator();

//inition
void initDemoData();

// vector<Bug*>
void printBugs(bool toFile = false);
void addBug();
void removeBug();
void editBug();
void searchBug();
void sortBugs();

// list<Vulnerability>
void printVulns(bool toFile = false);
void addVulnerability();
void removeVulnerability();
void editVulnerability();
void searchVulnerability();
void sortVulnerabilities();

// array<Tester, 5>
void printTesters(bool toFile = false);
void addTester();
void removeTester();
void editTester();
void searchTester();
void sortTesters();

// menu functions
void menuVector();
void menuList();
void menuArray();

