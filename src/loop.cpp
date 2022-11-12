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
		serv->fds[serv->n_fds].fd = usr_fd;
		serv->fds[serv->n_fds].events = POLLIN;
		serv->n_fds++;
	}
	return status;
}

int	irc_loop(t_serv* serv)
{
	int	return_code;
	int tmp_size;
	bool continue_connection;
	bool compress_array = false;
	bool status = true;

	while (status)
	{
		return_code = poll(serv->fds, serv->n_fds, serv->timeout);
		if (return_code < 0)
			return(error(errno)); //for failure
		else if (return_code == 0) //for timout
			return (error(POLLEXP));
		tmp_size = serv->n_fds;
		for (int i = 0; i < tmp_size; i++)
		{
			if (serv->fds[i].revents == 0)
				continue;
			else if (serv->fds[i].revents != POLLIN)
				return (error(REVENT));
			else if (serv->fds[i].fd == serv->listen_sd)
				status = establish_new_connection(serv);
			else
			{
				continue_connection = true;
				while (true)
				{
					return_code = recv(serv->fds[i].fd, serv->buffer, serv->len, 0);
					if (return_code < 0)
					{
						continue_connection = is_ewouldblock(errno);
						break;
					}
					else if (return_code == 0) //connection closed by client
					{
						error(CCLOSE);
						continue_connection = false;
						break;
					}
					serv->len = return_code;
					printf("%d bytes received \n", serv->len); //debug
					return_code = send(serv->fds[i].fd, serv->buffer, serv->len, 0);
					if (return_code < 0)
					{
						error(errno);
						continue_connection = false;
						break;
					}
				}
				if (continue_connection == false)
				{
					close(serv->fds[i].fd);
					serv->fds[i].fd = -1;
					compress_array = true;
				}
			}
		}
		if (compress_array == true)
		{
			compress_array = false;
			for (int i = 0; i < serv->n_fds; i++)
			{
				if (serv->fds[i].fd == -1)
				{
					for (int j = 0; j < serv->n_fds; j++)
						serv->fds[j].fd = serv->fds[j + 1].fd;
					i--;
					serv->n_fds--;
				}
			}
			
		}
	}
	return (EXIT_SUCCESS);
}