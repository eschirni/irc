#include <iostream>
#include <sys/socket.h>

//for tests
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{
	struct sockaddr_in address;

	if (argc != 3)
		return (1);
	int sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
	address.sin_family = AF_LOCAL;
    address.sin_port = htons(7777);
	bind(sockfd, (struct sockaddr*)&address, sizeof(address));
	listen(sockfd, 3);
	int tmp_len = sizeof(address);
	int new_socket = accept(sockfd, (struct sockaddr*)&address, (socklen_t*)&tmp_len);
	char buffer[1024] = { 0 };
	read(new_socket, buffer, 1024);
	send(new_socket, argv[2], strlen(argv[2]), 0);
	close(new_socket);
	shutdown(sockfd, SHUT_RDWR);
	return (0);
}