#include "Station.hpp"

Station::Station() {
    this->macAddr = 0;
};

Station::Station(in_addr ipAddr) {
    this->ipAddr = ipAddr;
}