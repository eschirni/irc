#include "irc.hpp"

static void	init_serv_struct(t_serv* serv)
{
	serv->len = 1;
	serv->n_fds = 1;
	serv->timeout = TIMEOUT;
	memset(&serv->address, 0, sizeof(serv->address));
	serv->address.sin_family = AF_INET;
	serv->address.sin_port = htons(PORT);
	// memset(serv->fds, 0, sizeof(serv->fds)); //sets fd array elements to 0 //replace by vector
}

int	initialization(t_serv* serv)
{
	int	return_code;

	init_serv_struct(serv);
	serv->listen_sd = socket(AF_INET, SOCK_STREAM, 0);
	if (serv->listen_sd < 0)
		return (error(errno));
	return_code = fcntl(serv->listen_sd, F_SETFL, O_NONBLOCK);
	if (return_code < 0)
		return (close(serv->listen_sd), error(errno));
	return_code = bind(serv->listen_sd, (struct sockaddr *)&serv->address, sizeof(serv->address));
	if (return_code < 0)
		return (close(serv->listen_sd), error(errno));
	return_code = listen(serv->listen_sd, 0);
	if (return_code < 0)
		return (close(serv->listen_sd), error(errno));
	serv->fds.push_back(pollfd());
	serv->fds[0].fd = serv->listen_sd;
	serv->fds[0].events = POLLIN;
	return (EXIT_SUCCESS);
}