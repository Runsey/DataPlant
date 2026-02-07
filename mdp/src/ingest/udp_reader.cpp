#include "udp_reader.h"


using namespace std;



bool udp_reader_init(UdpReader* r, const char* addr, int port){ //addr=ip_address, port
    r->fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(r->fd<0){
        cerr<<"Failed to creat socket"<<endl;
        return false;
    }
    addrinfo hints{}, * results_addr=nullptr;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_DGRAM;
    hints.ai_flags=AI_PASSIVE;
    
    int gai= getaddrinfo(addr, to_string(port).c_str(), &hints, &results_addr);
    if(gai!=0){
        cerr<<"Failed to get address info"<<endl;
        freeaddrinfo(results_addr);
        close(r->fd);
        return false;
    }

    if(bind(r->fd, results_addr->ai_addr, results_addr->ai_addrlen)<0){
        cerr<<"Failed to bind"<<endl;
        freeaddrinfo(results_addr);
        close(r->fd);
        return false;
    }
    freeaddrinfo(results_addr);
    cout<<"Bound and recieving"<<endl;
    return true;

}
ssize_t udp_reader_read(UdpReader* r, uint8_t* buf, size_t cap){
    return recv(r->fd, buf, cap, 0);
}
