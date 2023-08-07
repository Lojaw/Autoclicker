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
    // Implementierung
}

void Menu::RightClickMenu() {
    // Implementierung
}

void Menu::ChooseCPS(int& minCps, int& maxCps) {
    std::cout << "Enter the range of clicks per second (e.g. 7 10): ";
    std::cin >> minCps >> maxCps;
}

void Menu::SelectMode() {
    // Implementierung
}

void Menu::SelectKey() {
    // Implementierung
}