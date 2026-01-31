#pragma once

namespace ClassGame {
    void GameStartUp();
    void RenderGame();
    void EndOfTurn();
    // Expose state string for ImGui .ini save/load.
    const char* GetStateString();
    // Apply state string loaded from ImGui .ini.
    void SetStateString(const char* state);
}
