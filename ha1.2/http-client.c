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

void generate_http_get(char* target, const char* host) {
	static const char* prefix = "GET / HTTP/1.1\nHost: ";
	static const char* suffix = "\n\n";
	size_t offset = 0;
		
	memcpy(target, prefix, strlen(prefix));
	offset += strlen(prefix);

	memcpy(target + offset, host, strlen(host));
	offset += strlen(host);

	memcpy(target + offset, suffix, strlen(suffix));
	offset += strlen(suffix);

	target[offset] = 0;
}


int main(int argc, char* argv[]) {
	int sockfd;

	struct sockaddr_in their_addr; /* connector's address information */
	struct hostent* he;

	int serverPort;
	
	size_t rv;
	char buffer[1024];
			
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
	their_addr.sin_addr = *((struct in_addr*) he->h_addr);
	memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);

	if (connect(sockfd, (struct sockaddr*) &their_addr, sizeof(their_addr)) != 0) {
		fprintf(stderr, "Failed to connect\n");
		return 1;
	}
	
	generate_http_get(buffer, argv[1]);

	if (send(sockfd, buffer, 1024, 0) < 0) {
		fprintf(stderr, "Failed to send request!\n");
		return 1;
	}

	buffer[1024] = 0;

	do {
		rv = recv(sockfd, buffer, 1023, 0);
		
		if (rv > 0) {
			puts(buffer);
		}
	} while (rv >= 1023);
	
	if (close(sockfd) != 0) {
		fprintf(stderr, "Failed to shut socket down\n");
		return 1;
	}

	return 0;
}

