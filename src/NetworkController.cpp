#include "NetworkController.hpp"
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>

#define PORT 8080

NetworkController::NetworkController(bool isServer) : isServer(isServer) {
}

NetworkController::~NetworkController() {
    stop();
}

void NetworkController::start() {
    if (isServer) {
        startServer();
    } else {
        startClient();
    }
}

void NetworkController::stop() {
    if (isServer) {
        close(newSocket);
        shutdown(mainSocket, SHUT_RDWR);
    } else {
        close(mainSocket);
    }
}

void NetworkController::sendEventMsg() {
    int socket = isServer ? newSocket : mainSocket;
    send(socket, &eventMsgOut, sizeof(eventMsgOut), 0);
    eventMsgOut.numEvents = 0;
}

void NetworkController::receiveEventMsg() {
    int socket = isServer ? newSocket : mainSocket;
    read(socket, &eventMsgIn, sizeof(eventMsgIn));
}

void NetworkController::startServer() {
    createSocket();

    // Forcefully attaching socket to the port 8080
    if (setsockopt(mainSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = domain;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(mainSocket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "Listening on port " << PORT << '\n';
    if (listen(mainSocket, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    std::cout << "Waiting for connection" << '\n';
    if ((newSocket = accept(mainSocket, (struct sockaddr *)&address, (socklen_t*)&addrLen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    std::cout << "Connection established" << '\n';
}

void NetworkController::startClient() {
    createSocket();

    address.sin_family = domain;
    address.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(domain, "127.0.0.1", &address.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        exit(EXIT_FAILURE);
    }

    if (connect(mainSocket, (struct sockaddr *) &address, sizeof(address)) < 0) {
        printf("\nConnection Failed \n");
        exit(EXIT_FAILURE);
    }
    std::cout << "Connected to server\n";
}

void NetworkController::createSocket() {
    if ((mainSocket = socket(domain, type, protocol)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
}
