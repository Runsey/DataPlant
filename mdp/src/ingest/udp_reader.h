struct UdpReader {
    int fd;
};

bool udp_reader_init(UdpReader* r, const char* addr, int port);
ssize_t udp_reader_read(UdpReader* r, uint8_t* buf, size_t cap);
