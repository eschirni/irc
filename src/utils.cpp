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