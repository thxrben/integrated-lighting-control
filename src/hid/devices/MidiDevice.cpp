#include "hid/devices/MidiDevice.hpp"

using namespace std;

MidiDevice::MidiDevice() {

};

void MidiDevice::open()
{
    // Open the default MIDI Ports:
    if (auto port = libremidi::midi1::out_default_port())
    {
        midiOut.open_port(*port);
    }

    auto my_callback = [this](const libremidi::message &message)
    {
        printf("[MIDI] (IN) Received %d bytes at %Ld ", message.size(), message.timestamp);
        printf(" %d\t%d\t%d\n", message[0], message[1], message[2]);

        if(message[0] == 144) {
            midiOut.send_message(libremidi::channel_events::note_on(0x00, message[1], 1));
        } else if (message[0] == 128) {
            midiOut.send_message(libremidi::channel_events::note_on(0x00, message[1], 0));
        }

    };

    libremidi::input_configuration config;
    config.on_message = my_callback;
    midiIn = std::make_unique<libremidi::midi_in>(config);

    if (auto port = libremidi::midi1::in_default_port())
        midiIn->open_port(*port);
};

void MidiDevice::close()
{
    for (int i = 0; i < 64; i++)
    {
        midiOut.send_message(libremidi::channel_events::note_on(0x00, i, 0));
    }
};

void MidiDevice::write() {

};

void MidiDevice::read() {

};