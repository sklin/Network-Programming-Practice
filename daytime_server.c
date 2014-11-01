/*  Echo Server
 *  Client send 完一句話後就與Server結束連線，
 *  Server 會繼續Listen，等待下一個Client的連線。
 */
#define u_int32_t uint32_t  // for server_addr.sin_addr.s_addr in BSD
#include <stdio.h>
#include <netinet/in.h>  // for BSD
#include <sys/socket.h>  // for BSD
//#include <arpa/inet.h>   // Linux 可以不用
#include <stdlib.h>
#include <string.h>
//#include <iostream> // for BSD 
#include <time.h>
#define BUF_LEN 1024
#define CONNECTION_LIMIT 10

int main()
{
    
    int port = 51234;   //default
    fprintf(stdout, "Enter which port to bind : ");
    scanf("%d", &port);

    // declare
    struct sockaddr_in server_addr, client_addr;    // 存socket資訊
    int server_fd, client_fd;   // 存file descriptor
    socklen_t client_addr_len = sizeof(client_addr);    // 傳入 accept()
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

    // bind
    fprintf(stdout, "Binding...\n");
    if( bind(server_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in) ) == -1 ){   // success: 0, fail: -1
        fprintf(stderr, "Bind failed.\n");
        exit(-1);
    }
    else{
        fprintf(stdout, "Bind successed.\n");
    }
    fprintf(stdout, "Finished!\n");

    // listen
    fprintf(stdout, "Listening...\n");
    if( listen( server_fd, CONNECTION_LIMIT) == -1 ){   // success: 0, fail: -1
        fprintf(stderr, "Listen failed.\n");
        exit(-1);
    }
    else{
        fprintf(stdout, "Listen successed.\n");
    }
    fprintf(stdout, "Finished!\n");

    for(;;){
        // accept
        fprintf(stdout, "Accepting...\n");
        client_fd = accept( server_fd, (struct sockaddr*)&client_addr, (socklen_t*)&client_addr_len);   // success: nonnegative, fail: -1
        if( client_fd == -1 ){
            fprintf(stderr, "Accept failed.\n");
            exit(-1);
        }
        else{
            fprintf(stdout, "Accept successed.\n");
        }
        fprintf(stdout, "Finished!\n");

        // get the current time and store into buffer
        time_t ticks;
        ticks = time(NULL);
        sprintf( buffer, "%.24s\r\n", ctime(&ticks));
        //sprintf( buffer, sizeof(buffer), "%.24s\r\n", ctime(&ticks));

        //send
        fprintf(stdout, "Sending...\n");
        int send_len = send( client_fd, buffer, strlen(buffer), 0 );
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

        // close
        fprintf(stdout, "Closing...\n");
        if( close(client_fd) == -1){
            fprintf(stderr, "Close failed.\n");
            exit(-1);
        }
        else{
            fprintf(stdout, "Close successed.\n");
        }
        fprintf(stdout, "Finished!\n");
    }
    return 0;
}
