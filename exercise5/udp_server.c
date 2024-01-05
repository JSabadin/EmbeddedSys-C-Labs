/* Nepovezaven UDP streznik */
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

#define FIFO_V "fifo_file"

int main(void)
{
	int inft;
	size_t buff_size = 1460; //1280*720*4;
	char buffer[buff_size];
	size_t bytes;

	int sockfd, n;
	struct sockaddr_in servaddr, cliaddr;
	socklen_t len;
	char mesg[MAXLINE];

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("socket err");
		exit(1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERVPORTNO);

	if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
	{
		perror("bind err");
		exit(1);
	}

	if ((inft = open(FIFO_V, O_RDONLY)) < 0)
	{
		perror("napaka pri odpiranju fifo_v");
		exit(3);
	}

	while (1)
	{
		len = sizeof(cliaddr);
		n = recvfrom(sockfd, mesg, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
		if ((bytes = read(inft, buffer, sizeof(buffer))) == -1)
		{
			perror("napaka pri branju FIFO_V");
			exit(4);
		}
		// printf("client: %s:%d\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
		sendto(sockfd, buffer, bytes, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
	}

	if (close(inft) == -1)
	{
		perror("napaka pri zapiranju FIFO_V");
		exit(5);
	}
}
