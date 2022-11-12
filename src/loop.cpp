#include "irc.hpp"

int	irc_loop(t_serv* serv)
{
	int	return_code;
	int tmp_size;
	int usr_fd = 0;
	bool close_conn;
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
			{
				while (usr_fd != -1)
				{
					usr_fd = accept(serv->listen_sd, NULL, NULL);
					if (usr_fd < 0)
					{
						if (errno != EWOULDBLOCK)
						{
							error(errno);
							status = true;
						}
						break;
					}
					serv->fds[serv->n_fds].fd = usr_fd;
					serv->fds[serv->n_fds].events = POLLIN;
					serv->n_fds++;
				}
			}
			else
			{
				close_conn = false;
				while (true)
				{
					return_code = recv(serv->fds[i].fd, serv->buffer, serv->len, 0);
					if (return_code < 0)
					{
						if (errno != EWOULDBLOCK) //recv failure
						{
							error(errno);
							close_conn = true;
						}
						break;
					}
					else if (return_code == 0) //connection closed by client
					{
						error(CCLOSE);
						close_conn = true;
						break;
					}
					serv->len = return_code;
					printf("%d bytes received \n", serv->len); //debug
					return_code = send(serv->fds[i].fd, serv->buffer, serv->len, 0);
					if (return_code < 0)
					{
						error(errno);
						close_conn = true;
						break;
					}
				}
				if (close_conn == true)
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