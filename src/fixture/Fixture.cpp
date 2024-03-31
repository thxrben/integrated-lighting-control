#include "Fixture.hpp"


Fixture::Fixture() {
    this->channels.push_back(new Channel());
};

void Fixture::freeMemory() {
    // for(this->)

    for(int i = this->channels.size() -1; i >= 0; i--) {
        Channel *c = this->channels.at(i);
        delete c;
        this->channels.erase(this->channels.begin()+i);
    }

    this->channels.clear();
}
std::vector<Channel*> Fixture::getChannelsAsCpy() {
    return this->channels;
};