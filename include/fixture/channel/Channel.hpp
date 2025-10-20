#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdint>
#include <cassert>
#include <cstring>
#include <algorithm>

#include <fixture/channel/ChannelType.hpp>

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

static void uintToBytesBE(uint64_t value, uint8_t* out, size_t numBytes) {
    for (size_t i = 0; i < numBytes; ++i) {
        out[numBytes - 1 - i] = static_cast<uint8_t>(value & 0xFF);
        value >>= 8;
    }
}

class Channel {

    public:
        Channel();
        Channel(unsigned char size, uint16_t addr);
        void freeMemory();

        uint8_t getChannelSize();
        uint16_t getChannelStartAddress();
        
        uint8_t* getValue();
        uint64_t getMaxValue();
        uint8_t* getBlackoutValue();
        uint8_t* getHighlightValue();

        void setValue(uint8_t* value);
        void setBlackoutValue(uint8_t* value);
        void setHighlightValue(uint8_t* value);

        void setValuePercent(double value);
        void setBlackoutPercent(double value);
        void setHighlightValue(double value);
        
    private:
        ChannelType type;
        uint16_t fixtureDmxOffset; // If ChannelSize = 0, this field is ignored (will read 0). Starts at channel X, if 16/32 Bit mode is used the channel will traverse between X -> X+1/X+2/X+3 
        uint8_t* value;
        uint8_t* blackoutValue;
        uint8_t* highlightValue;
        uint8_t channelSize; // (0,1,2,3,4) declares 0, 8, 16, 24 or 32 Bit mode (Value in Number of Bytes for Channel)
        
        bool hasMemoryAllocated;
};

#endif