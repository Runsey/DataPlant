#include "udp_reader.h"
#include<unistd.h>
#include<iostream>
#include<fcntl.h>
#include<sys/socket.h>


using namespace std;

bool udp_reader_init(UdpReader* r, const char* addr, int port){
    r->fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(r->fd<0){
        cerr<<"Failed to creat socket"<<endl;
        return false;
    }
    sockaddr_in hints{}, *results_addr;
    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_DGRAM;
    hints.ai_flags=AI_PASSIVE;
    getaddrinfo(addr, to_string(port).c_str(), &hints, &results_addr);

    if(bind)
}
ssize_t udp_reader_read(UdpReader* r, uint8_t* buf, size_t cap);
