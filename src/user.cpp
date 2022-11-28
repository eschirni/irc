#include "irc.hpp"

/*********************** CONSTRUCTION / DESTRUCTION ****************************/

User::User(void) : _fd(0) {}

User::User(int fd) : _fd(fd), _first_msg(true) {}

User::~User(void) {}


/**************************** GETTER / SETTER **********************************/

int	User::getFd(void) const {return _fd;}

std::string	User::getClientMsg(void) const {return _client_msg;}


/******************************** METHODS **************************************/

int	User::initiate_handshake(const char* msg)
{
	int			crlf_count = 0;
	int			cmd_count = 0;
	u_long		pos = 0;
	std::string	tmp = static_cast<std::string>(msg);

	while (pos != NPOS)
	{
		pos = tmp.find("\r\n", pos);
		if (pos != NPOS)
		{
			crlf_count++;
			pos += 2;
		}
	}
	if (crlf_count == 1 || crlf_count == 3)
	{
		if (tmp.find("CAP") != NPOS)
			cmd_count++;
		if (tmp.find("NICK") != NPOS)
			cmd_count++;
		if (tmp.find("USER") != NPOS)
			cmd_count++;
	}
	if (cmd_count == 3)
	{
		_first_msg = false;
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}

int	User::process_msg(const char* msg)
{
	if (_first_msg == true && initiate_handshake(msg) == EXIT_FAILURE)
	{
		std::cout << "EXIT_FAILURE" << std::endl;
		return EXIT_FAILURE;
	}
	_client_msg.append(msg);
	std::cout << _client_msg << std::endl;
	return EXIT_SUCCESS;
}