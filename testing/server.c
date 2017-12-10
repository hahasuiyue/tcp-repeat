//Partially stolen from http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define SYS_send_repeat 333

void do_server(int sockfd){
	
	send(sockfd,"Hello",strlen("Hello"),0);
	syscall(SYS_send_repeat, sockfd,"Five times",strlen("Five times"),0, 5);
	send(sockfd,"Hello",strlen("Hello"),0);
	
}

int main(int argc, char** argv){
	int sockfd, newsockfd, port, clilen;
	struct sockaddr_in serv_addr, cli_addr;

	if(argc != 2){
		printf("Proper usage is: %s [port]\n",argv[0]);
		return -1;
	}

	sockfd = socket(AF_INET,SOCK_STREAM,0);

	if(!sockfd){
		perror("Couldn't create a socket\n");
	}

	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&(int){1},sizeof(int));

	bzero((char *) &serv_addr, sizeof(serv_addr)); //Initialize the server address

	port = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
		perror("Failed to bind socket\n");
		return -1;
	}

	listen(sockfd,2);
	clilen = sizeof(cli_addr);
	while(1){
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if(newsockfd < 0){
			perror("Failed to accept\n");
			return -1;
		}

		
		do_server(newsockfd);
		
	}

	return 0;
}
