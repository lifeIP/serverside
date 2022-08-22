#include "ServerSide.hpp"
#include <sys/types.h>
#include <sys/socket.h>

#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>

void ServerSide::set_socket_name(const std::string& socket_name){
    this->socket_name = socket_name;
}
void ServerSide::load_socket(){
    remove(socket_name.c_str());
    listener = socket(AF_UNIX, SOCK_STREAM, 0);
    if(listener < 0)
    {
        perror("socket");
        _exit(1);
    }
    
    addr.sun_family = AF_UNIX;
    dir = socket_name;
    strncpy(addr.sun_path, dir.c_str(), sizeof(addr.sun_path) - 1);
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {   
        perror("bind");
        _exit(2);
    }
    listen(listener, 1);
}

ServerSide::ServerSide(const std::string& socket_name)
{
    this->socket_name = socket_name;
    load_socket();
    new_request();
    
}
ServerSide::~ServerSide(){
    close(sock);
}
void ServerSide::new_request(){
    std::cout << "Waiting for the client to connect..." << std::endl;
    sock = accept(listener, NULL, NULL);
    while(sock < 0)
    {
        sock = accept(listener, NULL, NULL);

    }
    std::cout << "The connection is established!" << std::endl;    
}
void ServerSide::read(){
    memset(buf, 0, 200);
    bytes_read = recv(sock, buf, 199, 0);
    while(bytes_read <= 0) {
        memset(buf, 0, 200);
        new_request();    
        bytes_read = recv(sock, buf, 199, 0);
    }
    if(strlen(buf) > 2 && atoi(buf) % 32 == 0){
        std::cout << "\nData received: \tMessage summ: " << buf << std::endl;    
    }
    else{   
        std::cout << "\nMessage ERROR:\tThe data does not meet the requirements.\n" << std::endl;
    }
}
