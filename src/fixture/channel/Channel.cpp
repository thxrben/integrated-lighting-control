#include "fixture/channel/Channel.hpp"

Channel::Channel()
{
    Channel(1, 0);
}

Channel::Channel(unsigned char size, uint16_t addr)
{
    if (!(size == 8 || size == 16 || size == 32 || size == 64))
        throw std::invalid_argument("Size of channel is not 8, 16, 32 or 64 bits.");

    if ((addr < 0) || (addr > 512))
        throw std::invalid_argument("Address of channel is not correct");

    uint8_t numOfChannelsOccupied = size / 4;

    if (addr + numOfChannelsOccupied > 512)
        throw std::invalid_argument("This channel tried to occupy a channel outside an universe boundary");

    this->channelSize = size;
    this->fixtureDmxOffset = addr;

    this->value = (unsigned char *)malloc(numOfChannelsOccupied);
    this->blackoutValue = (unsigned char *)malloc(numOfChannelsOccupied);
    this->highlightValue = (unsigned char *)malloc(numOfChannelsOccupied);

    if (this->value == nullptr)
        throw std::runtime_error("Could not allocate enough memory for the channel.");

    *(this->value) = 0xFF;
};

void Channel::freeMemory()
{
    if (this->value != nullptr)
        free(this->value);

    if (this->blackoutValue != nullptr)
        free(this->blackoutValue);

    if (this->highlightValue != nullptr)
        free(this->highlightValue);

    this->value = nullptr;
    this->blackoutValue = nullptr;
    this->highlightValue = nullptr;
}

uint8_t *Channel::getValue()
{
    return (this->hasMemoryAllocated ? this->value : 0);
}

uint64_t Channel::getMaxValue()
{
    return (1u << static_cast<uint64_t>(this->channelSize)) - 1u;
}

uint8_t *Channel::getBlackoutValue()
{
    return (this->hasMemoryAllocated ? this->value : 0);
}

uint8_t *Channel::getHighlightValue()
{
    return (this->hasMemoryAllocated ? this->value : 0);
}

uint8_t Channel::getChannelSize()
{
    return this->channelSize;
}

uint16_t Channel::getChannelStartAddress()
{
    return (this->channelSize == 0 ? 0 : this->fixtureDmxOffset);
}

void Channel::setValue(uint8_t *value)
{
    memcpy(this->value, value, this->getChannelSize());
};

void Channel::setBlackoutValue(uint8_t *value)
{
    memcpy(this->blackoutValue, value, this->getChannelSize());
};

void Channel::setHighlightValue(uint8_t *value)
{
    memcpy(this->highlightValue, value, this->getChannelSize());
};

void Channel::setValuePercent(double value)
{
    double scaledValue = std::clamp(value, 0.0, 1.0) * this->getMaxValue();
    uint64_t newVal = static_cast<uint64_t>(scaledValue);

    uint8_t buffer[8];
    uintToBytesBE(newVal, buffer, this->getChannelSize());
    this->setValue(buffer);
};

void Channel::setBlackoutPercent(double value)
{
    double scaledValue = std::clamp(value, 0.0, 1.0) * this->getMaxValue();
    uint64_t newVal = static_cast<uint64_t>(scaledValue);

    uint8_t buffer[8];
    uintToBytesBE(newVal, buffer, this->getChannelSize());
    this->setValue(buffer);
};

void Channel::setHighlightValue(double value)
{
    double scaledValue = std::clamp(value, 0.0, 1.0) * this->getMaxValue();
    uint64_t newVal = static_cast<uint64_t>(scaledValue);

    uint8_t buffer[8];
    uintToBytesBE(newVal, buffer, this->getChannelSize());
    this->setValue(buffer);
};