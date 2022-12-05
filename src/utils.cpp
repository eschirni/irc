#include "irc.hpp"

void	print_str_with_crlf(const char* s, bool print_nonprint)//debug
{
	for (int i = 0; s[i]; ++i)
	{
		switch (s[i])
		{
		case '\a':
			std::cout << "\\a";
			if (print_nonprint == true)
				std::cout << "\a";
			break;
		
		case '\b':
			std::cout << "\\b";
			if (print_nonprint == true)
				std::cout << "\b";
			break;
		
		case '\t':
			std::cout << "\\t";
			if (print_nonprint == true)
				std::cout << "\t";
			break;
		
		case '\n':
			std::cout << "\\n";
			if (print_nonprint == true)
				std::cout << "\n";
			break;
		
		case '\v':
			std::cout << "\\v";
			if (print_nonprint == true)
				std::cout << "\v";
			break;
		
		case '\f':
			std::cout << "\\f";
			if (print_nonprint == true)
				std::cout << "\f";
			break;
		
		case '\r':
			std::cout << "\\r";
			if (print_nonprint == true)
				std::cout << "\r";
			break;
		
		case ' ':
			std::cout << "_";
		
		default:
			std::cout << s[i];
			break;
		}
	}
}

int	info(const char* info_msg) //debug
{
	std::cout << BYEL << "Info: " << RESET << info_msg << std::endl;
	return EXIT_SUCCESS;
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
	std::vector<pollfd>::iterator it = serv->fds.begin();
	while (it - serv->fds.begin() != static_cast<long int>(index))
		it++;
	serv->fds.erase(it);
	// serv->users.erase(it->fd); // FIXME container-overflow upon disconnect
	return EXIT_FAILURE;
}