#include "irc.hpp"

int	error(const int errno_code)
{
	std::cerr << BRED << "Error: " << RESET;
	std::cerr << strerror(errno_code) << std::endl;
	return EXIT_FAILURE;
}

int	error(const char* error_msg)
{
	std::cerr << BRED << "Error: " << RESET << error_msg << std::endl;
	return EXIT_FAILURE;
}

void	ft_exit(int exit_code)
{
	std::cout << BMAG << "Exit " << RESET << "with code " << exit_code << std::endl;
	// system("leaks ircserv");
	exit(exit_code);
}

bool	is_ewouldblock(int errno_code)
{
	if (errno_code != EWOULDBLOCK)
	{
		error(errno_code);
		return false;
	}
	return true;
}

void	compress_array(t_serv* serv)
{
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