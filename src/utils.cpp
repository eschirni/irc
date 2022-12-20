#include "../include/irc.hpp"

void	remove_line(std::string& string, int iterations)
{
	size_t					crlf;
	std::string::iterator 	from;
	std::string::iterator	to;

	for (int i = 0; i < iterations; ++i)
	{
		crlf = string.find(CRLF) + static_cast<std::string>(CRLF).length();
		from = string.begin();
		to = string.begin() + crlf;
		string.erase(from, to);
	}
}

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

int	erase_element(t_serv* serv, size_t index)
{
	close(serv->fds[index].fd);
	std::vector<pollfd>::iterator it = serv->fds.begin() + index;
	serv->users.erase(it->fd);
	serv->fds.erase(it);
	return EXIT_FAILURE;
}