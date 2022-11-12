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

/*	config	*/
# define TIMEOUT	3 * 60 * 1000	// 3min
# define PORT		7777

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
int error(void);

# endif