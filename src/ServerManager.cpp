#include "ServerManager.h"
#include "Html.h"
#include "BetterSockets.h"

ServerManager::ServerManager() : server_socket(socket(AF_INET, SOCK_STREAM, 0)), stop(new std::atomic<bool>(false)), server_address(), clients(), mtx(){
    if(server_socket < 0){
        throw std::runtime_error("Unable to retrieve socket");
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(HOST_PORT);

    try{
        bsock::setNoBlock(server_socket);
    }
    catch(std::runtime_error& e){
        throw std::runtime_error(e);
    }

    bsock::persistBind(server_socket, server_address);

    if(listen(server_socket, MAX_CLIENTS) < 0){
        throw std::runtime_error("Unable to begin listening on socket");
    }
}

void ServerManager::client_listener(){
    while(!stop->load(std::memory_order_acquire)){
        struct sockaddr_in client_address;
        socklen_t addrlen = sizeof(client_address);

        int client_socket = accept(server_socket, (sockaddr *) &server_address, &addrlen);

        if(client_socket < 0) {
            if(!(errno == EAGAIN || errno == EWOULDBLOCK)){
                std::cout << "Could not accept connection: " << errno << std::endl;
            }
        }

        else{
            std::thread t([&, client_socket](){
                clientHandler(client_socket, stop);
            });

            t.detach();
        }
    }
}

void ServerManager::clientHandler(int const &client_socket, std::atomic<bool>* s){
    struct sockaddr_in address = {0};
    socklen_t addressLength = sizeof(address);

    if(getpeername(client_socket , (struct sockaddr*) &address , &addressLength ) < 0){
        throw std::runtime_error("Could not get clients information");
    }

    std::cout << "Connection from: " << inet_ntoa(address.sin_addr) << " on port: " << ntohs(address.sin_port) << std::endl;


    if(html::handleHTMLReq(client_socket) == 0){
        ClientManager* client = new ClientManager(client_socket, s);

        mtx.lock();
        clients.push_back(client); 
        mtx.unlock();

        client->clientMain();

        mtx.lock();
        clients.erase(std::remove(clients.begin(), clients.end(), client));
        mtx.unlock();

        delete client;
    }

    else {
        close(client_socket);
    }
}

void ServerManager::shutdown(){
    stop->store(true, std::memory_order_release);
    close(server_socket);
}

ServerManager::~ServerManager(){
    delete stop;

    std::cout << "clients deleted\n";
}