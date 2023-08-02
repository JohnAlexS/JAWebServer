#include "BetterSockets.h"

using namespace bsock;

void bsock::setNoBlock(int& socketFD){
    int flags = fcntl(socketFD, F_GETFL, 0);

    if(flags < 0){
        throw std::runtime_error("Unable to retrieve socket flags");
    }

    if(fcntl(socketFD, F_SETFL, flags | O_NONBLOCK) < 0){
        throw std::runtime_error("Unable to set socket flags");
    }
}

std::string bsock::readSocket(int const &socketFD, int size){
    char* buf = new char[size];

    ssize_t buf_read = read(socketFD, buf, size);
    
    if(buf_read < 0){
        if(!(errno == EAGAIN || errno == EWOULDBLOCK)){
            delete[] buf;
            throw std::runtime_error("Failed to read buffer");
        }
        throw errno;
    }

    std::string data(buf, buf+buf_read);

    delete[] buf;
    return data;
}

void bsock::sendToSocket(int const &socketFD, std::string& str){
    if(send(socketFD, str.c_str(), str.length(), MSG_NOSIGNAL) < 0){
        throw std::runtime_error("Failed to send to client");
    }
}

void bsock::persistBind(int server_socket, sockaddr_in& server_address){
    bool err = true;

    while(err){
        if(bind(server_socket, (const sockaddr *) &server_address, sizeof(server_address)) == 0){
            err = false;
        }
        else{
            std::cout << "Failed to bind socket, retrying in 10 seconds...\n";
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }   
    }
}