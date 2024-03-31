#include "fixture/channel/Channel.hpp"

Channel::Channel() {
    Channel(1,0);
}

Channel::Channel(unsigned char size, unsigned char addr) {
    this->channelSize = size;
    this->address = addr;  
    this->value = (unsigned char *) malloc(size);
    this->hasMemoryAllocated = true;

    *(this->value) = 0xFF;  
};

unsigned char* Channel::getValue() {
    return (this->hasMemoryAllocated ? this->value : 0);
}

unsigned char Channel::getChannelSize() {
    return this->channelSize;
}

unsigned char Channel::getChannelStartAddress() {
    return (this->channelSize == 0 ? 0 : this->address);
}

void Channel::freeMemory() {
    this->hasMemoryAllocated = false;
    if(this->value == nullptr)
        return;
    free(this->value);
    this->value = nullptr;
}