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
		serv->fds.back().fd = usr_fd;
		serv->fds.back().events = POLLIN;
		serv->users.insert(std::pair<int, User>(serv->fds.back().fd, User(usr_fd)));
	}
	return status;
}

static int	process_existing_connection(t_serv* serv, size_t index)
{
	int return_code;

	if (serv->fds[index].fd == -1)
		return (info(CCLOSE), erase_element(serv, index));
	memset(serv->buffer, 0, BUFFER_SIZE);
	return_code = recv(serv->fds[index].fd, serv->buffer, BUFFER_SIZE, 0);
	if (return_code < 0 && is_ewouldblock(errno) == false)
		return (erase_element(serv, index));
	else if (return_code == 0) //connection closed by client
		return (erase_element(serv, index), error(CCLOSE));
	serv->users.find(serv->fds[index].fd)->second.process_msg(serv->buffer);
	// std::cout << serv->buffer; //debug
	return EXIT_SUCCESS;
}

int	irc_loop(t_serv* serv)
{
	int	return_code;
	bool status = true;

	while (status == true)
	{
		return_code = poll(&serv->fds[0], serv->fds.size(), TIMEOUT);
		if (return_code < 0)
			return(error(errno)); //for failure
		else if (return_code == 0) //for timout
			return (error(POLLEXP));
		for (size_t i = 0; i < serv->fds.size(); i++)
		{
			/*
			if (serv->fds[i].revents != 0) //debug
			{
				printf("  fd=%d; events: %s%s%s\n", serv->fds[i].fd,
						(serv->fds[i].revents & POLLIN)  ? "POLLIN "  : "",
						(serv->fds[i].revents & POLLHUP) ? "POLLHUP " : "",
						(serv->fds[i].revents & POLLERR) ? "POLLERR " : "");
			}
			*/
			if (serv->fds[i].revents == 0)
				continue;
			else if (serv->fds[i].revents & POLLHUP)
				serv->fds[i].fd = -1;
			else if (serv->fds[i].revents != POLLIN)
				return (error(REVENT));
			else if (serv->fds[i].fd == serv->listen_sd)
				status = establish_new_connection(serv);
			else
				process_existing_connection(serv, i);
		}
	}
	return (EXIT_SUCCESS);
}