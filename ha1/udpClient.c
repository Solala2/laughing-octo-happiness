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

#define MAX_BUFFER_LENGTH 100

int packData(unsigned char* buffer, unsigned int a, unsigned int b) {
	/* ******************************************************************
	TO BE DONE:  pack data
	******************************************************************* */

	buffer[0] = (a >> 8) & 255;
	buffer[1] = a & 255;
	buffer[2] = (b >> 8) & 255;
	buffer[3] = b & 255;

	return 4;
}

int main(int argc, char* argv[]) {
	int sockfd;

	struct sockaddr_in their_addr; /* connector's address information */
	struct sockaddr_in my_addr;
	struct hostent* he;

	int serverPort;
	int a = 0;
	int b = 0;

	unsigned char buffer[4];

	printf("TCP client example\n\n");

	if (argc != 5) {
		fprintf(stderr, "Usage: tcpClient serverName serverPort int1 int2\n");
		exit(1);
	}

	serverPort = atoi(argv[2]);
	a = atoi(argv[3]);
	b = atoi(argv[4]);

	/* Resolv hostname to IP Address */
	if ((he = gethostbyname(argv[1])) == NULL) { /* get the host info */
		herror("gethostbyname");
		exit(1);
	}

	/* ******************************************************************
	TO BE DONE: Create socket
	******************************************************************* */

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	if (sockfd < 0) {
		fprintf(stderr, "Failed to create socket: %i\n", sockfd);
		return 1;
	}

	/* setup transport address */
	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(serverPort);
	their_addr.sin_addr = *((struct in_addr*)he->h_addr);
	memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);


	/* ******************************************************************
	TO BE DONE: Bind
	******************************************************************* */

	memset(&my_addr, 0, sizeof(my_addr));
	my_addr.sin_family = AF_INET;

	if (bind(sockfd, (struct sockaddr*) &my_addr, sizeof(my_addr)) != 0) {
		fprintf(stderr, "Failed to bind\n");
		return 1;
	}

	/* ******************************************************************
	TO BE DONE:  Send data
	******************************************************************* */

	packData(buffer, a, b);

	if (sendto(sockfd, buffer, 4, 0,
	           (struct sockaddr*) &their_addr, sizeof(their_addr)) < 4) {
		fprintf(stderr, "Failed to send\n");
		return 0;
	}

	/* ******************************************************************
	TO BE DONE:  Close socket
	******************************************************************* */

	if (close(sockfd) != 0) {
		fprintf(stderr, "Failed to shut socket down\n");
		return 1;
	}

	return 0;
}
