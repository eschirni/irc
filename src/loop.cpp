#include "irc.hpp"

int	irc_loop(t_serv* serv)
{
	int	return_code;
	int tmp_size;
	int usr_fd;
	bool close_conn;
	bool compress_array = false;
	bool end_serv = false;

	do
	{
		return_code = poll(serv->fds, serv->n_fds, serv->timeout);
		if (return_code < 0)
			return(error()); //for failure
		else if (return_code == 0) //for timout
			return (error());
		tmp_size = serv->n_fds;
		for (int i = 0; i < tmp_size; i++)
		{
			if (serv->fds[i].revents == 0)
				continue;
			else if (serv->fds[i].revents != POLLIN)
				return (error());
			else if (serv->fds[i].fd == serv->listen_sd)
			{
				do
				{
					usr_fd = accept(serv->listen_sd, NULL, NULL);
					if (usr_fd < 0)
					{
						if (errno != EWOULDBLOCK)
						{
							error();
							end_serv = true;
						}
						break;
					}
					serv->fds[serv->n_fds].fd = usr_fd;
					serv->fds[serv->n_fds].events = POLLIN;
					serv->n_fds++;
				} while (usr_fd != -1);
				
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
							error();
							close_conn = true;
						}
						break;
					}
					else if (return_code == 0) //connection closed by client
					{
						error();
						close_conn = true;
						break;
					}
					serv->len = return_code;
					printf("%d bytes received \n", serv->len); //debug
					return_code = send(serv->fds[i].fd, serv->buffer, serv->len, 0);
					if (return_code < 0)
					{
						error();
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
	} while (end_serv == false);
	return (EXIT_SUCCESS);
}