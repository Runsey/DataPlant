#include<unistd.h>
#include<iostream>
#include<fcntl.h>
#include<sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include<cstring>

struct UdpReader {
    int fd;
};

struct MdpMessage{
    uint64_t seq;
    uint64_t ts_send_ns;
    uint64_t* payload;
    uint64_t len_payload;
}
bool udp_reader_init(UdpReader* r, const char* addr, int port);
ssize_t udp_reader_read(UdpReader* r, uint8_t* buf, size_t cap, uint64_t& mi, uint64_t& mx, uint64_t& mean, uint64_t& count);

bool parse_mdp_message(const uint8_t* buf, size_t len, MdpMessage& msg){
    if(len<16) return false;
    memcpy(, )
}
