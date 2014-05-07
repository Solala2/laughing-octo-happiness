/*
############################################################################
#                                                                          #
# Copyright TU-Berlin, 2011-2014                                           #
# Die Weitergabe, Veröffentlichung etc. auch in Teilen ist nicht gestattet #
#                                                                          #
############################################################################
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
#include <time.h>

#define MAX_BUFFER_LENGTH 100

int main(int argc, char* argv[]) {
	int sockfd;

	struct sockaddr_in their_addr; /* connector's address information */
	struct hostent* he;

	int serverPort;

	struct timespec local_time; 
	
	if (argc < 3) {
		fprintf(stderr, "Usage: %s <serverName> <serverPort>\n", argv[0]);
		exit(1);
	}

	serverPort = atoi(argv[2]);

	/* Resolv hostname to IP Address */
	if ((he = gethostbyname(argv[1])) == NULL) { /* get the host info */
		herror("gethostbyname");
		exit(1);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0) {
		fprintf(stderr, "Failed to create socket: %i\n", sockfd);
		return 1;
	}

	/* setup transport address */
	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(serverPort);
	their_addr.sin_addr = *((struct in_addr*)he->h_addr);
	memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);

	if (connect(sockfd, (struct sockaddr*) &their_addr, sizeof(their_addr)) != 0) {
		fprintf(stderr, "Failed to connect\n");
		return 1;
	}
		
	if (clock_gettime(CLOCK_REALTIME, &local_time) == 0) {
		send(sockfd, &local_time, sizeof(local_time), 0);
	}	

	if (close(sockfd) != 0) {
		fprintf(stderr, "Failed to shut socket down\n");
		return 1;
	}

	return 0;
}

