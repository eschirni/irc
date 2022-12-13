#include "irc.hpp"

static int	input_check(char** argv)
{
	std::string	str_port = static_cast<std::string>(argv[1]);
	std::string password = static_cast<std::string>(argv[2]);
	int port = atoi(str_port.c_str());
	size_t password_length = password.length();

	if (str_port.find_first_not_of("1234567890") != NPOS)
		return (error(INVPORT));
	if (port < PORT_MIN || PORT_MAX < port)
		return (error(INVPORT));
	if (password_length < PASSW_MIN_LEN)
		return (error(PASSTOSMALL));
	if (password_length > PASSW_MAX_LEN)
		return (error(PASSTOBIG));
	if (password.find_first_not_of("1234567890") == NPOS)
		return (error(PASSNOCHAR));
	if (password.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz") == NPOS)
		return (error(PASSNOINT));
	if (password.find("42") == NPOS)
		return (error(PASSNOFT));
	if (password.find("ThisIsTheBestIrcIHaveEverSeen") == NPOS)
		return (error(PASSNOHN));
	return EXIT_SUCCESS;
}

static void	init_serv_struct(t_serv* serv, char** argv)
{
	memset(&serv->address, 0, sizeof(serv->address));
	serv->address.sin_family = AF_INET;
	serv->address.sin_port = htons(atoi(argv[1]));
	serv->password = static_cast<std::string>(argv[2]);
}

int	initialization(t_serv* serv, char** argv)
{
	int	return_code;

	if (input_check(argv) == EXIT_FAILURE)
		return EXIT_FAILURE;
	init_serv_struct(serv, argv);
	serv->listen_sd = socket(AF_INET, SOCK_STREAM, 0);
	if (serv->listen_sd < 0)
		return (error(errno));
	return_code = fcntl(serv->listen_sd, F_SETFL, O_NONBLOCK);
	if (return_code < 0)
		return (close(serv->listen_sd), error(errno));
	return_code = bind(serv->listen_sd, (struct sockaddr *)&serv->address, sizeof(serv->address));
	if (return_code < 0)
		return (close(serv->listen_sd), error(errno));
	return_code = listen(serv->listen_sd, 0);
	if (return_code < 0)
		return (close(serv->listen_sd), error(errno));
	serv->fds.push_back(pollfd());
	serv->fds[0].fd = serv->listen_sd;
	serv->fds[0].events = POLLIN;
	return (EXIT_SUCCESS);
}
