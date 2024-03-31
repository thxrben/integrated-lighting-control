#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "channel/Channel.hpp"

#ifndef FIXTURE_HPP
#define FIXTURE_HPP

class Fixture {
    public:
        Fixture();
        void freeMemory();    
        std::vector<Channel*> getChannelsAsCpy();    
    private:
        std::vector<Channel*> channels;

};

#endif