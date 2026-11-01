#pragma once
#include <string>
#include <array>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include "classes.h"

extern std::array<Tester, 5> testersArray;
extern std::vector<Application*> appsVector;

extern std::map<int, Bug*> mapById;
extern std::set<Bug*, BugPtrLess> setById;
extern std::multimap<std::string, Bug*> multimapByTester;
extern std::multiset<Bug*, BugPtrLess> multisetById;
extern std::unordered_map<int, Bug*> umapById;
extern std::unordered_set<Bug*, BugPtrHash, BugPtrEqual> usetById;
extern std::unordered_multimap<std::string, Bug*> umultimapByTester;
extern std::unordered_multiset<Bug*, BugPtrHash, BugPtrEqual> umultisetById;

// helpers
int inputInt(const std::string& prompt);
std::string inputStr(const std::string& prompt);
Tester* findTesterById(int id);
Application* findAppById(int id);
void printSeparator();
Bug* inputNewBug();
void initDemoData(); 

// menus
void menuMap();
void menuSet();
void menuMultimap();
void menuMultiset();
void menuUnorderedMap();
void menuUnorderedSet();
void menuUnorderedMultimap();
void menuUnorderedMultiset();

void printApplications();
void printTesters();     