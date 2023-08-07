#include "Menu.h"
#include <iostream>

void Menu::ShowMainMenu() {
    int choice;
    do {
        std::cout << "1. Simulate left mouse button\n";
        std::cout << "2. Simulate right mouse button\n";
        std::cout << "3. Exit\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            LeftClickMenu();
            break;
        case 2:
            RightClickMenu();
            break;
        }
    } while (choice != 3);
}

void Menu::LeftClickMenu() {
    isLeftClickSelected = true;
    ShowSubmenu();
}

void Menu::RightClickMenu() {
    isLeftClickSelected = false;
    ShowSubmenu();
}

void Menu::ShowSubmenu() {
    std::cout << (isLeftClickSelected ? "Simulate left mouse button\n" : "Simulate right mouse button\n");
    int choice;
    do {
        std::cout << "1. Choose CPS\n";
        std::cout << "2. Select mode\n";
        std::cout << "3. Select key\n";
        std::cout << "4. Back to main menu\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            int minCps, maxCps;
            ChooseCPS(minCps, maxCps);
            break;
        case 2:
            SelectMode();
            break;
        case 3:
            SelectKey();
            break;
        }
    } while (choice != 4);
}

void Menu::ChooseCPS(int& minCps, int& maxCps) {
    std::cout << "Enter the maximal clicks per second (e.g. 10): ";
    std::cin >> maxCps;
    std::cout << "Enter the minimal clicks per second (e.g. 7): ";
    std::cin >> minCps;
}

void Menu::SelectMode() {
    int choice;
    do {
        std::cout << "1. Hold Mode\n";
        std::cout << "2. Toggle Mode\n";
        std::cout << "3. Back\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            // Setze den Modus auf Hold
            break;
        case 2:
            // Setze den Modus auf Toggle
            break;
        }
    } while (choice != 3);
}

void Menu::SelectKey() {
    
}