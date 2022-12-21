#include "../include/irc.hpp"
#include <sstream>

template <typename T>
std::string NumberToString (T Number)
{
	std::ostringstream ss;
	ss << Number;
	return ss.str();
}

/****************************** COMMANDS ***************************************/

int	User::get_current_command(void)
{
	if (_client_msg.compare(0, 4, "NICK") == 0)
		return NICK;
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
	else if (_client_msg.compare(0, 6, "LUSERS") == 0)
		return LUSERS;
	else if (_client_msg.compare(0, 4, "INFO") == 0)
		return INFO;
	else if (_client_msg.compare(0, 4, "KILL") == 0)
		return KILL;
	else if (_client_msg.compare(0, 4, "PING") == 0)
		return PING;
	else if (_client_msg.compare(0, 3, "DIE") == 0)
		return DIE;
	else if (this->_client_msg.compare(0, 4, "AWAY") == 0)
		return (AWAY);
	else if (this->_client_msg.compare(0, 4, "MODE") == 0)
		return (MODE);
	else
		return -1;
}

void User::info(void)
{
	std::string	msg;

	msg += ":irc_serv.42HN.de 371 \t\r\n";
	msg += ":irc_serv.42HN.de 371 ATTENTION:\r\n";
	msg += ":irc_serv.42HN.de 371 This info page is under construction and there is no guarantee for these commands to work or for the descriptions to be complete.\r\n";
	msg += ":irc_serv.42HN.de 371 \t\r\n";
	msg += ":irc_serv.42HN.de 371 nick -- Change nickname\r\n";
	msg += ":irc_serv.42HN.de 371 user -- Set username and other options\r\n";
	msg += ":irc_serv.42HN.de 371 oper -- Gain operator status\r\n";
	msg += ":irc_serv.42HN.de 371 quit -- quit the server\r\n";
	send(_fd, msg.c_str(), msg.length(), 0);
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
		it->second._mode = 'o';
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
	else if (this->_mode == 'o')
	{
		msg = ":irc_serv.42HN.de 371 You have been kicked: " + reason + "\r\n";
		send(it->second.getFd(), msg.c_str(), msg.length(), 0);
		msg = RPL_KILLDONE;
		it->second.setApproved(false);
	}
	send(this->_fd, msg.c_str(), msg.length(), 0);
}

void User::privmsg(std::string target, std::string text)
{
	if (target[0] == '#') //todo
		return ;
	std::map<int, User>::iterator it = this->get_user(target);
	std::string msg = ":" + this->_nick_name + " PRIVMSG " + target + " " + text + "\r\n";

	if (it == this->_serv->users.end())
	{
		msg = ERR_NOSUCHNICK + target + " :User or channel not found.\r\n";
		send(this->_fd, msg.c_str(), msg.length(), 0);
	}
	else if (it->second._mode == 'a')
	{
		msg = RPL_AWAY + it->second._nick_name + " " + it->second._away_msg + "\r\n";
		send(this->_fd, msg.c_str(), msg.length(), 0);
	}
	else
		send(it->second.getFd(), msg.c_str(), msg.length(), 0);
}

void User::lusers(void)
{
	int users = 0;
	int invisible = 0;
	int ops = 0;
	int channels = 0; //todo

	mapite_t it = this->_serv->users.begin();
	while (it != this->_serv->users.end())
	{
		++users;
		if (it->second._mode == 'o')
			++ops;
		else if (it->second._mode == 'i')
			++invisible;
		++it;
	}
	std::string msg = ":irc_serv.42HN.de 251 :There are " + NumberToString(users) + " users and " + NumberToString(invisible) + " invisible users on 1 server\r\n";
	msg.append(":irc_serv.42HN.de 252 " + NumberToString(ops) + " :Operators on the server\r\n");
	msg.append(RPL_LUSERUNKNOWN);
	msg.append(":irc_serv.42HN.de 254 " + NumberToString(channels) + " :Channels on the server\r\n");
	msg.append(":irc_serv.42HN.de 255 :I have " + NumberToString(users) + " clients and 1 server\r\n");
	send(this->_fd, msg.c_str(), msg.length(), 0);
}

void User::away(std::string away_msg)
{
	std::string msg = RPL_NOWAWAY;

	this->_away_msg = away_msg;
	if (away_msg == "AWAY")
	{
		this->_mode = '0';
		this->_away_msg = "";
		msg = RPL_UNAWAY;
	}
	else
		this->_mode = 'a';
	send(this->_fd, msg.c_str(), msg.length(), 0);
}

void User::mode(std::string target, std::string mode) //need to fix basic confilcts with oper and away, users can only have one mode on OUR SERVER HEHE
{
	std::string msg;
	if (target[0] == '#') //todo
		return ;
	else
	{
		if (target != this->_nick_name)
			msg = ERR_USERSDONTMATCH + target + " :Target name doesn't match your nickname\r\n";
		else if (target == mode)
			msg = ERR_NEEDMOREPARAMS;
		else if ((mode[0] != '+' && mode[0] != '-') || (mode[1] != 'o' && mode[1] != 'O' && mode[1] != 'i'))
			msg = ERR_UMODEUNKNOWN + mode + " :No valid mode\r\n";
		else
		{
			if (mode[0] == '+' && (mode[1] != 'o' && mode[1] != 'O'))
				this->_mode = mode[1];
			else if (mode[0] == '-')
				this->_mode = '0';
			msg = RPL_UMODEIS + this->_nick_name + " " + this->_mode + CRLF;
		}
	}
	send(this->_fd, msg.c_str(), msg.length(), 0);
}
