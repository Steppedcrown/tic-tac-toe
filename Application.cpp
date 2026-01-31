#include "Application.h"
#include "imgui/imgui.h"
#include "classes/TicTacToe.h"
#include <string>

namespace ClassGame {
        //
        // our global variables
        //
        TicTacToe *game = nullptr;
        bool gameOver = false;
        int gameWinner = -1;
        // Cached state string for ImGui save callback lifetime.
        static std::string g_cachedState;

        //
        // game starting point
        // this is called by the main render loop in main.cpp
        //
        void GameStartUp() 
        {
            game = new TicTacToe();
            game->setUpBoard();
            // Ensure game-over UI is correct for a freshly initialized/loaded state.
            gameOver = false;
            gameWinner = -1;
            EndOfTurn();
        }

        //
        // game render loop
        // this is called by the main render loop in main.cpp
        //
        void RenderGame() 
        {
                ImGui::DockSpaceOverViewport();

                //ImGui::ShowDemoWindow();

                if (!game) return;
                if (!game->getCurrentPlayer()) return;
                
                ImGui::Begin("Settings");
                ImGui::Text("Current Player Number: %d", game->getCurrentPlayer()->playerNumber());
                ImGui::Text("Current Board State: %s", game->stateString().c_str());

                if (gameOver) {
                    ImGui::Text("Game Over!");
                    if (gameWinner == -1) {
                        ImGui::Text("It's a Draw!");
                    } else {
                        ImGui::Text("Player %d wins!", gameWinner);
                    }
                    if (ImGui::Button("Reset Game")) {
                        game->stopGame();
                        game->setUpBoard();
                        gameOver = false;
                        gameWinner = -1;
                    }
                }
                ImGui::End();

                ImGui::Begin("GameWindow");
                game->drawFrame();
                ImGui::End();
        }

        //
        // end turn is called by the game code at the end of each turn
        // this is where we check for a winner
        //
        void EndOfTurn() 
        {
            Player *winner = game->checkForWinner();
            if (winner)
            {
                gameOver = true;
                gameWinner = winner->playerNumber();
            }
            if (game->checkForDraw()) {
                gameOver = true;
                gameWinner = -1;
            }
        }

        const char* GetStateString()
        {
            // Used by ImGui .ini save handler.
            if (!game)
            {
                g_cachedState.clear();
                return g_cachedState.c_str();
            }
            g_cachedState = game->stateString();
            return g_cachedState.c_str();
        }

        void SetStateString(const char* state)
        {
            // Used by ImGui .ini load handler.
            if (!game || state == nullptr)
                return;
            game->setStateString(std::string(state));
            // Re-evaluate game-over state after loading.
            gameOver = false;
            gameWinner = -1;
            EndOfTurn();
        }
}
