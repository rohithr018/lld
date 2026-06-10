#include <iostream>
#include <vector>
#include <memory>

using namespace std;

enum class VehicleType {
    BIKE,
    CAR,
    TRUCK
};

enum class SlotType {
    SMALL,
    MEDIUM,
    LARGE
};

class Vehicle {
protected:
    string vehicleNumber;
    VehicleType type;

public:
    Vehicle(string num, VehicleType t)
        : vehicleNumber(num), type(t) {}

    virtual ~Vehicle() {}

    VehicleType getType() {
        return type;
    }

    string getNumber() {
        return vehicleNumber;
    }
};

class Bike : public Vehicle {
public:
    Bike(string num)
        : Vehicle(num, VehicleType::BIKE) {}
};

class Car : public Vehicle {
public:
    Car(string num)
        : Vehicle(num, VehicleType::CAR) {}
};

class Truck : public Vehicle {
public:
    Truck(string num)
        : Vehicle(num, VehicleType::TRUCK) {}
};

class ParkingSlot {
    int slotId;
    SlotType slotType;
    bool occupied;
    Vehicle* vehicle;

public:
    ParkingSlot(int id, SlotType type)
        : slotId(id), slotType(type),
          occupied(false), vehicle(nullptr) {}

    bool canFitVehicle(Vehicle* v) {

        VehicleType vt = v->getType();

        if(vt == VehicleType::BIKE)
            return true;

        if(vt == VehicleType::CAR)
            return slotType == SlotType::MEDIUM ||
                   slotType == SlotType::LARGE;

        if(vt == VehicleType::TRUCK)
            return slotType == SlotType::LARGE;

        return false;
    }

    bool parkVehicle(Vehicle* v) {

        if(occupied)
            return false;

        if(!canFitVehicle(v))
            return false;

        vehicle = v;
        occupied = true;

        return true;
    }

    void removeVehicle() {
        vehicle = nullptr;
        occupied = false;
    }

    bool isOccupied() {
        return occupied;
    }

    int getSlotId() {
        return slotId;
    }

    SlotType getSlotType() {
        return slotType;
    }
};

class ParkingFloor {

    int floorId;
    vector<ParkingSlot> slots;

public:

    ParkingFloor(int id)
        : floorId(id) {}

    void addSlot(int slotId, SlotType type) {
        slots.emplace_back(slotId, type);
    }

    bool parkVehicle(Vehicle* v) {

        for(auto &slot : slots) {

            if(slot.parkVehicle(v)) {

                cout << "Vehicle "
                     << v->getNumber()
                     << " parked at Floor "
                     << floorId
                     << ", Slot "
                     << slot.getSlotId()
                     << endl;

                return true;
            }
        }

        return false;
    }

    bool unparkVehicle(int slotId) {

        for(auto &slot : slots) {

            if(slot.getSlotId() == slotId &&
               slot.isOccupied()) {

                slot.removeVehicle();

                cout << "Slot "
                     << slotId
                     << " vacated on Floor "
                     << floorId
                     << endl;

                return true;
            }
        }

        return false;
    }

    void displayAvailability() {

        cout << "Floor " << floorId << endl;

        for(auto &slot : slots) {

            cout << "Slot "
                 << slot.getSlotId()
                 << " -> ";

            if(slot.isOccupied())
                cout << "Occupied";
            else
                cout << "Free";

            cout << endl;
        }
    }
};

class ParkingLot {

    vector<ParkingFloor> floors;

public:

    void addFloor(ParkingFloor floor) {
        floors.push_back(floor);
    }

    void parkVehicle(Vehicle* v) {

        for(auto &floor : floors) {

            if(floor.parkVehicle(v))
                return;
        }

        cout << "Parking Full for Vehicle "
             << v->getNumber()
             << endl;
    }

    void displayStatus() {

        cout << "\nParking Lot Status\n";

        for(auto &floor : floors)
            floor.displayAvailability();
    }
};

int main() {

    ParkingLot lot;

    ParkingFloor f1(1);

    f1.addSlot(1, SlotType::SMALL);
    f1.addSlot(2, SlotType::MEDIUM);
    f1.addSlot(3, SlotType::LARGE);

    ParkingFloor f2(2);

    f2.addSlot(1, SlotType::SMALL);
    f2.addSlot(2, SlotType::MEDIUM);
    f2.addSlot(3, SlotType::LARGE);

    lot.addFloor(f1);
    lot.addFloor(f2);

    Bike bike("KA01-B1");
    Car car("KA01-C1");
    Truck truck("KA01-T1");

    lot.parkVehicle(&bike);
    lot.parkVehicle(&car);
    lot.parkVehicle(&truck);

    lot.displayStatus();

    return 0;
}