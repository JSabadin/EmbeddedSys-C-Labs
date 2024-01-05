/* Nepovezaven UDP odjemalec */
#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> //ta je potrebna za creat, open,...

#define MAXLINE 4096
#define SERVPORTNO 50000

#define FIFO_I "video.h264"

int main(int argc, char **argv)
{
	int outft;				 //datoteki (pisemo v outft, beremo iz inft)
	size_t buff_size = 1460; //1280*720*4;
	char buffer[buff_size];
	size_t bytes;

	int sockfd, n;
	struct sockaddr_in servaddr;
	char sendline[MAXLINE], recvline[MAXLINE];

	if (argc != 2)
	{
		printf("usage: udpcli <IP address>\n");
		exit(1);
	}

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("socket err");
		exit(2);
	}
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	servaddr.sin_port = htons(SERVPORTNO);

	if ((outft = creat(FIFO_I, 0644)) == -1)
	{
		printf("napaka creat \n");
		exit(3);
	}

	while (1)
	{
		n = sendto(sockfd, sendline, strlen(sendline), 0,
				   (struct sockaddr *)&servaddr, sizeof(servaddr));
		if (n == -1)
		{
			perror("sento err");
			exit(1);
		}

		bytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
		if (bytes < 0)
		{
			perror("recvfrom err");
			exit(4);
		}

		//printf(buffer);

		if (write(outft, buffer, bytes) < 0)
		{
			perror("napaka pri pisanju v fifo_i");
			exit(5);
		}
	}
	if (close(outft) == -1)
	{
		perror("napaka pri zapiranju v5_test");
		exit(6);
	}
	exit(0);
}


// ./udp_client 192.168.91.55