#pragma once

class AutoClicker {
public:
    void Start(int minCps, int maxCps, int button);
    void Stop();

private:
    bool running = false;
    void ClickLoop(int minCps, int maxCps, int button);
};