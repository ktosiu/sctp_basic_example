#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#define PORT_NUM 7345
#define NUM_PKT 1
#define PAYLOAD_SIZE 1400

typedef struct {
    char payload[PAYLOAD_SIZE];
} packet;


void server(const char* ip_master){
    int listen_sck, connect_sck;
    struct sockaddr_in addr;
    socklen_t len;
    int opt = 1;
    packet pkt;
    int i = 0;
    
    if( (listen_sck = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 ) {    //TCP socket
        perror("ERROR socket");
        exit( EXIT_FAILURE );
    }
    
    memset( &addr, 0, sizeof(struct sockaddr_in) );
    addr.sin_family = AF_INET;
    addr.sin_port = htons((unsigned short)(PORT_NUM));
    addr.sin_addr.s_addr = inet_addr(ip_master);
    
    setsockopt( listen_sck, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt) );
    if (bind( listen_sck, (struct sockaddr *) &addr, sizeof(addr) ) < 0) {
        perror("ERRIR bind");
        exit( EXIT_FAILURE );
    }
    listen(listen_sck, 10);
    
    printf("Master started at %s : %d - Waiting for a client...\n", ip_master, PORT_NUM);
    connect_sck = accept(listen_sck, (struct sockaddr *) &addr, &len);
    
    for(i = 0; i < NUM_PKT; i++) {
        if (sendto(connect_sck, &pkt, sizeof(pkt), 0, (struct sockaddr *) &addr, sizeof(addr)) <0 ) {
            perror("ERROR send");
            exit( EXIT_FAILURE );
         }
         sleep(1);
    }
}

int main(int argc, const char * argv[])
{
    if(argc < 2){
        printf("ERROR, usage: MASTER_IP\n");
        exit( EXIT_FAILURE );
    }
    
    server(argv[1]);
}
