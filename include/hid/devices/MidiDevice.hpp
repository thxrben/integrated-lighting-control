#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <chrono>
#include <csignal>
#include <memory>

#include <libremidi/libremidi.hpp>


#ifndef HID_MIDI_DEVICE_HPP
#define HID_MIDI_DEVICE_HPP


class MidiDevice {
        public:
            MidiDevice();
            void open();
            void close();
            void write();
            void read();
        private:
            std::unique_ptr<libremidi::midi_in> midiIn;
            libremidi::midi_out midiOut;
};

#endif