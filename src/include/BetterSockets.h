#pragma once

#include <fcntl.h>
#include <stdexcept>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <chrono>
#include <thread>
#include <iostream>

#define SEND_RATE 100000

namespace bsock{
    void setNoBlock(int& socketFD);
    void sendToSocket(int const &socketFD, std::string str);
    void persistBind(int server_socket, sockaddr_in& server_address);
    std::string readSocket(int const& socketFD, int size);
}
