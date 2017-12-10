//Partially stolen from http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <sys/syscall.h>



void do_client(int sockfd){
	char buffer[256];
	printf("Received data from server:\n");
	while(read(sockfd, buffer, 256) > 0){
		printf("%s\n\n", buffer);
	}
}

int main(int argc, char** argv){

	int sockfd, port;
	

	if(argc != 2){
		printf("Correct usage is %s [port]\n",argv[0]);
		return -1;
	}

	

	struct sockaddr_in serv_addr;
	struct hostent *server;

	port = atoi(argv[1]);
    	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		perror("Failed to open socket\n");
		
		return -1;
	}

	server = gethostbyname("localhost");
	if (!server) {
		printf("Couldn't find localhost");
		
		return -1;
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(port);

	if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){ 
		perror ("Error connecting to server\n");
		
		return -1;
	}

	do_client(sockfd);

	
	printf("Transmission completed\n");

	return 0;
}
