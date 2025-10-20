#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <thread>
#include <memory>
#include <mutex>

#include "dmx_engine/FrameLimit.hpp"

#include "dmx_engine/Universe.hpp"

#include "effects/SineWave.hpp"

#include "../dmx_output/Output.hpp"
#include "../dmx_output/ArtNet.hpp"

#ifndef DMXENGINE_HPP
#define DMXENGINE_HPP

#define DEFAULT_DMX_UNIVERSES 16
#define MAX_DMX_UNIVERSES 20

typedef struct EffectMeta {
    EffectMeta(std::shared_ptr<Effect> effect1, std::chrono::milliseconds startMs1) 
    : effect(effect1), startMs(startMs1) 
    { }
    std::shared_ptr<Effect> effect;
    std::chrono::milliseconds startMs;
};

class DMXEngine {
    private:
        int universeNum;
        unsigned char sequenceNum;
        double smoothingValue = 0.94;
        double averageOutputFps;
        double averageEffectFps;
        std::vector<std::shared_ptr<Universe>> universes;
        std::vector<std::shared_ptr<Output>> dmxOutputs;
        std::vector<std::shared_ptr<EffectMeta>> runningEffects;


        std::thread dmxEngineInputThread;
        std::thread dmxEngineOutputThread;
        std::thread dmxEngineEffectThread;

        std::mutex runningEffectsLock;

        bool shouldThreadStop;
        void runOutput();
        void runEffects();
        void runInputs();
    
    public:
        DMXEngine();
        DMXEngine(unsigned short universes);
        void addOutput(std::shared_ptr<Output> output);
        void setChannel(unsigned short universes, unsigned char value);
        void start();
        void stop();
        void freeMemory();
        double getAverageOutputFps() { return this->averageOutputFps; }
        double getAverageEffectFps() { return this->averageEffectFps; }
};

#endif