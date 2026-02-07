#include<unistd.h>
#include<iostream>
#include<fcntl.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <chrono>
#include "udp_reader.h"

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

    while(true){
        ssize_t n=udp_reader_read(&r, buf, sizeof(buf));

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
             << "\n";

             win_pkts=0;
             win_bytes=0;

             last+=1s;

        }

    }
    

}