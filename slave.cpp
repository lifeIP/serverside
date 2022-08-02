#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <thread>
#include <mutex>
#include <chrono>

#include <string>
#include <stdio.h>
#include <iostream>

#include <unistd.h>


class KlientSide
{
    int sock;
    std::mutex mutex;
    char common_bufer[130];
    struct sockaddr_un addr;
public:

    KlientSide(const std::string& socket_name){
        sock = socket(AF_UNIX, SOCK_STREAM, 0);
        if(sock < 0)
        {
            perror("socket");
            _exit(1);
        }
        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path, socket_name.c_str(), sizeof(addr.sun_path) - 1);
        
        std::cout << "Waiting for the server to connect..." << std::endl;
        while(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0);
        std::cout << "The connection is established." << std::endl;
    }

    ~KlientSide(){
        close(sock);
    }

void secondThread(){
    while(1){
        mutex.lock();
        const char sd = '\n';
        strcat(common_bufer, &sd);
        std::cout << "thread was locked 2" << std::endl;
        //std::cout << common_bufer << std::endl;
        char buf[130];
        memset(buf, 0, 129);
        send(sock, common_bufer, sizeof(common_bufer), 0);
        std::cout << common_bufer << std::endl;
        //recv(sock, buf, sizeof(common_bufer), MSG_PEEK);
        mutex.unlock();
        std::cout << "thread was ulocked 2" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void firstThread(){
    while(1){
        mutex.lock();
        std::cout << "thread was locked 1" << std::endl;
        dataEntering();
        for(int i = 0; is_invalid(); i++){
            if(i != 0){    
                std::cout << "You have entered data that is not digits. Try again." << std::endl;
                dataEntering();
            }
        }
        bubbleSort();
        char temp[129] = {};
        for(int i = 0; i < strlen(common_bufer); i++){
            const char a = common_bufer[i];
            if(a=='0'||a=='2'||a=='4'||a=='6'||a=='8'){
                strcat(temp, "KB");
            }
            else{
                strcat(temp, &a);
            }
        }
        strcpy(common_bufer, temp);
        mutex.unlock();
        std::cout << "thread was unlocked 1" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
private:
    bool is_invalid(){
        bool invalid = false;
        if(strlen(common_bufer)<=64){
            for(int i = 0; i < strlen(common_bufer); i++){
                if(common_bufer[i] < '0' || common_bufer[i] > '9'){
                    return true;
                }
            }
            return false;
        }
        return true;
    }

    void bubbleSort()
    {
        char tmp = 0;
        for(int i = 0;i<strlen(common_bufer);i++){
            for(int j = (strlen(common_bufer)-1);j>=(i+1);j--){
                if(common_bufer[j]>common_bufer[j-1]){
                    tmp = common_bufer[j];
                    common_bufer[j]=common_bufer[j-1];
                    common_bufer[j-1]=tmp;
                }
            }
        }
    }

    void dataEntering(){
        std::cout << "Enter the numeric data. (Numbers only!)" << std::endl;
        std::cin >> common_bufer;
    }

};


int main()
{
    KlientSide klient(std::string("/tmp/fileServer1.socet"));
    std::thread first(&KlientSide::firstThread, std::ref(klient));
    std::thread second(&KlientSide::secondThread, std::ref(klient));
    first.join();
    second.join();
    return 0;
}
