#pragma once

class Menu {
public:
    void ShowMainMenu();
    void LeftClickMenu();
    void RightClickMenu();
    void ChooseCPS(int& minCps, int& maxCps);
    void SelectMode();
    void SelectKey();

private:
    bool isLeftClickSelected;
    void ShowSubmenu();
};
