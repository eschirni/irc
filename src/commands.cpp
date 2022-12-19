#include "irc.hpp"

/****************************** COMMANDS ***************************************/

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

int	User::info(void)
{
	std::string	msg;
	int			return_code;

	msg += ":irc_serv.42HN.de 371 \t\r\n";
	msg += ":irc_serv.42HN.de 371 ATTENTION:\r\n";
	msg += ":irc_serv.42HN.de 371 This info page is under construction and there is no guarantee for these commands to work or for the descriptions to be complete.\r\n";
	msg += ":irc_serv.42HN.de 371 \t\r\n";
	msg += ":irc_serv.42HN.de 371 nick -- Change nickname\r\n";
	msg += ":irc_serv.42HN.de 371 user -- Set username and other options\r\n";
	msg += ":irc_serv.42HN.de 371 oper -- Gain operator status\r\n";
	msg += ":irc_serv.42HN.de 371 quit -- quit the server\r\n";
	return_code = send(_fd, msg.c_str(), msg.length(), 0);
	if (return_code < 0)
		return EXIT_FAILURE;
	remove_line();
	return EXIT_SUCCESS;
}

void User::oper(std::string nick, std::string pwd)
{
	std::string msg = RPL_BADCHANPASS;

	if (pwd != "teapot")
		send(this->_fd, msg.c_str(), msg.length(), 0);
	else
	{
		std::map<int, User>::iterator it = this->get_user(nick);
		if (it == this->_serv->users.end())
		{
			msg = ERR_NOSUCHNICK + nick + " :User not found.\r\n";
			send(this->_fd, msg.c_str(), msg.length(), 0);
			return ;
		}
		it->second._is_oper = true;
		msg = RPL_YOUREOPER;
		send(it->second._fd, msg.c_str(), msg.length(), 0);
	}
}

void User::nick(std::string nick)
{
	if (this->check_nickname(nick) == false)
		return ;
	this->send_all(":" + this->_nick_name + "!" + this->_user_name + "@" + SERV_ADDR + " NICK " + nick + "\r\n");
	this->_nick_name = nick;
}

void User::ping(std::string msg)
{
	msg = "PONG " + msg + "\r\n";
	send(this->_fd, msg.c_str(), msg.length(), 0);
}

void User::kill(std::string nick, std::string reason)
{
	std::map<int, User>::iterator it = this->get_user(nick);
	std::string msg = ERR_NOPRIVILEGES;

	if (it == this->_serv->users.end())
		msg = ERR_NOSUCHNICK + nick + " :User not found.\r\n";
	else if (this->_is_oper == true)
	{
		msg = ":irc_serv.42HN.de 371 You have been kicked: " + reason + "\r\n";
		send(it->second.getFd(), msg.c_str(), msg.length(), 0);
		msg = RPL_KILLDONE;
		it->second.setApproved(false);
	}
	send(this->_fd, msg.c_str(), msg.length(), 0);
}
