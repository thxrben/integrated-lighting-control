#include "Universe.hpp"

Universe::Universe(unsigned short universe) {
    this->universe = universe;
    this->dmxChannels = (unsigned char*) malloc(512);
    
    memset(this->dmxChannels, 0xFF, 512);

};

void Universe::freeMemory() {
    if(this->dmxChannels == nullptr)
        return;
        
    free(this->dmxChannels);
    this->dmxChannels = nullptr;
    this->fixtures.clear();
};

void Universe::refreshUniverseData() {
    for(int i = this->fixtures.size()-1; i >= 0; i++) {
        Fixture *fixture = this->fixtures.at(i);
        std::vector<Channel*> channels = fixture->getChannelsAsCpy();
        for(int j = 0; j < channels.size(); j++) {
            Channel *channel = channels.at(j);
            memcpy((this->dmxChannels + channel->getChannelStartAddress()), channel->getValue(), channel->getChannelSize());
            // *(this->dmxChannels + channel->getChannelStartAddress()) = channel->getValue();
        }
    }
};

unsigned short Universe::getUniverseNum() {
    return this->universe;
};

unsigned char * Universe::getDMXValues() {
    return this->dmxChannels;
};

void Universe::setDmxValue(unsigned short channel, unsigned char value) {
    
    if(channel >= 512) {
        return;
    }

    *(this->dmxChannels + channel) = value;
}