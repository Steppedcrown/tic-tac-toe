// -----------------------------------------------------------------------------
// TicTacToe class implementation:
// This implementation uses a 3x3 BitHolder grid, places Bits for each player,
// checks win/draw conditions, and serializes the board to/from a compact
// 9-character state string. Related changes include ImGui .ini load/save hooks
// for the state string and re-checking game-over state after loading.
//
// AI Implementation:
// - Player 1 (O) is controlled by a basic random AI.
// - The AI finds all empty squares and randomly selects one to place its piece.
// - Uses actionForEmptyHolder() to maintain consistent piece placement logic.
//
// Note: Portions of this project were completed with generative AI assistance.
// -----------------------------------------------------------------------------

#include "TicTacToe.h"
#include <vector>
#include <cstdlib>

// -----------------------------------------------------------------------------
// TicTacToe.cpp
// -----------------------------------------------------------------------------
// This file is intentionally *full of comments* and gentle TODOs that guide you
// through wiring up a complete Tic‑Tac‑Toe implementation using the game engine’s
// Bit / BitHolder grid system.
//
// Rules recap:
//  - Two players place X / O on a 3x3 grid.
//  - Players take turns; you can only place into an empty square.
//  - First player to get three-in-a-row (row, column, or diagonal) wins.
//  - If all 9 squares are filled and nobody wins, it’s a draw.
//
// Notes about the provided engine types you'll use here:
//  - Bit              : a visual piece (sprite) that belongs to a Player
//  - BitHolder        : a square on the board that can hold at most one Bit
//  - Player           : the engine’s player object (you can ask who owns a Bit)
//  - Game options     : let the mouse know the grid is 3x3 (rowX, rowY)
//  - Helpers you’ll see used: setNumberOfPlayers, getPlayerAt, startGame, etc.
//
// I’ve already fully implemented PieceForPlayer() for you. Please leave that as‑is.
// The rest of the routines are written as “comment-first” TODOs for you to complete.
// -----------------------------------------------------------------------------

const int AI_PLAYER    = 1;      // index of the AI player (O)
const int HUMAN_PLAYER = 0;      // index of the human player (X)
const int MAX_DEPTH    = 9;      // maximum search depth for negamax

TicTacToe::TicTacToe()
{
}

TicTacToe::~TicTacToe()
{
}

// -----------------------------------------------------------------------------
// make an X or an O
// -----------------------------------------------------------------------------
// DO NOT CHANGE: This returns a new Bit with the right texture and owner
Bit* TicTacToe::PieceForPlayer(const int playerNumber)
{
    // depending on playerNumber load the "x.png" or the "o.png" graphic
    Bit *bit = new Bit();
    bit->LoadTextureFromFile(playerNumber == 1 ? "x.png" : "o.png");
    bit->setOwner(getPlayerAt(playerNumber));
    return bit;
}

//
// setup the game board, this is called once at the start of the game
//
void TicTacToe::setUpBoard()
{
    // here we should call setNumberOfPlayers to 2 and then set up the game options so the mouse knows to draw a 3x3 grid
    // _gameOptions has a rowX and rowY property we should set to 3
    // then we need to setup our 3x3 array in _grid with the correct position of the square, and load the "square.png" sprite for each square
    // we will use the initHolder function on each square to do this
    // finally we should call startGame to get everything going
    setNumberOfPlayers(2);
    setAIPlayer(AI_PLAYER);

    _gameOptions.rowX = 3;
    _gameOptions.rowY = 3;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            ImVec2 position = ImVec2(30 + j * 100, 35 + i * 100); // example positions
            _grid[i][j].initHolder(position, "square.png", j, i);
        }
    }

    startGame();
}

//
// about the only thing we need to actually fill out for tic-tac-toe
//
bool TicTacToe::actionForEmptyHolder(BitHolder *holder)
{
    // 1) Guard clause: if holder is nullptr, fail fast.
    //    (Beginner hint: always check pointers before using them.)
    if (!holder) return false;

    // 2) Is it actually empty?
    //    Ask the holder for its current Bit using the bit() function.
    //    If there is already a Bit in this holder, return false.
    if (holder->bit() != nullptr) return false;

    // 3) Place the current player's piece on this holder:
    //    - Figure out whose turn it is (getCurrentPlayer()->playerNumber()).
    //    - Create a Bit via PieceForPlayer(currentPlayerIndex).
    //    - Position it at the holder's position (holder->getPosition()).
    //    - Assign it to the holder: holder->setBit(newBit);
    unsigned int currentPlayerIndex = getCurrentPlayer()->playerNumber();
    Bit* newBit = PieceForPlayer(currentPlayerIndex);
    newBit->setPosition(holder->getPosition().x, holder->getPosition().y);
    holder->setBit(newBit);

    // 4) Return whether we actually placed a piece. true = acted, false = ignored.
    return true; // replace with true if you complete a successful placement    
}

bool TicTacToe::canBitMoveFrom(Bit *bit, BitHolder *src)
{
    // you can't move anything in tic tac toe
    return false;
}

bool TicTacToe::canBitMoveFromTo(Bit* bit, BitHolder*src, BitHolder*dst)
{
    // you can't move anything in tic tac toe
    return false;
}

//
// free all the memory used by the game on the heap
//
void TicTacToe::stopGame()
{
    // clear out the board
    // loop through the 3x3 array and call destroyBit on each square
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            _grid[i][j].destroyBit();
        }
    }
}

//
// helper function for the winner check
//
Player* TicTacToe::ownerAt(int index ) const
{
    // index is 0..8, convert to x,y using:
    // y = index / 3
    // x = index % 3 
    // if there is no bit at that location (in _grid) return nullptr
    // otherwise return the owner of the bit at that location using getOwner()
    int y = index / 3;
    int x = index % 3;
    Bit* bit = _grid[y][x].bit();
    if (bit) {
        return bit->getOwner();
    }
    return nullptr;
}

Player* TicTacToe::checkForWinner()
{
    // check all the winning triples
    // if any of them have the same owner return that player
    // winning triples are:
    // 0,1,2
    // 3,4,5
    // 6,7,8
    // 0,3,6
    // 1,4,7
    // 2,5,8
    // 0,4,8
    // 2,4,6
    // you can use the ownerAt helper function to get the owner of a square
    // for example, ownerAt(0) returns the owner of the top-left square
    // if there is no bit in that square, it returns nullptr
    // if you find a winning triple, return the player who owns that triple
    // otherwise return nullptr

    // Hint: Consider using an array to store the winning combinations
    // to avoid repetitive code
    int winningCombos[8][3] = {
        {0, 1, 2},
        {3, 4, 5},
        {6, 7, 8},
        {0, 3, 6},
        {1, 4, 7},
        {2, 5, 8},
        {0, 4, 8},
        {2, 4, 6}
    };
    for (int i = 0; i < 8; i++) {
        int a = winningCombos[i][0];
        int b = winningCombos[i][1];
        int c = winningCombos[i][2];
        Player* ownerA = ownerAt(a);
        Player* ownerB = ownerAt(b);
        Player* ownerC = ownerAt(c);
        if (ownerA && ownerA == ownerB && ownerB == ownerC) {
            return ownerA;
        }
    }
    return nullptr;
}

bool TicTacToe::checkForDraw()
{
    // is the board full with no winner?
    // if any square is empty, return false
    // otherwise return true
    for (int i = 0; i < 9; i++) {
        if (ownerAt(i) == nullptr) {
            return false;
        }
    }
    return true;
}

//
// state strings
//
std::string TicTacToe::initialStateString()
{
    return "000000000";
}

//
// this still needs to be tied into imguis init and shutdown
// we will read the state string and store it in each turn object
//
std::string TicTacToe::stateString() const
{
    // return a string representing the current state of the board
    // the string should be 9 characters long, one for each square
    // each character should be '0' for empty, '1' for player 1 (X), and '2' for player 2 (O)
    // the order should be left-to-right, top-to-bottom
    // for example, the starting state is "000000000"
    // if player 1 has placed an X in the top-left and player 2 an O in the center, the state would be "100020000"
    // you can build the string using a loop and the to_string function
    // for example, to convert an integer to a string, you can use std::to_string(1) which returns "1"
    // you can get the bit at each square using _grid[y][x].bit()
    // if the bit is not null, you can get its owner using bit->getOwner()->playerNumber()
    // remember that player numbers are zero-based, so add 1 to get '1' or '2'
    // if the bit is null, add '0' to the string
    // finally, return the constructed string
    std::string state;
    for (int i = 0; i < 9; i++) {
        int y = i / 3;
        int x = i % 3;
        Bit* bit = _grid[y][x].bit();
        if (bit) {
            state += std::to_string(bit->getOwner()->playerNumber() + 1);
        } else {
            state += "0";
        }
    }
    return state;
}

//
// this still needs to be tied into imguis init and shutdown
// when the program starts it will load the current game from the imgui ini file and set the game state to the last saved state
//
void TicTacToe::setStateString(const std::string &s)
{
    // set the state of the board from the given string
    // the string will be 9 characters long, one for each square
    // each character will be '0' for empty, '1' for player 1 (X), and '2' for player 2 (O)
    // the order will be left-to-right, top-to-bottom
    // for example, the starting state is "000000000"
    // if player 1 has placed an X in the top-left and player 2 an O in the center, the state would be "100020000"
    // you can loop through the string and set each square in _grid accordingly
    // for example, if s[0] is '1', you would set _grid[0][0] to have player 1's piece
    // if s[4] is '2', you would set _grid[1][1] to have player 2's piece
    // if s[8] is '0', you would set _grid[2][2] to be empty
    // you can use the PieceForPlayer function to create a new piece for a player
    // remember to convert the character to an integer by subtracting '0'
    // for example, int playerNumber = s[index] - '0';
    // if playerNumber is 0, set the square to empty (nullptr)
    // if playerNumber is 1 or 2, create a piece for that player and set it in the square
    // finally, make sure to position the piece at the holder's position
    // you can get the position of a holder using holder->getPosition()
    // loop through the 3x3 array and set each square accordingly
    // the string should always be valid, so you don't need to check its length or contents
    // but you can assume it will always be 9 characters long and only contain '0', '1', or '2'
    for (int i = 0; i < 9; i++) {
        int y = i / 3;
        int x = i % 3;
        char c = s[i];
        if (c == '0') {
            _grid[y][x].destroyBit();
        } else {
            int playerNumber = c - '0' - 1; // convert '1'/'2' to 0/1
            Bit* newBit = PieceForPlayer(playerNumber);
            newBit->setPosition(_grid[y][x].getPosition().x, _grid[y][x].getPosition().y);
            _grid[y][x].setBit(newBit);
        }
    }
}


//
// this is the function that will be called by the AI
//
void TicTacToe::updateAI()
{
    int bestScore = -1000;
    int moveX = -1;
    int moveY = -1;
    
    // Try each empty square and evaluate using negamax
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            if (_grid[y][x].bit() == nullptr) { // empty square
                // Make the AI move
                Bit* tempBit = PieceForPlayer(AI_PLAYER);
                _grid[y][x].setBit(tempBit);
                
                // Evaluate this move (next turn is opponent, so color = -1)
                int score = -negamax(_grid, MAX_DEPTH - 1, -1);
                
                // Undo the move
                _grid[y][x].destroyBit();

                if (score > bestScore) {
                    bestScore = score;
                    moveX = x;
                    moveY = y;
                }
            }
        }
    }
    
    // Make the best move found
    if (moveX != -1 && moveY != -1) {
        Bit* bestMoveBit = PieceForPlayer(AI_PLAYER);
        bestMoveBit->setPosition(_grid[moveY][moveX].getPosition().x, _grid[moveY][moveX].getPosition().y);
        _grid[moveY][moveX].setBit(bestMoveBit);
        std::cout << "AI places at (" << moveX << ", " << moveY << ") with score " << bestScore << "\n";
        endTurn();
    }
}

int TicTacToe::negamax(Square board[3][3], int depth, int color) 
{
    // Check for terminal states
    Player *winner = checkForWinner();
    if (winner != nullptr) {
        // If AI won, return positive; if human won, return negative
        if (winner == getPlayerAt(AI_PLAYER)) {
            return 1;
        } else {
            return -1;
        }
    }

    if (checkForDraw() || depth == 0) {
        return 0; // Draw or max depth reached
    }
    
    // Determine which player is making this move
    // color = 1 means AI's turn, anything else means human's turn
    int currentPlayer = (color == 1) ? AI_PLAYER : HUMAN_PLAYER;
    int bestValue = -1000;
    
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            if (board[y][x].bit() == nullptr) { // empty square
                // Make the move
                Bit* tempBit = PieceForPlayer(currentPlayer);
                board[y][x].setBit(tempBit);
                
                // Recursively evaluate (negamax principle: negate opponent's score)
                int score = -negamax(board, depth - 1, -color);
                
                // Undo the move
                board[y][x].destroyBit();
                
                if (score > bestValue) {
                    bestValue = score;
                }
            }
        }
    }
    
    // If no moves were possible (shouldn't happen due to terminal checks)
    if (bestValue == -1000) {
        return 0;
    }
    return bestValue;
}

