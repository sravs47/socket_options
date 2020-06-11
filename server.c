/*
    C socket server example
*/

#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write


union val {
    int i_val;
    long l_val;
    struct linger     linger_val;
    struct timeval    timeval_val;
 } val;

int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , read_size, on=1, sock_option,resp,connfd;
    struct sockaddr_in server , client;
        socklen_t len = sizeof(resp);


    char client_message[1000];
    // int client_message;


    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8880 );


    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc , 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");

    // Receive a message from client
    while( (read_size = recv(client_sock , client_message , sizeof(client_message)-1,0)) > 0 )
    {

		// execvp(client_message,NULL);
        // client_message[read_size]='\0';
        puts(client_message);
        // printf("%s\n", client_message);
		// printf("Enter Your Reply : ");
		// fgets(client_message, sizeof(client_message),stdin);
		//Send the message back to client
        // write(client_sock , "Received" , strlen(client_message));
		// printf("\nwritten in server");
        // fflush(stdout);
		execvp(client_message,NULL);
		client_message[0]='\0';

    }




    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }

    return 0;
}
