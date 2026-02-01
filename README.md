# Tic-Tac-Toe Project Notes

## Design Process
- Implemented Tic-Tac-Toe using the engine’s `Bit`/`BitHolder` grid and `Player` ownership model.
- Added a compact 9-character board state string for serialization.
- Integrated ImGui settings handlers to persist game state via `imgui.ini`.
- Added a post-load game-over recheck so UI reflects completed games after loading.

## AI Implementation
- Implemented negamax algorithm with alpha-beta pruning for optimal play.
- Player 1 (O) is controlled by the AI, while Player 0 (X) is controlled by the human.
- Evaluation scores wins as `+10 + depth` and losses as `-10 - depth` to prefer faster wins and delay losses.
- The `color` parameter alternates between `1` (AI turn) and `-1` (opponent turn).
- Maximum search depth is 9 (all possible moves are explored for perfect play).

## Platform
- Development and testing were done on macOS.

## Relevant Changes
- Custom ImGui settings handler for save/load of the board state.
- Application-level state accessors for ImGui save/load callbacks.
- Game-over re-evaluation after initialization and state load.
- Added documentation comments in key modified files. (TicTacToe.cpp, Application.cpp, Application.h, imgui.cpp)

## Notes
- Portions of this project were completed with generative AI assistance.
