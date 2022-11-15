#include "irc.hpp"

static bool	establish_new_connection(t_serv* serv)
{
	int		usr_fd = 0;
	bool	status = true;

	while (usr_fd != -1)
	{
		usr_fd = accept(serv->listen_sd, NULL, NULL);
		if (usr_fd < 0)
		{
			status = is_ewouldblock(errno);
			break;
		}
		serv->fds.push_back(pollfd());
		serv->fds[serv->n_fds].fd = usr_fd;
		serv->fds[serv->n_fds].events = POLLIN;
		serv->n_fds++;
	}
	return status;
}

static int	process_existing_connection(t_serv* serv, std::vector<pollfd>::iterator it)
{
	int return_code = 0;

	memset(serv->buffer, 0, BUFFER_SIZE);
	while (true)
	{
		return_code = recv(it->fd, serv->buffer, BUFFER_SIZE, 0);
		if (return_code < 0)
		{
			if (is_ewouldblock(errno) == false)
				return (erase_element(serv, it));
			return EXIT_SUCCESS;
		}
		else if (return_code == 0) //connection closed by client
			return (erase_element(serv, it), error(CCLOSE));
		return_code = send(it->fd, serv->buffer, BUFFER_SIZE, 0);
		if (return_code < 0)
			return (erase_element(serv, it), error(errno));
	}
}

int	irc_loop(t_serv* serv)
{
	int	return_code;
	bool status = true;

	while (status == true)
	{
		return_code = poll(&serv->fds[0], serv->n_fds, TIMEOUT);
		if (return_code < 0)
			return(error(errno)); //for failure
		else if (return_code == 0) //for timout
			return (error(POLLEXP));
		for (std::vector<pollfd>::iterator it = serv->fds.begin(); it != serv->fds.end(); it++)
		{
			if (it->revents == 0)
				continue;
			else if (it->revents != POLLIN)
				return (error(REVENT));
			else if (it->fd == serv->listen_sd)
				status = establish_new_connection(serv);
			else
				process_existing_connection(serv, it);
		}
	}
	return (EXIT_SUCCESS);
}