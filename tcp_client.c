#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

#define PORT_NUM 7345
#define NUM_PKT 10000000
#define PAYLOAD_SIZE 1472

typedef struct {
    char payload[PAYLOAD_SIZE];
} packet;


void client(const char* ip_master){
    int sck;
    struct sockaddr_in addr;
    socklen_t len;
    int opt = 1;
    packet pkt;
    int i = 0;
    
    if( (sck = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 ) {    //TCP socket
        perror("ERROR socket");
        exit( EXIT_FAILURE );
    }
    
    memset( &addr, 0, sizeof(struct sockaddr_in) );
    addr.sin_family = AF_INET;
    addr.sin_port = htons((unsigned short)(PORT_NUM));
    addr.sin_addr.s_addr = inet_addr(ip_master);
    if ( connect( sck, (struct sockaddr *) &addr, sizeof(addr) ) < 0) {
        perror("ERROR connect");
        exit( EXIT_FAILURE );
    }
    
    printf("Client connected to %s\n", ip_master);
    len = sizeof(addr);
    for(i = 0; i < NUM_PKT; i++) {
        if ( recvfrom( sck, &pkt, sizeof(pkt), 0, (struct sockaddr *) &addr, &len ) < 0) {
            perror("ERROR recv");
            exit( EXIT_FAILURE );
        }   
    }
}

int main(int argc, const char * argv[])
{
    if(argc < 2){
        printf("ERROR, usage: MASTER_IP\n");
        exit( EXIT_FAILURE );
    }
    
    client(argv[1]);
    return 0;
}
