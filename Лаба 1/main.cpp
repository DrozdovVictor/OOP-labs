#include <iostream>
#include <windows.h>
#include <string>
#include "classes.h"
#include "inputs.h"
#include"functions.h"

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int choice;
    do {
        showMainMenu();
        choice = inputInt("Ваш выбор: ");

        switch (choice) {
        case 1: viewAll(); break;
        case 2: addTeam(); break;
        case 3: addAthlete(); break;
        case 4: addCoachToTeam(); break;
        case 5: removeTeam(); break;
        case 6: removeAthlete(); break;
        case 7: removeCoachFromTeam(); break;
        case 8: editTeam(); break;
        case 9: editAthlete(); break;
        case 10: editCoach(); break;
        case 11: sortAthletes(); break;
        case 12: searchAthlete(); break;
        case 13: saveToFile(); break;
        case 14: loadFromFile(); break;
        case 0: std::cout << "Выход.\n"; break;
        default: std::cout << "Неверный пункт меню.\n";
        }

    } while (choice != 0);

    cleanup();
    return 0;
}
