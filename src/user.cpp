#include "../include/irc.hpp"

/*********************** CONSTRUCTION / DESTRUCTION ****************************/

User::User(int fd, t_serv *serv) : _fd(fd), _serv(serv), _first_msg(true), _approved(false) {}

User::~User(void) {}


/**************************** GETTER / SETTER **********************************/

std::string	User::getClientMsg(void) const {return this->_client_msg;}

bool	User::getApproved(void) const {return this->_approved;}

void	User::setApproved(bool approval) {this->_approved = approval;}

bool	User::getFirstMsg(void) const {return this->_first_msg;}

int	User::getFd(void) const {return this->_fd;}

std::string	User::getNickName(void) const {return this->_nick_name;}


/**************************** PRIVATE METHODS **********************************/

bool	User::check_nickname(std::string nick)
{
	std::string msg;
	std::map<int, User>::iterator it = this->_serv->users.begin();

	while (it != this->_serv->users.end())
	{
		if (it->second.getNickName() == nick && it->second.getFd() != this->_fd)
		{
			msg = ERR_NICKNAMEINUSE + nick + " taken.\r\n";
			send(_fd, msg.c_str(), msg.length(), 0);
			return false;
		}
		++it;
	}
	return true;
}

std::map<int, User>::iterator	User::get_user(const std::string nick)
{
	std::map<int, User>::iterator it = this->_serv->users.begin();

	while (it != this->_serv->users.end() && it->second.getNickName() != nick)
		++it;
	return it;
}

std::vector<Channel>::iterator	User::get_channel(const std::string name)
{
	std::string msg;
	std::vector<Channel>::iterator it = this->_serv->channels.begin();

	while (it != this->_serv->channels.end() && it->getName() != name)
		++it;
	return it;
}

void User::send_all(std::string msg)
{
	std::map<int, User>::iterator it = this->_serv->users.begin();

	while (it != this->_serv->users.end())
	{
		send(it->second.getFd(), msg.c_str(), msg.length(), 0);
		++it;
	}
}

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
	this->_mode = _client_msg[suf_pos + 1];

	/* REAL_NAME */
	pre_pos = _client_msg.find(":", pre_pos) + 1;
	suf_pos = _client_msg.find("\r\n", pre_pos);
	_real_name = _client_msg.substr(pre_pos, suf_pos - pre_pos);
	remove_line(_client_msg, 3);
	if (check_nickname(this->_nick_name) == false)
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

std::vector<std::string>	User::get_command_arguments(void)
{
	std::vector<std::string>	argv;

	int pos = this->_client_msg.find(' ');
	std::string arg = this->_client_msg.substr(pos + 1, NPOS);
	int pos2 = arg.find(' ');
	std::string arg2 = arg.substr(pos2 + 1, NPOS);
	arg = this->_client_msg.substr(pos + 1, pos2);
	pos = arg.find('\r');
	arg = arg.substr(0, pos);
	pos = arg2.find('\r');
	arg2 = arg2.substr(0, pos);
	argv.push_back(arg);
	argv.push_back(arg2);
	return argv;
}


/***************************** PUBLIC METHODS **********************************/

int	User::process_msg(void)
{
	int							current_command;
	std::vector<std::string>	argv;
	std::string					send_msg;

	_client_msg.append(this->_serv->buffer);
	if (_client_msg.find(CRLF) == NPOS)
		return EXIT_FAILURE;
	if (_first_msg == true && initiate_handshake(this->_client_msg) == EXIT_FAILURE)
		return EXIT_FAILURE;
	if (_client_msg.empty())
		return EXIT_SUCCESS;
	current_command = get_current_command();
	argv = get_command_arguments();
	std::cout << this->_client_msg; //debug
	switch (current_command)
	{
		case INFO:
			this->info();
			break;
		case OPER:
			this->oper(argv[0], argv[1]);
			break;
		case NICK:
			this->nick(argv[0]);
			break;
		case PING:
			this->ping(argv[0]);
			break;
		case KILL:
			this->kill(argv[0], argv[1]);
			break;
		case PRIVMSG:
			this->privmsg(argv[0], argv[1]);
			break;
		case NOTICE:
			this->privmsg(argv[0], argv[1], true);
			break;
		case LUSERS:
			this->lusers();
			break;
		case AWAY:
			this->away(argv[0]);
			break;
		case MODE:
			this->mode(argv[0], argv[1]);
			break;
		case JOIN:
			this->join(argv[0]);
			break;
		case TOPIC:
			this->topic(argv[0], argv[1]);
			break;
		case NAMES:
			this->names(argv[0]);
			break;
		case PART:
			this->part(argv[0], argv[1]);
			break;
		case QUIT:
			this->quit(argv[0]);
			break;

		/* FILE TRANSFER */
		// trying to send the file 'file.txt' to user 'someone' via the following command in weechat:
		// /dcc send someone file.txt
		// results in the client sending our server the following:
		// "PRIVMSG someone :DCC SEND file.txt 2886860548 37009 0"
	
		default:
			send_msg = ERR_UNKNOWNCOMMAND + this->_client_msg.substr(0, this->_client_msg.find(' ')).substr(0, this->_client_msg.find('\r')) + " :Unknown command\r\n";
			send(_fd, send_msg.c_str(), send_msg.length(), 0);
			break;
	}
	this->_client_msg = "";
	return EXIT_SUCCESS;
}