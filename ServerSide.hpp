#pragma once


#include <string>
#include <sys/un.h>

class ServerSide
{
public:
    ServerSide(const std::string& socket_name);
    ~ServerSide();
    void set_socket_name(const std::string& socket_name);
    void read();
private:
    void load_socket();
    void new_request();

    std::string socket_name;
    std::string dir;
    int sock, listener;
    struct sockaddr_un addr;
    char buf[200];
    int bytes_read;
};

