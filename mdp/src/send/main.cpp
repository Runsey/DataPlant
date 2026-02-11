#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <time.h>

using namespace std;

static uint64_t now_ns_raw(){
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return uint64_t(ts.tv_sec)*1'000'000'000ULL +uint64_t(ts.tv_nsec);
}

void auto_fill(vector<uint8_t>& buff, int amount, uint64_t packet_id){
    if (buff.size()<16) return;

    memcpy(buff.data(), &packet_id, sizeof(packet_id));

    uint64_t ts_send_ns=now_ns_raw();
    memcpy(buff.data()+8,&ts_send_ns, sizeof(ts_send_ns));
    
}

int main(int argc, char** argv){
    string host = "127.0.0.1";
    string port = "3000";
    int size = 256 ;
    int count = 0; //infinte
    int pps=0; //maxspeed
    int seed= 1;

    for(int i=1;i<argc;i++){
        if(strcmp(argv[i],"--host")==0){
            host=argv[++i];
        }
        else if(strcmp(argv[i],"--port")==0){
            port=argv[++i];
        }
        else if(strcmp(argv[i],"--size")==0){
            size=atoi(argv[++i]);
        }
        else if(strcmp(argv[i],"--count")==0){
            count=atoi(argv[++i]);
        }
        else if(strcmp(argv[i],"--pps")==0){
            pps=atoi(argv[++i]);
        }
        else if(strcmp(argv[i],"--seed")==0){
            seed=atoi(argv[++i]);
        }
    }

    int sockfd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    vector<uint8_t> send_buf(size,0);

    addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_DGRAM;
    hints.ai_flags=AI_PASSIVE;

    int rc=getaddrinfo(host.c_str(), port.c_str() ,&hints, &res);
    if(rc!=0){
        cerr<<"getaddrinfo failed"<<gai_strerror(rc);
        return 1;
    }

    uint64_t packets_send=0;
    auto last= chrono::steady_clock::now();
    cout<<"Started sending.."<<endl;
    while(!count||packets_send< count){
        if(pps==0){
            auto_fill(send_buf,size,packets_send);
            sendto(sockfd, send_buf.data(), size, 0, res->ai_addr, res->ai_addrlen);

            packets_send++;
        }
        else{
            for(int i=0;i<pps;i++){
                auto_fill(send_buf,size,packets_send);
                sendto(sockfd, send_buf.data(), size, 0, res->ai_addr, res->ai_addrlen);
                packets_send++;
            }

            auto now=chrono::steady_clock::now();
            auto elapsed= now-last;
            auto one_sec=1s;

            if(one_sec>elapsed){
                this_thread::sleep_for(1s-(elapsed));
            }
            last=chrono::steady_clock::now();
        }
       // cout<<"Packets succesfully sent"<<endl;
        
    }





    


    return 0;
}