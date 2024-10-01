#include <iostream>
#include <map>
#include <algorithm>
using namespace std;

enum VehicleType {
    CAR,
    BIKE,
    TRUCK
};

class Vehicle {
    VehicleType vehicle_type;
    string registration_number;
    string color;

    public:
    Vehicle(VehicleType _v_type, string _reg_num, string _color) {
        vehicle_type = _v_type;
        registration_number = _reg_num;
        color = _color;
    }

    VehicleType get_vehicle_type() {
        return vehicle_type;
    }

    string getReg() {
        return registration_number;
    }
};

class ParkingSpot {
    VehicleType vehicle_type;
    string ticket;
    bool isOccupied;
    int floor_number;
    string reg;

    public:
    ParkingSpot(VehicleType type) {
        vehicle_type = type;
        isOccupied = false;
        ticket = "";
        reg = "";
        floor_number = 0;
    }

    VehicleType get_vehicle_type() {
        return vehicle_type;
    }

    bool isCurrentlyOccupied() {
        return isOccupied;
    }

    string getReg() {
        return reg;
    }

    ParkingSpot * setParking(Vehicle v, string parking_lot_id, int floor, int spot) {
        if (isOccupied || ticket != "") return this;
        isOccupied = true;
        floor_number = floor;
        reg = v.getReg();
        ticket = parking_lot_id + '_' + to_string(floor) + '_' + to_string(spot);
        return this;
    }

    string getTicket() {
        return ticket;
    }

    bool emptySpot() {
        if (!isOccupied) return false;
        if (ticket == "") return false;
        isOccupied = false;
        ticket = "";
        reg = "";
        return true;
    }

    int getFloorNumber() {
        return floor_number;
    }
};

class ParkingFloor {

    int floor_number;

    int trucks;
    int bikes;
    int cars;

    int max_trucks;
    int max_bikes;
    int max_cars; 

    int capacity;
    map<int, ParkingSpot * > map_parking_slots;

    public:
    ParkingFloor(int _slots_per_floor, int floor) {

        if (_slots_per_floor <= 0) exit(1);

        floor_number = floor;

        trucks = 0;
        bikes = 0;
        cars = 0;

        max_trucks = 1;
        max_bikes = min(2, _slots_per_floor - max_trucks);
        max_cars = max(0, _slots_per_floor - max_bikes - max_trucks);

        capacity = _slots_per_floor;

        for (int i = 1; i <= capacity; i++) {
            if (i == 1) {
                map_parking_slots[i] = new ParkingSpot(TRUCK);
            } else if (i <= 3) {
                map_parking_slots[i] = new ParkingSpot(BIKE);
            } else {
                map_parking_slots[i] = new ParkingSpot(CAR);
            }
        }
    }

    int getVehicleCount() {
        return trucks + bikes + cars;
    }

    bool hasSlots(VehicleType v_type) {
        if (v_type == CAR) {
            return cars < max_cars;
        } else if (v_type == BIKE) {
            return bikes < max_bikes;
        } else if (v_type == TRUCK) {
            return trucks < max_trucks;
        } else {
            return false;
        }
    }

    int getFreeSlots(VehicleType v_type) {
        if (v_type == CAR) {
            return max(0, max_cars - cars);
        } else if (v_type == TRUCK) {
            return max(0, max_trucks - trucks);
        } else if (v_type == BIKE) {
            return max(0, max_bikes - bikes);
        } else return 0;
    }

    int getCount(VehicleType v_type) {
        if (v_type == CAR) {
            return cars;
        } else if (v_type == TRUCK) {
            return trucks;
        } else if (v_type == BIKE) {
            return bikes;
        } return 0;
    }

    int getMaxCapacity() {
        return capacity;
    }

    void display_free_spots(VehicleType vtype) {
        for (int i = 1; i <= capacity; i++) {
            ParkingSpot *ps = map_parking_slots[i];
            if (vtype == ps->get_vehicle_type() && !(ps->isCurrentlyOccupied())) {
                cout << "\nEmpty spot " << floor_number << " " << i;
            }
        }
    }

    void display_occupied_spots(VehicleType vtype) {
        for (int i = 1; i <= capacity; i++) {
            ParkingSpot *ps = map_parking_slots[i];
            if (vtype == ps->get_vehicle_type() && ps->isCurrentlyOccupied()) {
                cout << "\nUsed spot " << floor_number << " " << i;
            }
        }
    }

    void decrement_count(VehicleType vtype) {
        if (vtype == CAR && cars > 0) {
            cars -= 1;
        } else if (vtype == TRUCK && trucks > 0) {
            trucks -= 1;
        } else if (vtype == BIKE && bikes > 0) {
            bikes -= 1;
        }
    }

    ParkingSpot * park_on_floor(Vehicle v, string parking_lot_id) {

        if (!hasSlots(v.get_vehicle_type())) return NULL;

        if (v.get_vehicle_type() == TRUCK) {

            if (!(map_parking_slots[1] -> isCurrentlyOccupied())) {
                trucks += 1;
                return map_parking_slots[1] -> setParking(v, parking_lot_id, floor_number, 1);       
            }

        } else if (v.get_vehicle_type() == BIKE) {

            for (int spot = 2; spot <= min(3, capacity - 1); spot++) {

                if (!(map_parking_slots[spot] -> isCurrentlyOccupied())) {
                    bikes += 1;
                    return map_parking_slots[spot] -> setParking(v, parking_lot_id, floor_number, spot);       
                }
            }
            return NULL;

        } else if (v.get_vehicle_type() == CAR) {

            for (int spot = 4; spot <= capacity; spot++) {

                if (!(map_parking_slots[spot] -> isCurrentlyOccupied())) {
                    cars += 1;
                    return map_parking_slots[spot] -> setParking(v, parking_lot_id, floor_number, spot);       
                }
            }

            return NULL;

        }
        return NULL;
    }
};

class ParkingLot {
    string parking_lot_id;
    int floors;
    map<int, ParkingFloor * > map_parking_floor;
    map<string, ParkingSpot * > map_ticket_to_vehicle;

    public:
    ParkingLot(string _parking_lot_id, int _floors, int _slots_per_floor) {

        if (_floors <= 0) exit(1);

        parking_lot_id = _parking_lot_id;
        floors = _floors;
        
        for (int i = 1; i <= floors; i++) {
            map_parking_floor[i] = new ParkingFloor(_slots_per_floor, i);
        }
    }

    string park_vehicle(Vehicle to_park) {
        for (int floor = 1; floor <= floors; floor++) {
            if (map_parking_floor[floor]->hasSlots(to_park.get_vehicle_type())) {
                ParkingSpot * spot = map_parking_floor[floor]->park_on_floor(to_park, parking_lot_id);
                if (spot != NULL && !spot->getTicket().empty() && spot -> isCurrentlyOccupied()) {
                    map_ticket_to_vehicle[spot->getTicket()] = spot;
                    cout << "\nParking successful! : " << spot -> getTicket();
                    return spot -> getTicket();
                } else {
                    cout << "\nParking failed!\n";
                    return "";
                }
                return "";
            }
        }
        return "";
    }

    bool unpark_vehicle(string ticket) {
        ParkingSpot *spot = map_ticket_to_vehicle[ticket];
        if (spot && spot -> getTicket() == ticket && spot -> isCurrentlyOccupied()) {
            int parked_on_floor = spot -> getFloorNumber();
            VehicleType vtype = spot -> get_vehicle_type();
            string t = spot -> getTicket();
            string reg = spot -> getReg();
            bool emptySpot = spot -> emptySpot();
            if (emptySpot) {
                map_parking_floor[parked_on_floor] -> decrement_count(vtype);
                cout << "\nSuccessfully unparked! " << t;
                cout << "\nUnpark reg! " << reg;
                return true;
            } else {
                cout << "\nSome error";
                return false;
            }
        } else {
            cout << "\nTicket doesnt exist in database " << ticket;
            return false;
        }
    }

    void display_free_count(VehicleType vtype) {
        for (int floor = 1; floor <= floors; floor++) {
            ParkingFloor *pf = map_parking_floor[floor];
            cout << "\n" << to_string(vtype) << " No of free slots for on floor " << floor << " " << pf->getFreeSlots(vtype);
        }
    }

    void display_free_slots(VehicleType vtype) {
        for (int floor = 1; floor <= floors; floor++) {
            ParkingFloor *pf = map_parking_floor[floor];
            pf -> display_free_spots(vtype);
        }
    }

    void display_used_slots(VehicleType vtype) {
        for (int floor = 1; floor <= floors; floor++) {
            ParkingFloor *pf = map_parking_floor[floor];
            pf -> display_occupied_spots(vtype);
        }
    }

};


int main() {
    ParkingLot parkingLot = ParkingLot("PR1234", 2, 6);
    
    parkingLot.display_free_count(CAR);
    parkingLot.display_free_count(BIKE);
    parkingLot.display_free_count(TRUCK);
    
    parkingLot.display_free_slots(CAR);
    parkingLot.display_free_slots(BIKE);
    parkingLot.display_free_slots(TRUCK);
    
    parkingLot.display_used_slots(CAR);
    parkingLot.display_used_slots(BIKE);
    parkingLot.display_used_slots(TRUCK);
    
    parkingLot.park_vehicle(Vehicle(CAR, "KA-01-DB-1234", "black"));
    parkingLot.park_vehicle(Vehicle(CAR, "KA-02-CB-1334", "red"));
    parkingLot.park_vehicle(Vehicle(CAR, "KA-01-DB-1133", "black"));
    parkingLot.park_vehicle(Vehicle(CAR, "KA-05-HJ-8432", "white"));
    parkingLot.park_vehicle(Vehicle(CAR, "WB-45-HO-9032", "white"));
    parkingLot.park_vehicle(Vehicle(CAR, "KA-01-DF-8230", "black"));
    parkingLot.park_vehicle(Vehicle(CAR, "KA-21-HS-2347", "red"));
    
    parkingLot.display_free_count(CAR);
    parkingLot.display_free_count(BIKE);
    parkingLot.display_free_count(TRUCK);
    
    parkingLot.unpark_vehicle("PR1234_2_5");
    parkingLot.unpark_vehicle("PR1234_2_5");
    parkingLot.unpark_vehicle("PR1234_2_7");
    
    parkingLot.display_free_count(CAR);
    parkingLot.display_free_count(BIKE);
    parkingLot.display_free_count(TRUCK);
    
    parkingLot.display_free_slots(CAR);
    parkingLot.display_free_slots(BIKE);
    parkingLot.display_free_slots(TRUCK);
    
    parkingLot.display_used_slots(CAR);
    parkingLot.display_used_slots(BIKE);
    parkingLot.display_used_slots(TRUCK);
    
    parkingLot.park_vehicle(Vehicle(BIKE, "KA-01-DB-1541", "black"));
    parkingLot.park_vehicle(Vehicle(TRUCK, "KA-32-SJ-5389", "orange"));
    parkingLot.park_vehicle(Vehicle(TRUCK, "KL-54-DN-4582", "green"));
    parkingLot.park_vehicle(Vehicle(TRUCK, "KL-12-HF-4542", "green"));
    
    parkingLot.display_free_count(CAR);
    parkingLot.display_free_count(BIKE);
    parkingLot.display_free_count(TRUCK);
    
    parkingLot.display_free_slots(CAR);
    parkingLot.display_free_slots(BIKE);
    parkingLot.display_free_slots(TRUCK);
    
    parkingLot.display_used_slots(CAR);
    parkingLot.display_used_slots(BIKE);
    parkingLot.display_used_slots(TRUCK);
    cout << endl;
    return 0;
}

