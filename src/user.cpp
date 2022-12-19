#include "irc.hpp"

/*********************** CONSTRUCTION / DESTRUCTION ****************************/

User::User(int fd, t_serv *serv) : _fd(fd), _serv(serv), _first_msg(true), _approved(false) {}

User::~User(void) {}


/**************************** GETTER / SETTER **********************************/

std::string	User::getClientMsg(void) const {return _client_msg;}

bool	User::getApproved(void) const {return _approved;}

void	User::setApproved(bool approval) {_approved = approval;}

bool	User::getFirstMsg(void) const {return _first_msg;}

int	User::getFd(void) const {return _fd;}

std::string	User::getNickName(void) const {return _nick_name;}


/**************************** PRIVATE METHODS **********************************/

bool	User::check_nickname(void)
{
	std::string msg;
	std::map<int, User>::iterator it = this->_serv->users.begin();
	std::map<int, User>::iterator end = this->_serv->users.end();

	while (it != end)
	{
		if (it->second.getNickName().compare(this->_nick_name) == 0 && it->second.getFd() != this->_fd)
		{
			msg += ERR_NICKNAMEINUSE;
			send(_fd, msg.c_str(), msg.length(), 0);
			return false;
		}
		++it;
	}
	return true;
}

std::map<int, User>::iterator	User::get_user(std::string nick)
{
	std::string msg;
	std::map<int, User>::iterator it = this->_serv->users.begin();

	while (it != this->_serv->users.end() && it->second.getNickName() != nick)
		++it;
	return it;
}

int	User::get_current_command(void)
{
	if (_client_msg.compare(0, 4, "NICK") == 0)
		return NICK;
	else if (_client_msg.compare(0, 4, "USER") == 0)
		return USER;
	else if (_client_msg.compare(0, 4, "OPER") == 0)
		return OPER;
	else if (_client_msg.compare(0, 4, "QUIT") == 0)
		return QUIT;
	else if (_client_msg.compare(0, 4, "JOIN") == 0)
		return JOIN;
	else if (_client_msg.compare(0, 4, "PART") == 0)
		return PART;
	else if (_client_msg.compare(0, 5, "TOPIC") == 0)
		return TOPIC;
	else if (_client_msg.compare(0, 5, "NAMES") == 0)
		return NAMES;
	else if (_client_msg.compare(0, 4, "LIST") == 0)
		return LIST;
	else if (_client_msg.compare(0, 6, "INVITE") == 0)
		return INVITE;
	else if (_client_msg.compare(0, 4, "KICK") == 0)
		return KICK;
	else if (_client_msg.compare(0, 7, "PRIVMSG") == 0)
		return PRIVMSG;
	else if (_client_msg.compare(0, 6, "NOTICE") == 0)
		return NOTICE;
	else if (_client_msg.compare(0, 4, "INFO") == 0)
		return INFO;
	else if (_client_msg.compare(0, 4, "KILL") == 0)
		return KILL;
	else if (_client_msg.compare(0, 4, "PING") == 0)
		return PING;
	else if (_client_msg.compare(0, 3, "DIE") == 0)
		return DIE;
	else
		return -1;
}

void User::oper(std::string nick, std::string pwd)
{
	std::string msg = RPL_BADCHANPASS;

	if (pwd != "teapot")
		send(this->_fd, msg.c_str(), msg.length(), 0);
	else
	{
		std::map<int, User>::iterator it = this->get_user(nick);
		it->second.is_oper = true;
		msg = RPL_YOUREOPER;
		send(it->second._fd, msg.c_str(), msg.length(), 0);
	}
}

void User::nick(std::string nick)
{
	if (this->check_nickname() == false)
		return ;
	this->_nick_name = nick;
}

void User::ping(std::string msg)
{
	msg = "PONG " + msg + "\r\n";
	send(this->_fd, msg.c_str(), msg.length(), 0);
}

//TODO kick, PART, JOIN <- Create Channel class

int	User::send_welcome_reply(void)
{
	int			return_code;
	std::string	msg;
	
	msg = RPL_WELCOME + RPL_YOURHOST + RPL_CREATED;
	return_code = send(_fd, msg.c_str(), msg.length(), 0);
	if (return_code < 0)
		return (error(errno));
	return EXIT_SUCCESS;
}

void	User::remove_line(int times)
{
	size_t					crlf_pos;
	std::string::iterator	from;
	std::string::iterator	to;

	for (int i = 0; i < times; i++)
	{
		crlf_pos = _client_msg.find("\r\n", 0) + 2;
		from = _client_msg.begin();
		to = _client_msg.begin() + crlf_pos;
		_client_msg.erase(from, to);
	}
}		

int	User::process_handshake(void)
{
	size_t	pre_pos;
	size_t	suf_pos;

	/* NICK_NAME */
	pre_pos = _client_msg.find("NICK") + static_cast<std::string>("NICK ").length();
	suf_pos = _client_msg.find("\r\n", pre_pos);
	_nick_name = _client_msg.substr(pre_pos, suf_pos - pre_pos);

	/* USER_NAME */
	pre_pos = _client_msg.find("USER") + static_cast<std::string>("USER ").length();
	suf_pos = _client_msg.find(" ", pre_pos);
	_user_name = _client_msg.substr(pre_pos, suf_pos - pre_pos);

	/* USER_MODE */
	_user_mode = std::strtod(&_client_msg[suf_pos + 1], NULL);

	/* REAL_NAME */
	pre_pos = _client_msg.find(":", pre_pos) + 1;
	suf_pos = _client_msg.find("\r\n", pre_pos);
	_real_name = _client_msg.substr(pre_pos, suf_pos - pre_pos);
	remove_line(3);
	if (check_nickname() == false)
		return EXIT_FAILURE;
	if (send_welcome_reply() == EXIT_FAILURE)
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

int	User::initiate_handshake(std::string msg)
{
	int		crlf_count = 0;
	int		cmd_count = 0;
	u_long	pos = 0;

	while (pos != NPOS)
	{
		pos = msg.find("\r\n", pos);
		if (pos != NPOS)
		{
			crlf_count++;
			pos += 2;
		}
	}
	if (crlf_count == 1 || crlf_count == 3)
	{
		if (msg.find("CAP") != NPOS)
			cmd_count++;
		if (msg.find("NICK") != NPOS)
			cmd_count++;
		if (msg.find("USER") != NPOS)
			cmd_count++;
	}
	if (cmd_count == 3)
	{
		if (process_handshake() == EXIT_FAILURE)
			return EXIT_FAILURE;
		_first_msg = false;
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}


/***************************** PUBLIC METHODS **********************************/

int	User::process_msg(void)
{
	int			current_command;
	int			return_code;
	std::string send_msg;

	_client_msg.append(this->_serv->buffer);
	if (_first_msg == true && initiate_handshake(this->_client_msg) == EXIT_FAILURE)
		return EXIT_FAILURE;
	if (_client_msg.empty())
		return EXIT_SUCCESS;
	std::cout << "_client_msg:" << std::endl << _client_msg << std::endl; //debug
	current_command = get_current_command();
	int pos = this->_client_msg.find(' '); //split would be awesome
	std::string arg = this->_client_msg.substr(pos + 1, std::string::npos);
	switch (current_command)
	{
		case INFO:
			this->info();
			break;
		case OPER:
			this->oper(arg, "teapot");
			break;
		case NICK:
			this->nick(arg);
			break;
		case PING:
			this->ping(arg);
			break;

		/* FILE TRANSFER */
		// trying to send the file 'file.txt' to user 'someone' via the following command in weechat:
		// /dcc send someone file.txt
		// results in the client sending our server the following:
		// "PRIVMSG someone :DCC SEND file.txt 2886860548 37009 0"
	
		default:
			send_msg = ERR_UNKNOWNCOMMAND;
			return_code = send(_fd, send_msg.c_str(), send_msg.length(), 0);
			if (return_code < 0)
				return EXIT_FAILURE;
			break;
	}
	return EXIT_SUCCESS;
}