#include "DMXEngine.hpp"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"

#define ANSI_COLOR_RESET   "\x1b[0m"

//Utility Function
int min(int i, int j) {
    return (i < j ? i : j);
};

using namespace std;
using namespace std::chrono;

DMXEngine::DMXEngine() {
    for(int i = 0; i < DEFAULT_DMX_UNIVERSES; i++) {
        this->universes.push_back(std::make_shared<Universe>(i));
    }
    printf("[DMXEngine] Initiated %d universes.\n", DEFAULT_DMX_UNIVERSES);
    this->universeNum = DEFAULT_DMX_UNIVERSES;
    this->sequenceNum = 0;
}

DMXEngine::DMXEngine(unsigned short universes) {
    universes = min(universes, MAX_DMX_UNIVERSES);
    for(int i = 0; i < universes; i++) {
        this->universes.push_back(std::make_shared<Universe>(i));
    }
    printf("[DMXEngine] Initiated %d universes.\n", universes);
    this->universeNum = universes;
    this->sequenceNum = 0;
};

/*
    This is the Output-Thread
*/
void DMXEngine::runOutput() {
    printf(ANSI_COLOR_RESET "[" ANSI_COLOR_YELLOW "DMXENGINE" ANSI_COLOR_RESET "] : Outputloop " ANSI_COLOR_GREEN "started" ANSI_COLOR_RESET " on PID %d / TID %d.\n", getpid(), gettid());
    this->shouldThreadStop = false;
    
    FrameLimit<42> frameLimiter;
    while(!this->shouldThreadStop) {
        //printf("[DMXENGINE] Shall not stop.\n");
        
        //TODO: Apply the effects here!
        runInputs();

        for(int i = 0; i < this->universeNum; i++) {
            std::shared_ptr<Universe> currentUniverse;
            currentUniverse = this->universes.at(i);
            
            unsigned char* dmxValues = (currentUniverse->getDMXValues());
            
            for(int i = 0; i < this->dmxOutputs.size(); i++) {
                std::shared_ptr<Output> output = dmxOutputs.at(i);
                output->sendUniverse(currentUniverse, sequenceNum);
            }
        }
    
        sequenceNum++;
        frameLimiter.sleep(); // Should limit to 42 dmx frames per sec
    }
    printf(ANSI_COLOR_RESET "[" ANSI_COLOR_YELLOW "DMXENGINE" ANSI_COLOR_RESET "] : Output-Thread has stopped.\n");
};

/*
    This is the Effect-Thread
*/
void DMXEngine::runEffects() {
    printf(ANSI_COLOR_RESET "[" ANSI_COLOR_YELLOW "DMXENGINE" ANSI_COLOR_RESET "] : Effectloop " ANSI_COLOR_GREEN "started" ANSI_COLOR_RESET " on PID %d / TID %d.\n", getpid(), gettid());

    std::chrono::milliseconds currentMs = duration_cast< milliseconds >(
        system_clock::now().time_since_epoch()
    );

    std::shared_ptr<SineWave> newSineWave = std::make_shared<SineWave>();
    std::shared_ptr<Effect> newBaseEffect = std::dynamic_pointer_cast<Effect>(newSineWave);
    std::shared_ptr newEffectsMeta = std::make_shared<EffectMeta>(newBaseEffect, currentMs);

    this->runningEffects.push_back(newEffectsMeta);

    FrameLimit<5> frameLimiter;

    while(!this->shouldThreadStop) {
        
        // Process effects here:

        // TODO: We can allow insertion, since its APPENDED and we can always iterate one more object
        // We should only lock for removal of objects!

        // Lock the Thread so Effects can't be deleted:
        runningEffectsLock.lock();

        // Calculate current Time:

        milliseconds currentMs = duration_cast< milliseconds >(
            system_clock::now().time_since_epoch()
        );

        for(std::shared_ptr<EffectMeta> effectMeta : this->runningEffects) {
            milliseconds diffMs = currentMs - effectMeta->startMs; // Calculate diff
            printf("%d\n", effectMeta->effect->calcValue(diffMs)); // Run Effect
            //Assign Effect result to selection
        }


        runningEffectsLock.unlock();

        frameLimiter.sleep(); // Should limit to 60 effect updates per sec
    }
    
    printf(ANSI_COLOR_RESET "[" ANSI_COLOR_YELLOW "DMXENGINE" ANSI_COLOR_RESET "] : Effect-Thread has stopped.\n");
}
    
void DMXEngine::runInputs() {
    // FrameLimit<30> frameLimiter;
    // while(!this->shouldThreadStop) {

    //     frameLimiter.sleep();
    // }
    
    // printf(ANSI_COLOR_RESET "[" ANSI_COLOR_YELLOW "DMXENGINE" ANSI_COLOR_RESET "] : Input-Thread has stopped.\n");
}

void DMXEngine::addOutput(std::shared_ptr<Output> output) {
    this->dmxOutputs.push_back(output);
};


void DMXEngine::start() {
    printf(ANSI_COLOR_RESET "[" ANSI_COLOR_YELLOW "DMXENGINE" ANSI_COLOR_RESET "] : Starting Thread \n");
    this->dmxEngineOutputThread = std::thread(&DMXEngine::runOutput, this);
    this->dmxEngineEffectThread = std::thread(&DMXEngine::runEffects, this);
  //  this->dmxEngineInputThread = std::thread(&DMXEngine::runInputs, this);
}

void DMXEngine::stop() {
    printf(ANSI_COLOR_RESET "[" ANSI_COLOR_YELLOW "DMXENGINE" ANSI_COLOR_RESET "] : " ANSI_COLOR_RED "Stopping" ANSI_COLOR_RESET " Thread \n");
    this->shouldThreadStop = true;
    this->dmxEngineOutputThread.join();
    this->dmxEngineEffectThread.join();
   // this->dmxEngineInputThread.join();

}

void DMXEngine::freeMemory() {
    for(std::shared_ptr<Output> out : this->dmxOutputs) {
        out->stop();
        out->freeMemory();
        out.reset();
    }

    for(std::shared_ptr<Universe> uni : this->universes) {
        uni->freeMemory();
        uni.reset();
    }

    this->dmxOutputs.clear();
    this->universes.clear();
}