#include "irc.hpp"

int	clean_up(t_serv* serv) //closes all left open fds
{
	for (int i = 0; i < serv->n_fds; i++)
	{
		if (serv->fds[i].fd >= 0)
			close (serv->fds[i].fd);
	}
	return (EXIT_SUCCESS);
}