#include "Network.hpp"

#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>

void createSocket(Network::NetworkState *state) {
    state->mainSocket = socket(Network::DOMAIN, Network::TYPE, Network::PROTOCOL);
    if (state->mainSocket < 0) {
        perror("Error creating socket");
        exit(1);
    }
}

void Network::startServer(Network::NetworkState *state) {
    state->isServer = true;
    createSocket(state);

    // Forcefully attaching socket to the port 8080
    if (setsockopt(state->mainSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &Network::OPT, sizeof(Network::OPT))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    state->address.sin_family = Network::DOMAIN;
    state->address.sin_addr.s_addr = INADDR_ANY;
    state->address.sin_port = htons(Network::PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(state->mainSocket, (struct sockaddr *)&state->address, sizeof(state->address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "Listening on port " << PORT << '\n';
    if (listen(state->mainSocket, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    std::cout << "Waiting for connection" << '\n';
    size_t addrLen = sizeof(state->address);
    state->secondSocket = accept(state->mainSocket, (struct sockaddr *)&state->address, (socklen_t*)&addrLen);
    if (state->secondSocket < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    std::cout << "Connection established" << '\n';
}

void Network::startClient(Network::NetworkState *state) {
    state->isServer = false;
    createSocket(state);

    state->address.sin_family = Network::DOMAIN;
    state->address.sin_port = htons(Network::PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(Network::DOMAIN, "127.0.0.1", &state->address.sin_addr) <= 0) {
        std::cout << "Invalid address/ Address not supported" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (connect(state->mainSocket, (struct sockaddr *) &state->address, sizeof(state->address)) < 0) {
        std::cout << "Connection Failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Connected to server\n";
}

void Network::sendEventMsg(Network::NetworkState *state) {
    int socket = state->isServer ? state->secondSocket : state->mainSocket;
    send(socket, &state->eventMsgOut, sizeof(state->eventMsgOut), 0);
    state->eventMsgOut.numEvents = 0;
}

void Network::receiveEventMsg(Network::NetworkState *state) {
    int socket = state->isServer ? state->secondSocket : state->mainSocket;
    read(socket, &state->eventMsgIn, sizeof(state->eventMsgIn));
}