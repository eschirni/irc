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

/*	structs	*/
typedef struct s_serv
{
	int					len;
	int					listen_sd;
	int					new_sd;
	int					timeout;
	int					n_fds;
	int					tmp_size;
	bool				desc_ready;
	bool				end_server;
	bool				compress_array;
	bool				close_conn;
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