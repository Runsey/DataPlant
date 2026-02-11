#include<unistd.h>
#include<iostream>
#include<fcntl.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <chrono>
#include "udp_reader.h"
#include <climits>

using namespace std;



int main(int argc, char** argv){
    UdpReader r;
    const char* addr=  argv[1];
    const char * port_str=argv[2];
    int port=atoi(port_str);
    if(!udp_reader_init(&r, addr, port)){
        return 1;
    }

    uint64_t total_pkts  = 0;
    uint64_t total_bytes = 0;

    uint64_t win_pkts  = 0;
    uint64_t win_bytes = 0;
    auto last= chrono::steady_clock::now();

    uint8_t buf[2048];

    uint64_t min_latency=ULLONG_MAX, max_latency=0, mean=0, count=0;

    while(true){
        ssize_t n=udp_reader_read(&r, buf, sizeof(buf), min_latency, max_latency, mean, count);
        
        win_pkts++;
        win_bytes+=n;
        auto now=chrono::steady_clock::now();
        if(now-last>=1s){
            total_pkts+=win_pkts;
            total_bytes+=win_bytes;
            cout << "pkts/s=" << win_pkts
             << " bytes/s=" << win_bytes
             << " total_pkts=" << total_pkts
             << " total_bytes=" << total_bytes
             << " min_latency_ns=" <<min_latency
             << " max_latency_ns=" <<max_latency
             << " mean_latency_ns=" <<mean
             << "\n";



             win_pkts=0;
             win_bytes=0;

             last+=1s;

        }

    }
    

}