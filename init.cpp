#include "irc.hpp"

int	initialization(t_serv* serv)
{
	int	return_code;

	serv->len = 1;
	serv->n_fds = 1;
	serv->listen_sd = socket(AF_INET, SOCK_STREAM, 0);
	if (serv->listen_sd < 0)
		return (error());
	return_code = fcntl(serv->listen_sd, F_SETFL, O_NONBLOCK);
	if (return_code < 0)
		return (close(serv->listen_sd), error());
	memset(&serv->address, 0, sizeof(serv->address));
	serv->address.sin_family = AF_INET;
	serv->address.sin_port = htons(420);
	return_code = bind(serv->listen_sd, (struct sockaddr *)&serv->address, sizeof(serv->address));
	if (return_code < 0)
		return (close(serv->listen_sd), error());
	return_code = listen(serv->listen_sd, 0);
	if (return_code < 0)
		return (close(serv->listen_sd), error());
	memset(serv->fds, 0, sizeof(serv->fds)); //sets fd array elements to 0
	serv->fds[0].fd = serv->listen_sd; //dunno if needed
	serv->fds[0].events = POLLIN;
	serv->timeout = 180000;
	return (EXIT_SUCCESS);
}