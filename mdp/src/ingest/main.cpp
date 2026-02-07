#include<unistd.h>
#include<iostream>
#include<fcntl.h>
#include<socket.h>
#include<netinet/in.h>

using namespace std;

int main(int argc, char* argv){
    int sockfd =socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd<0){
        std::cerr
    }
}