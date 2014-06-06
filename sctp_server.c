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

#define MAX_BUFFER 1024
#define PORT_NUM 9124
#define LOCALTIME_STREAM 0
#define GMT_STREAM 1
#define NUM_PARAM 2

int main(int argc, char* argv[]){
    int listen_socket, connection_socket, ret;
    struct sockaddr_in server_address;
    char buffer[MAX_BUFFER + 1];
    time_t current_time;

    //input params
    if(argc != NUM_PARAM) {
        printf("Program args error: expected SERVER_ADDR\n");
        exit( EXIT_FAILURE );
    }
    
    //SCTP socket
    listen_socket = socket( AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if(listen_socket < 0){
        perror( "Socket errror" );
        exit( EXIT_FAILURE );
    }
    
    //initialize the server address
    memset( (void *)&server_address, 0, sizeof(server_address) );
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr( argv[1] );
    server_address.sin_port = htons( PORT_NUM );
    
    //bind
    ret = bind( listen_socket, (struct sockaddr *)&server_address, sizeof(server_address) );
    if(ret < 0){
        perror( "Bind errror" );
        exit( EXIT_FAILURE );
    }
    
    //listen
    listen( listen_socket, 10 );

    //server loop
    while( 1 ) {
        printf("Waiting for a client ...\n");
        //wait for a client
        connection_socket = accept( listen_socket, (struct sockaddr *)NULL, (int *)NULL );
        printf("New client connected ...\n");
        //grab the current time
        current_time = time(NULL);
        
        sleep(2);

        /* Send local time on stream 0 (local time stream) */
        snprintf( buffer, MAX_BUFFER, "%s\n", ctime(&current_time) );
        printf("Sending %s \n-----End Buffer-----\nOn stream: %d\n", buffer, LOCALTIME_STREAM);
        ret = sctp_sendmsg( connection_socket, 
                            (void *)buffer, (size_t)strlen(buffer) + 1, 
                            NULL, 0, 
                            0, 0, LOCALTIME_STREAM, 
                            0, 0 );
        if(ret < 0){
            perror("Sending on LOCALTIME_STREAM error\n");
            exit( EXIT_FAILURE );
        }
        printf("Sended %d bytes\n\n", ret);
        
        sleep(2);
        
        /* Send GMT on stream 1 (GMT stream) */
        snprintf( buffer, MAX_BUFFER, "%s\n", asctime( gmtime(&current_time) ) );
        //snprintf( buffer, MAX_BUFFER, "%s\n", "CIAO" );
        printf("Sending %s \n-----End Buffer-----\nOn stream: %d\n", buffer, GMT_STREAM);
        ret = sctp_sendmsg( connection_socket, 
                            (void *)buffer, (size_t)strlen(buffer) + 1, 
                            NULL, 0, 
                            0, 0, GMT_STREAM, 
                            0, 0 );
        if(ret < 0){
            perror("Sending on GMT_STREAM error\n");
            exit( EXIT_FAILURE );
        }
        printf("Sended %d bytes\n\n", ret);
        
        /* Close the client connection */
        close( connection_socket );

    }

    return 0;
}
