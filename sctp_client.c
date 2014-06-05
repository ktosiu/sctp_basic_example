#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <errno.h>
#include <strings.h>
#include <string.h>
#include <time.h>

#define SERVER_ADDR "20.0.0.1"
#define PORT_NUM 9124
#define MAX_BUFFER 1024
#define LOCALTIME_STREAM 0
#define GMT_STREAM 1
#define NUM_MSG 2

int main(int argc, char *argv[]){
    int connection_socket, in, i, flags;
    struct sockaddr_in servaddr;
    struct sctp_sndrcvinfo sndrcvinfo;
    struct sctp_event_subscribe events;
    char buffer[MAX_BUFFER+1];

    //SCTP socket
    connection_socket = socket( AF_INET, SOCK_STREAM, IPPROTO_SCTP );

    //Specify the peer endpoint to which we'll connect
    bzero( (void *)&servaddr, sizeof(servaddr) );
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT_NUM);
    servaddr.sin_addr.s_addr = inet_addr( SERVER_ADDR );

    //Connect to the server
    connect( connection_socket, (struct sockaddr *)&servaddr, sizeof(servaddr) );

    //Enable receipt of SCTP Snd/Rcv Data via sctp_recvmsg
    memset( (void *)&events, 0, sizeof(events) );
    events.sctp_data_io_event = 1;
    setsockopt( connection_socket, SOL_SCTP, SCTP_EVENTS, (const void *)&events, sizeof(events) );

    //Expect two messages from the peer 
    for (i = 0 ; i < NUM_MSG ; i++) {
	//SCTP rcv
        in = sctp_recvmsg( connection_socket, (void *)buffer, sizeof(buffer), (struct sockaddr *)NULL, 0, &sndrcvinfo, &flags );

        //Null terminate the incoming string
        buffer[in] = 0;

        if (sndrcvinfo.sinfo_stream == LOCALTIME_STREAM) {
            printf("(Local) %s\n", buffer);
        } else if (sndrcvinfo.sinfo_stream == GMT_STREAM) {
            printf("(GMT  ) %s\n", buffer);
        }

    }  

    //Close our socket and exit
    close(connection_socket);

    return 0;
}
