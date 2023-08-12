#include "AutoClicker.h"
#include <Windows.h>
#include <random>
#include <thread>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>

#define KEY_PRESSED_BIT 0x0001 // Definiere das Bit

// Globale Variable, um auf das AutoClicker-Objekt zuzugreifen
AutoClicker* globalAutoClicker = nullptr;

int AutoClicker::activationKeyStatic = 0;

// Globale Funktion, die die Methode im AutoClicker-Objekt aufruft
LRESULT CALLBACK GlobalLowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (globalAutoClicker) {
        return globalAutoClicker->LowLevelKeyboardProc(nCode, wParam, lParam);
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}


HHOOK mouseHook = NULL;

LRESULT CALLBACK GlobalMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (globalAutoClicker) {
        return globalAutoClicker->MouseProc(nCode, wParam, lParam);
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}



void AutoClicker::Start(int minCps, int maxCps, int activationKey, bool suppressActivationKey, bool simulateLeftButton, ClickMode mode) {
    running = true;
    this->suppressActivationKey = suppressActivationKey; // Speichere den Parameter
    this->mode = mode;
    std::thread clickThread(&AutoClicker::ClickLoop, this, minCps, maxCps, activationKey, simulateLeftButton, mode);
    clickThread.detach();

    globalAutoClicker = this; // Setze die globale Variable
    activationKeyStatic = activationKey;
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, GlobalLowLevelKeyboardProc, NULL, 0);
    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, GlobalMouseProc, NULL, 0);
}

void AutoClicker::Stop() {
    UnhookWindowsHookEx(keyboardHook);
    UnhookWindowsHookEx(mouseHook);
    running = false;
}

void AutoClicker::ClickLoop(int minCps, int maxCps, int activationKey, bool simulateLeftButton, ClickMode mode) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(minCps, maxCps);
    toggleState = false;

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
            if (toggleState) {
                mouse_event(mouseDownEvent, 0, 0, 0, 0);
                mouse_event(mouseUpEvent, 0, 0, 0, 0);
                Sleep(delay);
            }
        }
    }
}


// Methode hinzugefügt, um den Toggle-Zustand abzufragen
bool AutoClicker::isToggleActive() {
    return toggleState;
}

LRESULT CALLBACK AutoClicker::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    struct tm timeInfo;
    localtime_s(&timeInfo, &time);

    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* pKeyStruct = (KBDLLHOOKSTRUCT*)lParam;
        wchar_t keyName[50];
        GetKeyNameTextW(pKeyStruct->scanCode << 16, keyName, sizeof(keyName) / sizeof(wchar_t));

        if (wParam == WM_KEYDOWN && pKeyStruct->vkCode == activationKeyStatic) {
            activationKeyPressed = true; // Taste wurde gedrückt, aber nicht losgelassen
            if (suppressActivationKey) {
                return 1; // Unterdrücke das Tastendruckereignis
            }
        }

        if (wParam == WM_KEYUP && pKeyStruct->vkCode == activationKeyStatic && activationKeyPressed) {
            wprintf(L"Activation key released. Previous toggle state: %d\n", toggleState);
            toggleState = !toggleState;
            wprintf(L"New toggle state: %d\n", toggleState);
            activationKeyPressed = false;
            if (suppressActivationKey) {
                return 1;
            }
        }

        if (wParam == WM_KEYDOWN) {
            wprintf(L"%02d:%02d:%02d.%03d - Generic WM_KEYDOWN detected. Key: %s\n",
                timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec, ms.count(), keyName);
        }
        else if (wParam == WM_KEYUP) {
            wprintf(L"%02d:%02d:%02d.%03d - Generic WM_KEYUP detected. Key: %s\n",
                timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec, ms.count(), keyName);
        }

        // Rest des Codes ...
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}


LRESULT CALLBACK AutoClicker::MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    struct tm timeInfo;
    localtime_s(&timeInfo, &time);

    if (nCode == HC_ACTION) {
        switch (wParam) {
        case WM_LBUTTONDOWN:
            wprintf(L"%02d:%02d:%02d.%03d - Left mouse button DOWN detected.\n",
                timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec, ms.count());
            break;
        case WM_LBUTTONUP:
            wprintf(L"%02d:%02d:%02d.%03d - Left mouse button UP detected.\n",
                timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec, ms.count());
            break;
        case WM_RBUTTONDOWN:
            wprintf(L"%02d:%02d:%02d.%03d - Right mouse button DOWN detected.\n",
                timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec, ms.count());
            break;
        case WM_RBUTTONUP:
            wprintf(L"%02d:%02d:%02d.%03d - Right mouse button UP detected.\n",
                timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec, ms.count());
            break;
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}










