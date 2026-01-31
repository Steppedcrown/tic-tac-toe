#pragma once

namespace ClassGame {
    void GameStartUp();
    void RenderGame();
    void EndOfTurn();
    const char* GetStateString();
    void SetStateString(const char* state);
}
