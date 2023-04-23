#pragma once

#include "Event.h"
#include <netinet/in.h>

// This is needed for macOS to prevent naming conflicts
#ifdef DOMAIN
#undef DOMAIN
#endif


namespace Network {

    static const i32 PORT = 8080;
    static const i32 DOMAIN = AF_INET;
    static const i32 TYPE = SOCK_STREAM;
    static const i32 PROTOCOL = 0;
    static const i32 OPT = 1;
    static const i32 MAX_EVENTS_PER_MSG = 100;

    struct EventMsg {
        ui32 numEvents;
        Event::Event events[MAX_EVENTS_PER_MSG];
    };

    struct NetworkState {
        bool isServer;
        i32 mainSocket;
        i32 secondSocket;
        struct sockaddr_in address;
        EventMsg eventMsgIn;
        EventMsg eventMsgOut;
    };

    void startServer(NetworkState* state);
    void startClient(NetworkState* state);
    void sendEventMsg(NetworkState* state);
    void receiveEventMsg(NetworkState* state);
}