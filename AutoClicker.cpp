#include "AutoClicker.h"
#include <Windows.h>
#include <random>
#include <thread>

void AutoClicker::Start(int minCps, int maxCps, int button) {
    running = true;
    std::thread clickThread(&AutoClicker::ClickLoop, this, minCps, maxCps, button);
    clickThread.detach();
}

void AutoClicker::Stop() {
    running = false;
}

bool running = false;

void AutoClicker::ClickLoop(int minCps, int maxCps, int button) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(minCps, maxCps);

    while (running) {
        int cps = distrib(gen);
        int delay = 1000 / cps;
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        Sleep(delay);
    }
}