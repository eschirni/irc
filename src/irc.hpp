#ifndef IRC_HPP
# define IRC_HPP

/*	includes	*/
# include <iostream>
# include <sys/socket.h>
# include <cerrno>	// errno
# include <cstring>	// strerror
# include <sys/poll.h> //poll
# include <fcntl.h> //fcntl

//for tests
# include <netinet/in.h>
# include <unistd.h>

//for linux
# include <cstdio>
# include <cstdlib>

/*	config	*/
# define TIMEOUT	3 * 60 * 1000	// 3min
# define PORT		4181

/*	defines	*/
# define NEWLINE()	std::cout << std::endl

/*	structs	*/
typedef struct s_serv
{
	int					len;
	int					listen_sd;
	int					timeout;
	int					n_fds;
	char				buffer[80];
	struct sockaddr_in	address;
	struct pollfd		fds[200];
} t_serv;

/*	init.cpp	*/
int	initialization(t_serv* serv);

/*	loop.cpp	*/
int	irc_loop(t_serv* serv);

/*	clean.cpp	*/
int clean_up(t_serv* serv);

/*	utils.cpp	*/
int		error(int errno_code);
int		error(const char* error_msg);
void	ft_exit(int exit_code);
bool	is_ewouldblock(int errno_code);

/*	error messages	*/
# define POLLEXP	"Poll time out expired"
# define REVENT		"Unexpected return event result"
# define CCLOSE		"Connection closed by client"

/*	colors	*/
# define BLK "\e[0;30m"
# define RED "\e[0;31m"
# define GRN "\e[0;32m"
# define YEL "\e[0;33m"
# define BLU "\e[0;34m"
# define MAG "\e[0;35m"
# define CYN "\e[0;36m"
# define WHT "\e[0;37m"
# define BBLK "\e[1;30m"
# define BRED "\e[1;31m"
# define BGRN "\e[1;32m"
# define BYEL "\e[1;33m"
# define BBLU "\e[1;34m"
# define BMAG "\e[1;35m"
# define BCYN "\e[1;36m"
# define BWHT "\e[1;37m" 
# define RESET "\e[0m"

# endif