#include "ClientManager.h"
#include "BetterSockets.h"
#include "Html.h"


ClientManager::ClientManager(int socketFD, std::atomic<bool>* stop) : socketFD(socketFD), client_address(), stop(stop){
    struct sockaddr_in address = {0};
    socklen_t addressLength = sizeof(address);

    if(getpeername(socketFD , (struct sockaddr*) &address , &addressLength ) < 0){
        throw std::runtime_error("Could not get clients information");
    }

    bsock::setNoBlock(socketFD);

    client_address.ip = inet_ntoa(address.sin_addr);
    client_address.port = ntohs(address.sin_port);
}

void ClientManager::clientMain(){
    utils::Timer elapsedTime;

    while((!stop->load(std::memory_order_acquire)) && elapsedTime.getElapsedTimeSeconds() < 360){
        if(html::handleHTMLReq(socketFD) == 0){
            elapsedTime.reset();
        }
    }

    std::cout << "stopping" << std::endl;
}

int ClientManager::getSocketFD() const{
    return socketFD;
}

ClientAddress ClientManager::getIPAddress() const {
    return client_address;
}

ClientManager::~ClientManager(){
    std::cout << "Closing connection with client " << socketFD <<"\n";
    close(socketFD);
}