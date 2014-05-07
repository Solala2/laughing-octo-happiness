#include <sys/socket.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define fail(msg) { fprintf(stderr, msg); fputc('\n', stderr); return 1; }

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
		
		struct timespec local_time;
		struct timespec remote_time;

		cfd = accept(sockfd, NULL, NULL);

		if (cfd < 0)
			continue;

		
		if (recv(cfd, &remote_time, sizeof(remote_time), 0) == sizeof(remote_time)
		    && clock_gettime(CLOCK_REALTIME, &local_time) == 0) {
			printf("> check\n");
			printf("local:  %20lus, %20luns\n", local_time.tv_sec, local_time.tv_nsec);
			printf("remote: %20lus, %20luns\n", remote_time.tv_sec, remote_time.tv_nsec);
			
			printf("diff:   %43lins\n", (local_time.tv_sec - remote_time.tv_sec) * 1000000000
			                          + (local_time.tv_nsec - remote_time.tv_nsec));
		}
		
		close(cfd);
	}

	close(sockfd);

	return 0;
}
