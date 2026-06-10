#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <cmath>

using namespace std;

enum class Direction {
    UP,
    DOWN,
    IDLE
};

enum class ElevatorState {
    MOVING,
    IDLE
};

class Elevator {
private:
    int id;
    int currentFloor;

    Direction direction;
    ElevatorState state;

    // Floors above current floor
    priority_queue<int, vector<int>, greater<int>> upRequests;

    // Floors below current floor
    priority_queue<int> downRequests;

public:
    Elevator(int id)
        : id(id),
          currentFloor(0),
          direction(Direction::IDLE),
          state(ElevatorState::IDLE) {}

    int getId() const {
        return id;
    }

    int getCurrentFloor() const {
        return currentFloor;
    }

    Direction getDirection() const {
        return direction;
    }

    bool isIdle() const {
        return state == ElevatorState::IDLE;
    }

    int distanceFrom(int floor) const {
        return abs(currentFloor - floor);
    }

    // Internal request
    void addInternalRequest(int floor) {

        if (floor > currentFloor) {
            upRequests.push(floor);
        }
        else if (floor < currentFloor) {
            downRequests.push(floor);
        }
    }

    // External request
    void addExternalRequest(int floor) {

        if (floor > currentFloor) {
            upRequests.push(floor);
        }
        else if (floor < currentFloor) {
            downRequests.push(floor);
        }
        else {
            cout << "Elevator "
                 << id
                 << " already at floor "
                 << floor
                 << "\n";
        }
    }

    bool hasPendingRequests() const {
        return !upRequests.empty() ||
               !downRequests.empty();
    }

    void processNextRequest() {

        if (!upRequests.empty()) {

            state = ElevatorState::MOVING;
            direction = Direction::UP;

            int nextFloor = upRequests.top();
            upRequests.pop();

            cout << "Elevator "
                 << id
                 << " moving UP from "
                 << currentFloor
                 << " -> "
                 << nextFloor
                 << "\n";

            currentFloor = nextFloor;
        }

        else if (!downRequests.empty()) {

            state = ElevatorState::MOVING;
            direction = Direction::DOWN;

            int nextFloor = downRequests.top();
            downRequests.pop();

            cout << "Elevator "
                 << id
                 << " moving DOWN from "
                 << currentFloor
                 << " -> "
                 << nextFloor
                 << "\n";

            currentFloor = nextFloor;
        }

        if (!hasPendingRequests()) {
            direction = Direction::IDLE;
            state = ElevatorState::IDLE;
        }
    }

    void displayStatus() const {

        cout << "Elevator "
             << id
             << " | Floor: "
             << currentFloor
             << " | Direction: ";

        switch (direction) {
            case Direction::UP:
                cout << "UP";
                break;
            case Direction::DOWN:
                cout << "DOWN";
                break;
            case Direction::IDLE:
                cout << "IDLE";
                break;
        }

        cout << "\n";
    }
};

class ElevatorController {

private:
    vector<Elevator*> elevators;

public:

    void addElevator(Elevator* elevator) {
        elevators.push_back(elevator);
    }

    Elevator* findBestElevator(int floor) {

        Elevator* bestElevator = nullptr;
        int minDistance = INT_MAX;

        for (auto elevator : elevators) {

            int distance =
                elevator->distanceFrom(floor);

            if (distance < minDistance) {

                minDistance = distance;
                bestElevator = elevator;
            }
        }

        return bestElevator;
    }

    void requestElevator(
        int floor,
        Direction direction)
    {

        Elevator* elevator =
            findBestElevator(floor);

        if (!elevator) {
            cout << "No elevator available\n";
            return;
        }

        cout << "Request at floor "
             << floor
             << " assigned to Elevator "
             << elevator->getId()
             << "\n";

        elevator->addExternalRequest(floor);
    }

    void selectFloor(
        int elevatorId,
        int destinationFloor)
    {

        for (auto elevator : elevators) {

            if (elevator->getId() ==
                elevatorId) {

                elevator->addInternalRequest(
                    destinationFloor);

                cout << "Inside Elevator "
                     << elevatorId
                     << " -> destination "
                     << destinationFloor
                     << " added\n";

                return;
            }
        }

        cout << "Elevator not found\n";
    }

    void processStep() {

        cout << "\nProcessing Step\n";

        for (auto elevator : elevators) {

            if (elevator->hasPendingRequests()) {
                elevator->processNextRequest();
            }
        }
    }

    void displaySystemStatus() {

        cout << "\n========== STATUS ==========\n";

        for (auto elevator : elevators) {
            elevator->displayStatus();
        }

        cout << "============================\n";
    }
};

int main() {

    Elevator e1(1);
    Elevator e2(2);
    Elevator e3(3);

    ElevatorController controller;

    controller.addElevator(&e1);
    controller.addElevator(&e2);
    controller.addElevator(&e3);

    // External requests
    controller.requestElevator(
        5,
        Direction::UP);

    controller.requestElevator(
        12,
        Direction::DOWN);

    controller.requestElevator(
        3,
        Direction::UP);

    controller.displaySystemStatus();

    controller.processStep();

    controller.displaySystemStatus();

    // User enters elevator and presses buttons
    controller.selectFloor(1, 15);
    controller.selectFloor(2, 2);

    controller.processStep();

    controller.displaySystemStatus();

    controller.processStep();

    controller.displaySystemStatus();

    return 0;
}