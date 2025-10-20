# ILC

ILC stands for Integrated Lightinc Control and is a system capable of generating lighting effects with its dmx engine.

## How to compile:

Create a folder called build/ and enter it:

> /: mkdir build && cd build

Invoke cmake to generate all Makefiles:

> build/: cmake ..

Now the build files are localed in the build/ directory.
This means you can invoke Make to compile the project:

> build/: make all

The output should appear in the bin/ folder insice the build/ directory:

> build/bin/: ls -la

To execute from the build directory:

> build/: ./bin/ilc

ILC should now start. You can exit via CTRL+C (forcefully).
To terminate it correctly, use the SIGTERM (15) signal. This assumes you know the Process ID ([PID]) through e.g. ps, top or similar.

> /: kill -15 [PID]

## Dependencies

This project relies on dependencies.
This list contains the list of dependencies needed:

1. ZeroMQ / zmqpp (ZMQ for C++)
    When using Arch, you can install it from the AUR, e.g.:
    > /: yay -S zmqpp-git
2. Boost
3. libremidi

    You can clone the libremidi git repo:
    > /: mkdir libraries

    > /libraries: git clone git@github.com:celtera/libremidi.git

    The known working version is commit 9dd44c5

4. readerwriterqueue

    You can clone the readerwriterqueue git repo:
    > /: mkdir libraries

    > /libraries: git clone https://github.com/cameron314/readerwriterqueue

    The known working version is 1.0.7 / commit 8b21766


## LUA - Wireshark Dissector
This repo includes a Wireshark-Dissector which parses network traffic and analyzes it.
It adds support for the "ILCNET" protocol.
