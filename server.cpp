#include "ServerSide.hpp"
#include <string>

int main()
{
    ServerSide server(std::string("/tmp/fileServer1.socet"));
    while(1){
    server.read();
    }
    return 0;
}

