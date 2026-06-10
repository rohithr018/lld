#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>

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
public:
    string number;
    VehicleType type;

    Vehicle(string num, VehicleType t)
        : number(num), type(t) {}
};

struct ParkingSlot {
    int floor;
    int slotId;
    SlotType type;
    bool occupied;

    ParkingSlot(int f, int id, SlotType t)
        : floor(f), slotId(id), type(t), occupied(false) {}
};

struct CompareSlot {
    bool operator()(ParkingSlot* a,
                    ParkingSlot* b) const {

        if(a->floor == b->floor)
            return a->slotId < b->slotId;

        return a->floor < b->floor;
    }
};

class ParkingLot {

private:

    set<ParkingSlot*, CompareSlot> freeSmall;
    set<ParkingSlot*, CompareSlot> freeMedium;
    set<ParkingSlot*, CompareSlot> freeLarge;

    unordered_map<string, ParkingSlot*> parkedVehicle;

public:

    vector<ParkingSlot*> allSlots;

    void addSlot(int floor,
                 int slotId,
                 SlotType type) {

        ParkingSlot* slot =
            new ParkingSlot(floor, slotId, type);

        allSlots.push_back(slot);

        if(type == SlotType::SMALL)
            freeSmall.insert(slot);

        else if(type == SlotType::MEDIUM)
            freeMedium.insert(slot);

        else
            freeLarge.insert(slot);
    }

    ParkingSlot* allocateSlot(VehicleType type) {

        if(type == VehicleType::BIKE) {

            if(!freeSmall.empty())
                return *freeSmall.begin();

            if(!freeMedium.empty())
                return *freeMedium.begin();

            if(!freeLarge.empty())
                return *freeLarge.begin();
        }

        else if(type == VehicleType::CAR) {

            if(!freeMedium.empty())
                return *freeMedium.begin();

            if(!freeLarge.empty())
                return *freeLarge.begin();
        }

        else {

            if(!freeLarge.empty())
                return *freeLarge.begin();
        }

        return nullptr;
    }

    bool parkVehicle(Vehicle& vehicle) {

        if(parkedVehicle.count(vehicle.number))
            return false;

        ParkingSlot* slot =
            allocateSlot(vehicle.type);

        if(!slot) {
            cout << "Parking Full\n";
            return false;
        }

        slot->occupied = true;

        if(slot->type == SlotType::SMALL)
            freeSmall.erase(slot);

        else if(slot->type == SlotType::MEDIUM)
            freeMedium.erase(slot);

        else
            freeLarge.erase(slot);

        parkedVehicle[vehicle.number] = slot;

        cout << vehicle.number
             << " parked at Floor "
             << slot->floor
             << " Slot "
             << slot->slotId
             << endl;

        return true;
    }

    bool unparkVehicle(string vehicleNo) {

        auto it = parkedVehicle.find(vehicleNo);

        if(it == parkedVehicle.end())
            return false;

        ParkingSlot* slot = it->second;

        slot->occupied = false;

        if(slot->type == SlotType::SMALL)
            freeSmall.insert(slot);

        else if(slot->type == SlotType::MEDIUM)
            freeMedium.insert(slot);

        else
            freeLarge.insert(slot);

        parkedVehicle.erase(it);

        cout << vehicleNo
             << " unparked\n";

        return true;
    }

    void displayFreeSlots() {

        cout << "\nFree Slots\n";

        cout << "Small : "
             << freeSmall.size() << endl;

        cout << "Medium : "
             << freeMedium.size() << endl;

        cout << "Large : "
             << freeLarge.size() << endl;
    }
};

int main() {

    ParkingLot lot;

    lot.addSlot(1,1,SlotType::SMALL);
    lot.addSlot(1,2,SlotType::MEDIUM);
    lot.addSlot(1,3,SlotType::LARGE);

    lot.addSlot(2,1,SlotType::SMALL);
    lot.addSlot(2,2,SlotType::MEDIUM);
    lot.addSlot(2,3,SlotType::LARGE);

    Vehicle bike("B101", VehicleType::BIKE);
    Vehicle car("C101", VehicleType::CAR);
    Vehicle truck("T101", VehicleType::TRUCK);

    lot.parkVehicle(bike);
    lot.parkVehicle(car);
    lot.parkVehicle(truck);

    lot.displayFreeSlots();

    lot.unparkVehicle("C101");

    lot.displayFreeSlots();
}