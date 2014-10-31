#define u_int32_t uint32_t  // for server_addr.sin_addr.s_addr in BSD
#include <stdio.h>
#include <netinet/in.h>  // for BSD
#include <sys/socket.h>  // for BSD
//#include <arpa/inet.h>   // Linux 可以不用
#include <stdlib.h>
#include <string.h>
#define BUF_LEN 1024

int main()
{
    int port = 51234;
    char ip[15];

    fprintf(stdout, "Enter ip : ");
    scanf("%s", ip);
    fprintf(stdout, "Enter port : ");
    scanf("%d", &port);

    // declare
    struct sockaddr_in server_addr;    // 存socket資訊
    int server_fd;   // 存file descriptor
    socklen_t server_addr_len = sizeof(server_addr);    // 傳入 accept()
    char buffer[BUF_LEN];

    // create socket
    fprintf(stdout, "Creating socket...\n");
    server_fd = socket( AF_INET, SOCK_STREAM, 0);   // success: nonnegative, fail: -1
    if(server_fd == -1){
        fprintf(stderr, "Create socket failed.\n");
        exit(-1);
    }
    else{
        fprintf(stdout, "Create socket successed.\n");
    }

    // initialize socket
    fprintf(stdout, "Initializing socket...\n");
    bzero( &server_addr, sizeof(server_addr) );
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl( INADDR_ANY );  // Failed in BSD, can be fixed by including iostream and using g++, or define u_int32_t
    //server_addr.sin_addr.s_addr = INADDR_ANY;  // Failed in both BSD, can be fixed by including iostream and using g++, or define u_int32_t
    fprintf(stdout, "Finished!\n");

    // connect
    fprintf(stdout, "Connecting...\n");
    if( connect( server_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in)) == -1 ){
        fprintf(stderr, "Connect failed.\n");
        exit(-1);
    }
    else{
        fprintf(stdout, "Connect successed.\n");
    }
    fprintf(stdout, "Finished!\n");

    // input message
    fprintf(stdout, ">>>");
    char c;
    while ( (c = getchar()) != '\n' && c != EOF );
    //fflush(stdin);
    //while( fgets(buffer, sizeof(buffer), stdin) && strcmp(buffer,"\n")== 0);
    fgets(buffer, sizeof(buffer), stdin);
    if(buffer[strlen(buffer)-1] == '\n')
        buffer[strlen(buffer)-1] = '\0';
    //scanf("%s", buffer);

    //send
    fprintf(stdout, "Sending...\n");
    int send_len = send( server_fd, buffer, strlen(buffer), 0 );
    if(send_len == -1){
        fprintf(stderr, "Send failed.\n");
        exit(-1);
    }
    else{
        fprintf(stdout, "Send successed.\n");
        fprintf(stdout, "Send : %s.\n", buffer);
        fprintf(stdout, "Send words : %d.\n", send_len);
    }
    fprintf(stdout, "Finished!\n");

    // recv
    fprintf(stdout, "Receiving...\n");
    int recv_len = recv( server_fd, buffer, BUF_LEN-1, 0 );
    if(recv_len == -1){
        fprintf(stderr, "Receive failed.\n");
        exit(-1);
    }
    else{
        fprintf(stdout, "Receive successed.\n");
        fprintf(stdout, "Receive : %s.\n", buffer);
        fprintf(stdout, "Receive words : %d.\n", recv_len);
    }
    fprintf(stdout, "Finished!\n");

    // close
    fprintf(stdout, "Closing...\n");
    if( close(server_fd) == -1){
        fprintf(stderr, "Close failed.\n");
        exit(-1);
    }
    else{
        fprintf(stdout, "Close successed.\n");
    }
    fprintf(stdout, "Finished!\n");

    return 0;
}
