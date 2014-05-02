#include <sys/socket.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define fail(msg) { fprintf(stderr, msg); fputc('\n', stderr); return 1; }

void unpack(uint8_t* buffer, uint16_t* a, uint16_t* b) {
	*a = (buffer[0] << 8) | buffer[1];
	*b = (buffer[2] << 8) | buffer[3];
}

uint16_t gcd(uint16_t a, uint16_t b) {
	return b != 0 ? gcd(b, a % b) : a;
}

int main(int argc, char** argv) {
	struct sockaddr_in mine;
	int sockfd;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s <port>\n", argv[0]);
		return 1;
	}


	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
		fail("Couldn't create socket!");

	memset(&mine, 0, sizeof(mine));
	mine.sin_family = AF_INET;
	mine.sin_port = htons(atoi(argv[1]));

	if (bind(sockfd, (struct sockaddr*) &mine, sizeof(mine)) < 0)
		fail("Couldn't bind");

	if (listen(sockfd, 5) < 0)
		fail("Couldn't listen");

	while (1) {
		int cfd;
		uint8_t buffer[4];

		cfd = accept(sockfd, NULL, NULL);

		if (cfd < 0)
			continue;

		if (recv(cfd, buffer, 4, 0) == 4) {
			uint16_t a, b;
			unpack(buffer, &a, &b);

			if (a > 0 && b > 0) {
				printf("a = %i, b = %i\n", a, b);
				printf("GCD is %i\n", gcd(a, b));
			} else {
				printf("Nope\n");
			}
		}

		close(cfd);
	}

	close(sockfd);

	return 0;
}
