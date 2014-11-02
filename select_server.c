#define u_int32_t uint32_t
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>

#define BUF_LEN 1024
//#define FD_SETSIZE 256  // new for select()
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

    // Used for select()
    int maxfd = -1, nready;
    int client[FD_SETSIZE], client_size = -1;   // store client_fd and its ammount
    fd_set rset, allset;

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

    // Setting select()
    maxfd = server_fd;  // a.k.a. listen_fd
    client_size = -1;   // client socket @ client[] 的最高index
    int i;
    for( i=0 ; i < FD_SETSIZE ; i++ )
        client[i] = -1; // initialize client[], -1 indicate available entry
    FD_ZERO( &allset);  // initailize allset
    FD_SET( server_fd, &allset);    // set server_fd (listen_fd)

    while(1){
        rset = allset;
        nready = select( maxfd+1, &rset, NULL, NULL,NULL);
        if(FD_ISSET( server_fd, &rset)){
            // Accept
            client_addr_len = sizeof(client_addr);
            client_fd = accept( server_fd, (struct sockaddr*)&server_addr, (socklen_t*)&client_addr_len);
            if(client_fd == -1){
                fprintf( stderr, "Accept fail.\n");
                exit(-1);
            }

            printf( "new client: %s, port %d\n",
                    inet_ntop(AF_INET, &client_addr.sin_addr, 4, NULL),
                    ntohs(client_addr.sin_port) );

            // store client_fd into client[]
            for( i=0 ; i < FD_SETSIZE ; i++ ){
                if(client[i] < 0){
                    client[i] = client_fd;
                    printf("client_fd %d is stored in client[%d]\n", client_fd, i);
                    break;
                }
            }
            if(i==FD_SETSIZE){  // run out of space
                fprintf( stderr, "Too many clients.\n");
                exit(-1);
            }
            FD_SET( client_fd, &allset);    //set client_fd
            if(client_fd>maxfd) // Update maxfd
                maxfd = client_fd;
            if(i > client_size) // Update client_size
                client_size = i;
            printf("client_size : %d\n", client_size);
            if(--nready <= 0)   // after adding new client_fd to client[], no more readable descriptor,
                continue;       // continue the while loop, skip the following instruction
        }

        int sock_fd;
        for( i=0 ; i <= client_size ; i++ ){  // check all clients for data
            if( (sock_fd = client[i]) < 0 )  // -1, means no connection
                continue;
            if( FD_ISSET( sock_fd, &rset) ){

                // Receive
                memset(buffer, 0, sizeof(buffer));
                int recv_len = recv( sock_fd, buffer, BUF_LEN-1, 0);
                if(recv_len < 0){
                    fprintf( stderr, "Receive fail.\n");
                    exit(-1);
                }
                else if(recv_len == 0){
                    // connection closed by client
                    close(sock_fd);
                    FD_CLR( sock_fd, &allset);  // Update allset
                    client[i] = -1;
                    printf("close the client %d\n", sock_fd);
                }
                else{
                    printf("Receive: '%s'\n", buffer);
                    printf("Size: %d\n", recv_len);

                    // Send
                    int send_len = send( sock_fd, buffer, strlen(buffer), 0);
                    if(send_len == -1){
                        fprintf( stderr, "Send fail.\n");
                        exit(-1);
                    }
                    else{
                        printf("Send: '%s'\n", buffer);
                        printf("Size: %d\n", send_len);
                        memset(buffer, 0, sizeof(buffer));
                    }
                }

                if(--nready <= 0)   // no more readable descriptor
                    break;  // break out of for loop
            }
        }
    }

    return 0;
}
