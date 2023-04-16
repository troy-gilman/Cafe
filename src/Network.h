#pragma once

#include "Event.h"
#include <netinet/in.h>

namespace Network {

    static const int PORT = 8080;
    static const int DOMAIN = AF_INET;
    static const int TYPE = SOCK_STREAM;
    static const int PROTOCOL = 0;
    static const int OPT = 1;
    static const int MAX_EVENTS_PER_MSG = 100;

    struct EventMsg {
        size_t numEvents;
        Event::Event events[MAX_EVENTS_PER_MSG];
    };

    struct NetworkState {
        bool isServer;
        int mainSocket;
        int secondSocket;
        struct sockaddr_in address;
        EventMsg eventMsgIn;
        EventMsg eventMsgOut;
    };

    void startServer(NetworkState* state);
    void startClient(NetworkState* state);
    void sendEventMsg(NetworkState* state);
    void receiveEventMsg(NetworkState* state);
}