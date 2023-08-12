#pragma once
#include "ClickMode.h"
#include <Windows.h>

class AutoClicker {
public:
    void Start(int minCps, int maxCps, int activationKey, bool suppressActivationKey, bool simulateLeftButton, ClickMode mode);
    void Stop();
    bool isToggleActive();
    LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam); // Nicht mehr statisch
    LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);

private:
    bool running = false;
    bool toggleState = false;
    bool lastToggleState = false; // Hinzufügen
    bool isActivationKeyPressed = false; // Hinzufügen
    bool activationKeyJustPressed = false;
    bool suppressActivationKey = false;
    bool activationKeyPressed = false;
    ClickMode mode;
    void ClickLoop(int minCps, int maxCps, int activationKey, bool simulateLeftButton, ClickMode mode);
    HHOOK keyboardHook;

    static int activationKeyStatic; // Statische Variable, um den Aktivierungsschlüssel zu speichern
};