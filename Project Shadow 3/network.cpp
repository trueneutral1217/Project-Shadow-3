#include "Network.h"
#include <iostream>

Network::Network() {
    if (SDLNet_Init() == -1) {
        std::cerr << "SDLNet_Init: " << SDLNet_GetError() << std::endl;
    }
    socket = nullptr;
}

Network::~Network() {
    disconnect();
    SDLNet_Quit();
}

bool Network::connect(const std::string& host, Uint16 port) {
    if (SDLNet_ResolveHost(&ip, host.c_str(), port) == -1) {
        std::cerr << "SDLNet_ResolveHost: " << SDLNet_GetError() << std::endl;
        return false;
    }
    socket = SDLNet_TCP_Open(&ip);
    if (!socket) {
        std::cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
        return false;
    }
    return true;
}

void Network::disconnect() {
    if (socket) {
        SDLNet_TCP_Close(socket);
        socket = nullptr;
    }
}

bool Network::send(const std::string& message) {
    if (socket && SDLNet_TCP_Send(socket, message.c_str(), message.length() + 1) < message.length() + 1) {
        std::cerr << "SDLNet_TCP_Send: " << SDLNet_GetError() << std::endl;
        return false;
    }
    return true;
}

std::string Network::receive() {
    char buffer[512];
    if (socket && SDLNet_TCP_Recv(socket, buffer, 512) > 0) {
        return std::string(buffer);
    }
    return "";
}
