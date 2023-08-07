#pragma once
#include "ClickMode.h"

class AutoClicker {
public:
    void Start(int minCps, int maxCps, int activationKey, bool simulateLeftButton, ClickMode mode);
    void Stop();

private:
    bool running = false;
    void ClickLoop(int minCps, int maxCps, int activationKey, bool simulateLeftButton, ClickMode mode);
};