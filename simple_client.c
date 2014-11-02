#define u_int32_t uint32_t
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#define BUF_LEN 1024

int main(int argc, char* argv[])
{
    // Setting
    int port = 54321;
    char ip[15];
    if(argc>2){
        strcpy( ip, argv[1]);
        port = atoi(argv[2]);
    }
    else{
        printf("Enter ip : ");
        scanf("%s", ip);
        printf("Enter port : ");
        scanf("%d", &port);

        char c; // tmp
        while( (c=getchar()) != '\n' && c != EOF ); // flush stdin
    }

    // Declare
    struct sockaddr_in server_addr;
    int server_fd;
    char buffer[BUF_LEN];

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd == -1){
        fprintf( stderr, "Create socket fail.\n");
        exit(-1);
    }

    // Initialize socket
    bzero( &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Connect
    if( connect( server_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in)) == -1 ){
        fprintf( stderr, "Connect fail.\n");
        exit(-1);
    }

    while(1){
        // Input
        // char c; // tmp
        //while( (c=getchar()) != '\n' && c != EOF ); // flush stdin
        memset(buffer, 0, sizeof(buffer));
        printf(">> ");
        fgets(buffer, sizeof(buffer), stdin);
        if(buffer[strlen(buffer)-1] == '\n')
            buffer[strlen(buffer)-1] = '\0';
        if(strlen(buffer) == 0)
            continue;

        // Send
        int send_len = send( server_fd, buffer, strlen(buffer), 0);
        if(send_len == -1){
            fprintf( stderr, "Send fail.\n");
            exit(-1);
        }
        else{
            printf("Send: '%s'\n", buffer);
            printf("Size: %d\n", send_len);
        }
        memset(buffer, 0, sizeof(buffer));

        // Receive
        memset(buffer, 0, sizeof(buffer));
        int recv_len = recv( server_fd, buffer, BUF_LEN-1, 0);
        if(recv_len == -1){
            fprintf( stderr, "Receive fail.\n");
            exit(-1);
        }
        else{
            printf("Receive: '%s'\n", buffer);
            printf("Size: %d\n", recv_len);
        }

    }

    // Close
    if(close(server_fd) == -1){
        fprintf( stderr, "Close fail.\n");
        exit(-1);
    }

    return 0;
}
