#pragma once

#include <netinet/in.h>
#include "Event.hpp"
#include "ObjectPool.hpp"

class NetworkController {
private:
    bool isServer;
    int mainSocket;
    int newSocket;

    int domain = AF_INET;
    int type = SOCK_STREAM;
    int protocol = 0;
    long valRead;
    struct sockaddr_in address;
    int opt = 1;
    int addrLen = sizeof(address);

public:
    EventMsg eventMsgIn;
    EventMsg eventMsgOut;

public:
    NetworkController(bool isServer);
    ~NetworkController();
    void start();
    void stop();
    void sendEventMsg();
    void receiveEventMsg();


private:
    void startServer();
    void startClient();
    void createSocket();
};