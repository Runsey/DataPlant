#include<ctime>
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
ssize_t udp_reader_read(UdpReader* r, uint8_t* buf, size_t cap, uint64_t& mi, uint64_t& mx, uint64_t& mean, uint64_t& expected_id){
   int n= recv(r->fd, buf, cap, 0);

    if(n<0){
        cerr<<"Failed to read "<<endl;
        return -1;
    }
    uint64_t seq;
    memcpy(&seq, buf, sizeof(seq));
    uint64_t ts_send_ns;
    memcpy(&ts_send_ns, buf+8, sizeof(ts_send_ns));

    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    uint64_t ts_recv_ns= uint64_t(ts.tv_sec)*1'000'000'000ULL + uint64_t(ts.tv_nsec);
    uint64_t latency_ns=ts_recv_ns-ts_send_ns;

    mi=min(mi, latency_ns);
    mx=max(mx, latency_ns);

    count++;

    mean=(mean*(count-1)+latency_ns)/count;

    return n;

}
