#include <string>

#ifndef SESSION_HPP
#define SESSION_HPP

class Session {

    public:
        Session();

    private:
        unsigned int sessionID;
        std::string sessionName;
        unsigned int deviceIDMaster;
        
};

#endif