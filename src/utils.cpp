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

int	info(const char* info_msg) //debug
{
	std::cout << BYEL << "Info: " << RESET << info_msg << std::endl;
	return EXIT_SUCCESS;
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

int	erase_element(t_serv* serv, size_t index)
{
	close(serv->fds[index].fd);
	std::vector<pollfd>::iterator it = serv->fds.begin();
	while (it - serv->fds.begin() != static_cast<long int>(index))
		it++;
	serv->fds.erase(it);
	serv->users.erase(it->fd);
	return EXIT_FAILURE;
}