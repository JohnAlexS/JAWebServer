#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <thread>
#include <unistd.h>
#include <iostream>
#include <future>
#include <Utils.h>

struct ClientAddress{
    std::string ip = "0.0.0.0";
    int port = -1;
};

class ClientManager{
    private:
        int socketFD;
        ClientAddress client_address;
        std::atomic<bool>* stop;


    public:
        ClientManager(int socketFD, std::atomic<bool>* stop);
        void clientMain();
        int getSocketFD() const;
        ClientAddress getIPAddress() const;
        ~ClientManager();
};


