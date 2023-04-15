#pragma once

#include "Event.hpp"
#include <netinet/in.h>

namespace Network {

    static const int PORT = 8080;
    static const int DOMAIN = AF_INET;
    static const int TYPE = SOCK_STREAM;
    static const int PROTOCOL = 0;
    static const int OPT = 1;
    static const int MAX_EVENTS_PER_MSG = 100;

    struct EventMsg {
        int numEvents;
        Event events[MAX_EVENTS_PER_MSG];
    };

    struct State {
        bool isServer;
        int mainSocket;
        int secondSocket;
        struct sockaddr_in address;
        EventMsg eventMsgIn;
        EventMsg eventMsgOut;
    };

    void startServer(State* state);
    void startClient(State* state);
    void sendEventMsg(State* state);
    void receiveEventMsg(State* state);
}