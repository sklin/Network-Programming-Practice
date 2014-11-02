#define u_int32_t uint32_t
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define BUF_LEN 1024
#define CONNECTION_LIMIT 10

int main(int argc, char* argv[])
{
    // Setting
    int port = 54321;   //default
    if(argc>1){
        port = atoi(argv[1]);
    }
    else{
        printf("Enter which port to bind : ");
        scanf("%d", &port);
    }

    // Declare
    struct sockaddr_in server_addr, client_addr;   // socket information
    int server_fd, client_fd;   // file descriptor
    socklen_t client_addr_len = sizeof(client_addr);    // 傳入accept()
    char buffer[BUF_LEN];

    // Create socket
    server_fd = socket( AF_INET, SOCK_STREAM, 0);
    if(server_fd == -1){
        fprintf( stderr, "Create socket fail.\n");
        exit(-1);
    }

    // Initailize socket
    bzero( &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind
    if( bind( server_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in)) == -1){
        fprintf( stderr, "Bind fail.\n");
        exit(-1);
    }

    // Listen
    if( listen( server_fd, CONNECTION_LIMIT) == -1 ){
        fprintf( stderr, "Listen fail.\n");
        exit(-1);
    }

    while(1){
        // Accept
        client_addr_len = sizeof(client_addr);
        client_fd = accept( server_fd, (struct sockaddr*)&server_addr, (socklen_t*)&client_addr_len);
        if(client_fd == -1){
            fprintf( stderr, "Accept fail.\n");
            exit(-1);
        }

        while(1){
            // Receive
            memset(buffer, 0, sizeof(buffer));
            int recv_len = recv( client_fd, buffer, BUF_LEN-1, 0);
            //if(recv_len == -1){
            if(recv_len <= 0){
                //fprintf( stderr, "Receive fail.\n");
                //exit(-1);
                break;
            }
            else{
                printf("Receive: '%s'\n", buffer);
                printf("Size: %d\n", recv_len);
            }

            // Send
            int send_len = send( client_fd, buffer, strlen(buffer), 0);
            if(send_len == -1){
                fprintf( stderr, "Send fail.\n");
                exit(-1);
            }
            else{
                printf("Send: '%s'\n", buffer);
                printf("Size: %d\n", send_len);
            }
            memset(buffer, 0, sizeof(buffer));
        }

        // Close
        if(close(client_fd) == -1){
            fprintf( stderr, "Close fail.\n");
            exit(-1);
        }
    }

    return 0;
}
