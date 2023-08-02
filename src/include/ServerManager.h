#pragma once
#include <vector>
#include <ClientManager.h>
#include <assert.h>
#include <future>

#define MAX_CLIENTS 5
#define HOST_PORT 8080

class ServerManager{
    public:
        ServerManager();
        void client_listener();
        void shutdown();
        ~ServerManager();

    private:
        void clientHandler(int const &client_socket, std::atomic<bool>* s);
        int server_socket;
        struct sockaddr_in server_address;
        std::vector<ClientManager*> clients;
        std::atomic<bool>* stop;
        std::mutex mtx;
};