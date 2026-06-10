#include <iostream>
#include <vector>
#include <stdexcept>

using namespace std;

class Player {
private:
    string name;
    char symbol;

public:
    Player(string name, char symbol)
        : name(name), symbol(symbol) {}

    string getName() const {
        return name;
    }

    char getSymbol() const {
        return symbol;
    }
};

class Board {
private:
    int n;

    vector<vector<char>> grid;

    vector<int> rows;
    vector<int> cols;

    int diag;
    int antiDiag;

public:

    Board(int n)
        : n(n),
          grid(n, vector<char>(n, '-')),
          rows(n, 0),
          cols(n, 0),
          diag(0),
          antiDiag(0) {}

    bool isCellEmpty(int row,
                     int col) const {

        return grid[row][col] == '-';
    }

    bool makeMove(
        int row,
        int col,
        char symbol)
    {
        if (!isCellEmpty(row, col))
            return false;

        grid[row][col] = symbol;

        int value =
            (symbol == 'X') ? 1 : -1;

        rows[row] += value;
        cols[col] += value;

        if (row == col)
            diag += value;

        if (row + col == n - 1)
            antiDiag += value;

        if (abs(rows[row]) == n ||
            abs(cols[col]) == n ||
            abs(diag) == n ||
            abs(antiDiag) == n)
        {
            return true;
        }

        return false;
    }

    bool isFull() const {

        for (int i = 0; i < n; i++) {

            for (int j = 0; j < n; j++) {

                if (grid[i][j] == '-')
                    return false;
            }
        }

        return true;
    }

    void display() const {

        cout << "\n";

        for (int i = 0; i < n; i++) {

            for (int j = 0; j < n; j++) {

                cout << grid[i][j] << " ";
            }

            cout << "\n";
        }

        cout << "\n";
    }
};

class TicTacToeGame {

private:

    Board board;

    Player player1;
    Player player2;

    Player* currentPlayer;

public:

    TicTacToeGame()
        : board(3),
          player1("Alice", 'X'),
          player2("Bob", 'O')
    {
        currentPlayer = &player1;
    }

    void switchTurn() {

        if (currentPlayer == &player1)
            currentPlayer = &player2;
        else
            currentPlayer = &player1;
    }

    void play() {

        while (true) {

            board.display();

            int row, col;

            cout << currentPlayer->getName()
                 << " (" << currentPlayer->getSymbol()
                 << ") Enter row and col: ";

            cin >> row >> col;

            if (row < 0 ||
                row >= 3 ||
                col < 0 ||
                col >= 3)
            {
                cout << "Invalid cell\n";
                continue;
            }

            bool win =
                board.makeMove(
                    row,
                    col,
                    currentPlayer->getSymbol());

            if (!win &&
                !board.isCellEmpty(row, col))
            {
                // move already applied
            }

            if (win) {

                board.display();

                cout
                    << currentPlayer->getName()
                    << " wins!\n";

                break;
            }

            if (board.isFull()) {

                board.display();

                cout << "Draw Game\n";

                break;
            }

            switchTurn();
        }
    }
};

int main() {

    TicTacToeGame game;

    game.play();

    return 0;
}