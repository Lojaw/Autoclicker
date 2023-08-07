#include <Windows.h>
#include "Menu.h"
#include "AutoClicker.h"
#include <thread>
#include <chrono>

int main() {
    // Beispiel: Starte den Autoclicker mit einem Bereich von 5 bis 10 CPS
    // für die linke Maustaste (0) im Hold-Modus
    AutoClicker autoClicker;
    Menu menu;

    // Zeige das Hauptmenü an
    //menu.ShowMainMenu();

    // Warte 5 Sekunden (optional, für Testzwecke)
    std::this_thread::sleep_for(std::chrono::seconds(3));

    // Starte den Autoclicker mit der "r"-Taste, um die linke Maustaste im Hold-Modus zu simulieren
    //autoClicker.Start(5, 10, 'R', true, ClickMode::Hold);
    autoClicker.Start(5, 10, VK_SPACE, true, ClickMode::Toggle);

    // Starte einen separaten Thread, der den Autoclicker nach 10 Sekunden stoppt
    std::thread timerThread([&autoClicker]() {
        std::this_thread::sleep_for(std::chrono::seconds(20));
        autoClicker.Stop();
    });

    // Warte auf den Timer-Thread, um sicherzustellen, dass er abgeschlossen ist, bevor das Programm beendet wird
    timerThread.join();

    //TEST TEST TEST

    return 0;
}
