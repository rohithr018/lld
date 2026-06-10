#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cstdlib>
#include <ctime>

using namespace std;

class Player {
private:
    string name;
    int position;

public:
    Player(string name)
        : name(name), position(0) {}

    string getName() const {
        return name;
    }

    int getPosition() const {
        return position;
    }

    void setPosition(int pos) {
        position = pos;
    }
};

class Snake {
private:
    int head;
    int tail;

public:
    Snake(int head, int tail)
        : head(head), tail(tail) {}

    int getHead() const {
        return head;
    }

    int getTail() const {
        return tail;
    }
};

class Ladder {
private:
    int start;
    int end;

public:
    Ladder(int start, int end)
        : start(start), end(end) {}

    int getStart() const {
        return start;
    }

    int getEnd() const {
        return end;
    }
};

class Dice {
public:
    int roll() {
        return rand() % 6 + 1;
    }
};

class Board {

private:
    unordered_map<int,int> snakes;
    unordered_map<int,int> ladders;

public:

    void addSnake(int head, int tail) {
        snakes[head] = tail;
    }

    void addLadder(int start, int end) {
        ladders[start] = end;
    }

    int getFinalPosition(int pos) {

        if(snakes.count(pos)) {

            cout << "Bitten by snake: "
                 << pos << " -> "
                 << snakes[pos]
                 << endl;

            return snakes[pos];
        }

        if(ladders.count(pos)) {

            cout << "Climbed ladder: "
                 << pos << " -> "
                 << ladders[pos]
                 << endl;

            return ladders[pos];
        }

        return pos;
    }
};

class Game {

private:

    Board board;
    Dice dice;

    queue<Player*> players;

    static const int WINNING_CELL = 100;

public:

    Board& getBoard() {
        return board;
    }

    void addPlayer(Player* player) {
        players.push(player);
    }

    void play() {

        while(true) {

            Player* currentPlayer =
                players.front();

            players.pop();

            int roll = dice.roll();

            cout << "\n"
                 << currentPlayer->getName()
                 << " rolled "
                 << roll
                 << endl;

            int currentPos =
                currentPlayer->getPosition();

            int newPos =
                currentPos + roll;

            if(newPos > WINNING_CELL) {

                cout << currentPlayer->getName()
                     << " needs exact roll"
                     << endl;

                players.push(currentPlayer);
                continue;
            }

            newPos =
                board.getFinalPosition(newPos);

            currentPlayer->setPosition(newPos);

            cout << currentPlayer->getName()
                 << " moved to "
                 << newPos
                 << endl;

            if(newPos == WINNING_CELL) {

                cout << "\nWinner is "
                     << currentPlayer->getName()
                     << " !!!"
                     << endl;

                break;
            }

            players.push(currentPlayer);
        }
    }
};

int main() {

    srand(time(nullptr));

    Game game;

    // Snakes
    game.getBoard().addSnake(99, 54);
    game.getBoard().addSnake(70, 55);
    game.getBoard().addSnake(52, 42);
    game.getBoard().addSnake(25, 2);

    // Ladders
    game.getBoard().addLadder(6, 25);
    game.getBoard().addLadder(11, 40);
    game.getBoard().addLadder(60, 85);
    game.getBoard().addLadder(46, 90);

    Player p1("Alice");
    Player p2("Bob");

    game.addPlayer(&p1);
    game.addPlayer(&p2);

    game.play();

    return 0;
}