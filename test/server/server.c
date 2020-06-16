/*
	C socket server example
*/

#include <sys/stat.h>
#include<stdio.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write
#include <stdlib.h>
#include <sys/types.h>

int main(int argc , char *argv[]) {

    int fd;
    int status;
    unsigned char *file_content_to_send;
    size_t read_count;
    struct stat stat_buf;
    

    int socket_desc , client_sock , c , read_size;
	struct sockaddr_in server , client;
	char client_message[2000];

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
	server.sin_port = htons( 8888 );
	
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
	


    shbf_rcv_global_init();
    fd = shbf_rcv_create("/run/audio_play");

    status = shbf_rcv_start();
    if (status < 0) {
        perror("shbf_rcv_start");
    } else {
        //Receive a message from client
        while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
        {
            printf("new Client message\n");
            shbf_rcv_fd(fd);
            file_content_to_send = (unsigned char *)malloc(sizeof(client_message));
            if (file_content_to_send == (unsigned char *)0x0) {
            printf("[audio_player] malloc error");
            }
            read_count =  1872; //sizeof(client_message);
            printf("sending size of %i \n", read_count);
            if (0 < (int)read_count) {
                shbf_set_size(fd, read_count);
                shbf_rcv_send_message(fd,client_message,read_count);
                // sleep(2);
                puts("send over");
                free(file_content_to_send);
            }
        }
    }

    
	if(read_size == 0) {
		puts("Client disconnected");
		fflush(stdout);
	}
	else if(read_size == -1)
	{
		perror("recv failed");
	}
	
	return 0;
}