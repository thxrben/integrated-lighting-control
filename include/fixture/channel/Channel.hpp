#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdint>
#include <cassert>
#include <cstring>

#include <fixture/channel/ChannelType.hpp>

#ifndef CHANNEL_HPP
#define CHANNEL_HPP


class Channel {

    public:
        Channel();
        Channel(unsigned char size, uint16_t addr);
        unsigned char getChannelSize();
        unsigned char getChannelStartAddress();
        unsigned char* getValue();
        void freeMemory();
    private:
        ChannelType type;
        uint8_t fixtureDmxOffset; // If ChannelSize = 0, this field is ignored (will read 0). Starts at channel X, if 16/32 Bit mode is used the channel will traverse between X -> X+1/X+2/X+3 
        uint8_t* value;
        uint8_t channelSize; // (0,1,2,3,4) declares 0, 8, 16, 24 or 32 Bit mode (Value in Number of Bytes for Channel)
        
        bool hasMemoryAllocated;
};

#endif