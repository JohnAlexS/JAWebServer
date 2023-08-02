#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <thread>
#include <unistd.h>
#include <iostream>
#include <future>
#include <Utils.h>

enum UserPrivilege{
    admin,
    standard_user,
    guest_user
};

struct ClientAddress{
    std::string ip = "0.0.0.0";
    int port = -1;
};

class ClientManager{
    private:
        UserPrivilege privilege;
        ClientAddress client_address;
        int socketFD;
        int c = 0;
        std::atomic<bool>* stop;


    public:
        ClientManager(int socketFD, std::atomic<bool>* stop);
        void clientMain();
        int getSocketFD() const;
        ClientAddress getIPAddress() const;
        ~ClientManager();
};


