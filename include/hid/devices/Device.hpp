

#ifndef HID_DEVICE_HPP
#define HID_DEVICE_HPP

class HidDevice {
    public:
        HidDevice();
        virtual ~HidDevice() = default;
        virtual void open() = 0;
        virtual void close() = 0;
        virtual void write() = 0;
        virtual void read() = 0;
};

#endif