/*
Adapted from Beej's listener.c UDP server.
https://beej.us/guide/bgnet/source/examples/listener.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MYPORT "65330"	// the port users will be connecting to

#define MAXBUFLEN 100

#define BACKLOG 10 

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
	int sockfd, new_fd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;
	struct sockaddr_storage their_addr;
	char buf[MAXBUFLEN];
	socklen_t addr_len;
	char s[INET6_ADDRSTRLEN];
	int rps, rps_c;
	bool won;
	int yes=1;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET6; // set to AF_INET to use IPv4
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	//ROCK PAPER SCISSORS
	do{
	printf("Rock Paper Scissors: Enter 1 for rock, 2 for paper, 3 for scissors:");
	scanf("%d", &rps);
	printf("\n");
	}
	while ((rps != 1) && (rps != 2) && (rps != 3)); 

	if ((rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("listener: socket");
			continue;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("listener: bind");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "listener: failed to bind socket\n");
		return 2;
	}

	freeaddrinfo(servinfo);

	printf("listener: waiting to recvfrom...\n");



	addr_len = sizeof their_addr;
	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
		(struct sockaddr *)&their_addr, &addr_len)) == -1) {
		perror("recvfrom");
		exit(1);
	}

	printf("listener: got packet from %s\n",
		inet_ntop(their_addr.ss_family,
			get_in_addr((struct sockaddr *)&their_addr),
			s, sizeof s));
	printf("listener: packet is %d bytes long\n", numbytes);
	buf[numbytes] = '\0';
	printf("listener: packet contains \"%s\"\n", buf);

	//Convert to integer
	if (strcmp(buf,"rock")){
		rps_c = 1;
	}
	else if (strcmp(buf,"paper")){
		rps_c = 2;
	}
	else if (strcmp(buf,"scissors")){
		rps_c = 3;
	}
	else {
		rps_c = 4; //invalid string received
	}

	if (rps_c == rps) {
		printf("You tied!\n");
	}
	else {
		switch (rps_c) {
			case 1:
				if (rps == 2)
					won = true;
				break;
			case 2:
				if (rps == 3)
					won = true;
		        	break;
			case 3:
				if (rps == 1)
					won = true;
				break;
			default:
				printf("Invalid move received from the client.\n");
		}


	}
	if (won)
		printf("You won!\n");
	else
		printf("You didn't win...\n");
	


	close(sockfd);

	return 0;
}
