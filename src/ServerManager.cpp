#include <ServerManager.h>
#include <unistd.h>
#include <chrono>
#include <error.h>
#include <string.h>

ServerManager::ServerManager() : server_socket(socket(AF_INET, SOCK_STREAM, 0)), stop(new std::atomic<bool>(false)), server_address(), clients(), mtx(){
    bool err = true;

    if(server_socket < 0){
        throw std::runtime_error("Unable to retrieve socket");
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(HOST_PORT);

    try{utils::setNoBlock(server_socket);}
    catch(std::runtime_error e){
        throw std::runtime_error(e);
    }

    while(err){
        if(bind(server_socket, (const sockaddr *) &server_address, sizeof(server_address)) == 0){
            err = false;
        }
        else{
            std::cout << "Failed to bind socket, retrying in 10 seconds...\n";
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
    }

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

    if(utils::handleHTMLReq(client_socket) == 0){
        ClientManager* client = new ClientManager(client_socket, s);

        mtx.lock();
        clients.push_back(client); 
        mtx.unlock();

        client->clientMain();
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
    for(long unsigned int i = 0; i < clients.size(); i++){
        delete clients.at(i);
    }

    delete stop;

    std::cout << "clients deleted\n";
}