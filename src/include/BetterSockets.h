#pragma once

#include <fcntl.h>
#include <stdexcept>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <chrono>
#include <thread>
#include <iostream>

namespace bsock{
    void setNoBlock(int& socketFD);
    void sendToSocket(int const &socketFD, std::string& str);
    void persistBind(int server_socket, sockaddr_in& server_address);
    std::string readSocket(int const& socketFD, int size);
}
