#include <iostream>
#include <sys/socket.h>

//for tests
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>

int main(int argc, char **argv)
{
	struct sockaddr_in	address;
	int					rc;

	if (argc != 3)
		return (1);
	int sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		std::cerr << "socket() failed" << std::endl;
		std::cerr << "errno: " << strerror(errno) << std::endl;
		return EXIT_FAILURE;
	}
	address.sin_family = AF_LOCAL;
    address.sin_port = htons(6667);
	rc = bind(sockfd, (struct sockaddr*)&address, sizeof(address));
	if (rc < 0)
	{
		std::cerr << "bind() failed" << std::endl;
		std::cerr << "errno: " << strerror(errno) << std::endl;
		close(sockfd);
		return EXIT_FAILURE;
	}
	rc = listen(sockfd, 3);
	if (rc < 0)
	{
		std::cerr << "listen() failed" << std::endl;
		std::cerr << "errno: " << strerror(errno) << std::endl;
		close(sockfd);
		return EXIT_FAILURE;
	}
	int tmp_len = sizeof(address);
	int new_socket = accept(sockfd, (struct sockaddr*)&address, (socklen_t*)&tmp_len);
	char buffer[1024] = { 0 };
	read(new_socket, buffer, 1024);
	send(new_socket, argv[2], strlen(argv[2]), 0);
	close(new_socket);
	shutdown(sockfd, SHUT_RDWR);
	return EXIT_SUCCESS;
}