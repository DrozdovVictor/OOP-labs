#include <iostream>
#include"functions.h"
#include"inputs.h"
#include <windows.h>

using namespace std;

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int choice;
    do {
        showMenu();
        choice = inputInt("Ваш выбор: ");
        switch (choice) {
        case 1:  viewAll(); break;
        case 2:  addTeam(); break;
        case 3:  addAthlete(); break;
        case 4:  addCoachToTeam(); break;
        case 5:  removeTeam(); break;
        case 6:  removeAthlete(); break;
        case 7:  removeCoachFromTeam(); break;
        case 8:  editTeam(); break;
        case 9:  editAthlete(); break;
        case 10: editCoach(); break;
        case 11: sortAthletes(); break;
        case 12: searchAthlete(); break;
        case 13: fileAddAthlete(); break;
        case 14: fileViewAthletes(); break;
        case 15: fileEditAthlete(); break;
        case 16: fileDeleteAthlete(); break;
        case 17: fileSearchAthlete(); break;
        case 0:  cout << "Выход.\n"; break;
        default: cout << "Неверно.\n";
        }
    } while (choice != 0);
    cleanup();
    return 0;
}