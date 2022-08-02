#include <sys/types.h>
#include <sys/socket.h>

#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>

class ServerSide
{
public:
    ServerSide(const std::string& socket_name);
    ~ServerSide();
    void new_request();
    bool read();
private:
    std::string dir;
    int sock, listener;
    struct sockaddr_un addr;
    char buf[1200];
    int bytes_read;
};
ServerSide::ServerSide(const std::string& socket_name)
{
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
void ServerSide::new_request(){
    std::cout << "Waiting for the client to connect..." << std::endl;
    sock = accept(listener, NULL, NULL);
    while(sock < 0)
    {
        std::cout << "This is fail 1!" << std::endl;
        sock = accept(listener, NULL, NULL);
    }
}
bool ServerSide::read(){
    memset(buf, 0, 1200);
    bytes_read = recv(sock, buf, 1199, 0);
    while(bytes_read <= 0) {
        memset(buf, 0, 1200);
    
        new_request();    
        bytes_read = recv(sock, buf, 1199, 0);
    }
    std::cout << buf << std::endl;
    return true;
}
ServerSide::~ServerSide(){
    close(sock);
    remove(dir.c_str());
}

int main()
{
    ServerSide server(std::string("/tmp/fileServer1.socet"));
    server.new_request();
    while(1){
    server.read();
    }
    return 0;
}
