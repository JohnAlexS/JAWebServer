#include <ClientManager.h>
#include <Utils.h>


ClientManager::ClientManager(int socketFD, std::atomic<bool>* stop) : socketFD(socketFD), privilege(standard_user), client_address(), stop(stop){
    struct sockaddr_in address = {0};
    socklen_t addressLength = sizeof(address);
    std::string ip;
    int port;

    if(getpeername(socketFD , (struct sockaddr*) &address , &addressLength ) < 0){
        throw std::runtime_error("Could not get clients information");
    }

    utils::setNoBlock(socketFD);

    ip = inet_ntoa(address.sin_addr);       
    port = ntohs(address.sin_port);

    client_address.ip = inet_ntoa(address.sin_addr);
    client_address.port = ntohs(address.sin_port);
}

void ClientManager::clientMain(){
    while(!stop->load(std::memory_order_acquire)){
        utils::handleHTMLReq(socketFD);
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