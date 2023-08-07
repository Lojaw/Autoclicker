#include "AutoClicker.h"
#include <Windows.h>
#include <random>
#include <thread>

void AutoClicker::Start(int minCps, int maxCps, int activationKey, bool simulateLeftButton, ClickMode mode) {
    running = true;
    std::thread clickThread(&AutoClicker::ClickLoop, this, minCps, maxCps, activationKey, simulateLeftButton, mode);
    clickThread.detach();
}

void AutoClicker::Stop() {
    running = false;
}

void AutoClicker::ClickLoop(int minCps, int maxCps, int activationKey, bool simulateLeftButton, ClickMode mode) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(minCps, maxCps);
    bool toggleState = false;

    while (running) {
        int cps = distrib(gen);
        int delay = 1000 / cps;

        // Entscheide, welche Maustaste simuliert werden soll
        DWORD mouseDownEvent = simulateLeftButton ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN;
        DWORD mouseUpEvent = simulateLeftButton ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_RIGHTUP;

        if (mode == ClickMode::Hold && GetAsyncKeyState(activationKey)) {
            mouse_event(mouseDownEvent, 0, 0, 0, 0);
            mouse_event(mouseUpEvent, 0, 0, 0, 0);
            Sleep(delay);
        }
        else if (mode == ClickMode::Toggle) {
            if (GetAsyncKeyState(activationKey) & 1) {
                toggleState = !toggleState;
            }
            if (toggleState) {
                mouse_event(mouseDownEvent, 0, 0, 0, 0);
                mouse_event(mouseUpEvent, 0, 0, 0, 0);
                Sleep(delay);
            }
        }
    }
}