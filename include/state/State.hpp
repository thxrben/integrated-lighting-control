#ifndef STATE_HPP
#define STATE_HPP

enum State { STARTUP, RUNNING, STOP_REQUEST, STOPPING};


inline State currentState = STARTUP;


#endif