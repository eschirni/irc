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

	msg += RPL_INFO + std::string("\t") + CRLF;
	msg += RPL_INFO + std::string("/******************* INFO *******************/") + CRLF;
	msg += RPL_INFO + std::string("The following contains the commands that our") + CRLF;
	msg += RPL_INFO + std::string("ft_irc server has implemented. For a more") + CRLF;
	msg += RPL_INFO + std::string("detailed description of a specific command") + CRLF;
	msg += RPL_INFO + std::string("type in the following '/help <command>'") + CRLF;
	msg += RPL_INFO + std::string("\t") + CRLF;
	msg += RPL_INFO + std::string("Channel Management:") + CRLF;
	msg += RPL_INFO + std::string("/list -- List all currently open channels") + CRLF;
	msg += RPL_INFO + std::string("/join -- Join or create channels") + CRLF;
	msg += RPL_INFO + std::string("/part -- Leave one or more channels") + CRLF;
	msg += RPL_INFO + std::string("/names -- List all users in a channel") + CRLF;
	msg += RPL_INFO + std::string("/op -- Enlighten small power onto someone") + CRLF;
	msg += RPL_INFO + std::string("/topic -- Add or change a channel description") + CRLF;
	msg += RPL_INFO + std::string("/kick -- Kick an user from a channel") + CRLF;
	msg += RPL_INFO + std::string("/invite -- Invite an user to a channel") + CRLF;
	msg += RPL_INFO + std::string("\t") + CRLF;
	msg += RPL_INFO + std::string("Private Messaging:") + CRLF;
	msg += RPL_INFO + std::string("/msg -- Message an user privately") + CRLF;
	msg += RPL_INFO + std::string("/notice -- Message an user privately") + CRLF;
	msg += RPL_INFO + std::string("/away -- Set yourself as away") + CRLF;
	msg += RPL_INFO + std::string("\t") + CRLF;
	msg += RPL_INFO + std::string("Other:") + CRLF;
	msg += RPL_INFO + std::string("/info -- Get this nice info chart here") + CRLF;
	msg += RPL_INFO + std::string("/oper -- Enlighten big power onto someone") + CRLF;
	msg += RPL_INFO + std::string("/mode -- Change an user mode from an user") + CRLF;
	msg += RPL_INFO + std::string("/lusers -- List information about the server") + CRLF;
	msg += RPL_INFO + std::string("/nick -- Change your nickname") + CRLF;
	msg += RPL_INFO + std::string("/ping -- Ping the server") + CRLF;
	msg += RPL_INFO + std::string("/kill -- Close a client-server connection") + CRLF;
	msg += RPL_INFO + std::string("/quit -- Leave WeeChat") + CRLF;
	msg += RPL_INFO + std::string("\t") + CRLF;
	msg += RPL_INFO + std::string("File Transfer:") + CRLF;
	msg += RPL_INFO + std::string("/dcc send <user> <filepath>") + CRLF;
	msg += RPL_INFO + std::string("Upon accepting the file transfer you can find") + CRLF;
	msg += RPL_INFO + std::string("the file under the following path:") + CRLF;
	msg += RPL_INFO + std::string("~/.local/share/weechat/xfer/") + CRLF;
	msg += RPL_INFO + std::string("/********************************************/") + CRLF;
	send(_fd, msg.c_str(), msg.length(), 0);
}

void User::oper(std::string nick, std::string pwd)
{
	std::string msg = RPL_BADCHANPASS;

	if (pwd != OPER_PASSW)
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
		if (it->second._mode == 'a')
			send(it->second._fd, RPL_UNAWAY, std::string(RPL_UNAWAY).length(), 0);
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
		it->second.part("", ":* user has been killed");
		msg = ":irc_serv.42HN.de 371 You have been killed: " + reason + "\r\n";
		send(it->second.getFd(), msg.c_str(), msg.length(), 0);
		msg = RPL_KILLDONE;
		it->second.setApproved(false);
	}
	send(this->_fd, msg.c_str(), msg.length(), 0);
}

void User::privmsg(std::string target, std::string text, bool notice)
{
	std::string msg = ":" + this->_nick_name + " PRIVMSG " + target + " " + text + CRLF;
	if (notice == true)
		msg = ":" + this->_nick_name + " NOTICE " + target + " " + text + CRLF;

	if (target[0] == '#')
	{
		std::vector<Channel>::iterator it = this->get_channel(target);
		if (it == this->_serv->channels.end() || it->has_member(this->_nick_name) == false)
		{
			msg = ERR_CANNOTSENDTOCH + target + " :You are not member of a channel with this name\r\n";
			send(this->_fd, msg.c_str(), msg.length(), 0);
		}
		else
		{
			it->send_all(msg, this->_nick_name);
			if (it->getName() == "#Support" && text.find_last_of('?') == text.length() - 1)
			{
				msg = ":@42-Pascal PRIVMSG #Support Ask your peers!\r\n";
				it->send_all(msg);
			}
		}
	}
	else //users
	{
		std::map<int, User>::iterator it = this->get_user(target);
		if (it == this->_serv->users.end())
		{
			msg = ERR_NOSUCHNICK + target + " :User not found.\r\n";
			send(this->_fd, msg.c_str(), msg.length(), 0);
		}
		else
		{
			send(it->second.getFd(), msg.c_str(), msg.length(), 0);
			if (notice == false && it->second._mode == 'a')
			{
				msg = RPL_AWAY + it->second._nick_name + " " + it->second._away_msg + CRLF;
				send(this->_fd, msg.c_str(), msg.length(), 0);
			}
		}
	}	
}

void User::lusers(void)
{
	int invisible = 0;
	int ops = 0;

	std::map<int, User>::iterator it = this->_serv->users.begin();
	while (it != this->_serv->users.end())
	{
		if (it->second._mode == 'o')
			++ops;
		else if (it->second._mode == 'i')
			++invisible;
		++it;
	}
	std::string msg = ":irc_serv.42HN.de 251 :There are " + NumberToString(this->_serv->users.size()) + " users and " + NumberToString(invisible) + " invisible users on 1 server\r\n";
	msg.append(":irc_serv.42HN.de 252 " + NumberToString(ops) + " :Operators on the server\r\n");
	msg.append(RPL_LUSERUNKNOWN);
	msg.append(":irc_serv.42HN.de 254 " + NumberToString(this->_serv->channels.size()) + " :Channels on the server\r\n");
	msg.append(":irc_serv.42HN.de 255 :I have " + NumberToString(this->_serv->users.size()) + " clients and 1 server\r\n");
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

void User::mode(std::string target, std::string mode) //maybe write mode rpl for oper and away too so it changes in weechat
{
	std::string msg = ERR_UNKNOWNMODE + mode + " :unknown mode\r\n";

	if (target[0] == '#')
	{
		std::vector<Channel>::iterator it = this->get_channel(target);
		if (it == this->_serv->channels.end() || it->has_member(this->_nick_name) == false)
			msg = ":irc_serv.42HN.de 442 " + target + " :not on channel\r\n";
		else if (mode == target)
			msg = ERR_NOCHANMODES + target + " :channel modes not supported\r\n";
		else if (mode[1] == 'o')
			return (it->op(&get_user(this->_nick_name)->second, mode.substr(0, mode.find(' ')), mode.substr(mode.find(' ') + 1, NPOS)));
	}
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
			if (mode[0] == '+' && (mode[1] != 'o' && mode[1] != 'O' && this->_mode != 'a'))
				this->_mode = mode[1];
			else if (mode[0] == '-' && this->_mode != 'a')
				this->_mode = '0';
			msg = RPL_UMODEIS + this->_nick_name + " " + this->_mode + CRLF;
		}
	}
	send(this->_fd, msg.c_str(), msg.length(), 0);
}

void User::join(std::string target)
{
	std::vector<std::string>			target_split = split(target, ',');
	std::vector<std::string>::iterator	it = target_split.begin();
	std::vector<Channel>::iterator		channel;
	std::string 						msg;

	if (target[0] == '0' && target.length() == 1)
		return part("", ":*");
	while (it != target_split.end())
	{
		channel = this->get_channel(*it);
		msg = ERR_BADCHANMASK + *it + " :Channel names have to start with #\r\n";
		if (it[0][0] != '#')
			send(this->_fd, msg.c_str(), msg.length(), 0);
		else if (channel == this->_serv->channels.end())
			this->_serv->channels.push_back(Channel(*it, &get_user(this->_nick_name)->second));
		else
			channel->join(&get_user(this->_nick_name)->second);
		++it;
	}
}

void User::topic(std::string target, std::string topic)
{
	std::vector<Channel>::iterator it = this->get_channel(target);
	std::string msg = ":irc_serv.42HN.de 442 " + target + " :not on channel\r\n";

	if (topic == target)
		topic = "";
	if (it == this->_serv->channels.end() || it->has_member(this->_nick_name) == false)
		send(this->_fd, msg.c_str(), msg.length(), 0);
	else
		it->topic(&get_user(this->_nick_name)->second, topic);
}

void User::names(std::string target)
{
	std::vector<Channel>::iterator it = this->get_channel(target);
	if (it != this->_serv->channels.end())
		it->names(&get_user(this->_nick_name)->second);
}

void User::part(std::string target, std::string leave_msg)
{
	std::vector<std::string>			target_split = split(target, ',');
	std::vector<std::string>::iterator	it = target_split.begin();
	std::vector<Channel>::iterator		channel;
	std::string 						msg;

	if (leave_msg[1] == '*')
	{
		if (leave_msg.length() >= 3)
			leave_msg.erase(BEGIN, 3);
		else
			leave_msg = PART_ALL_MSG;
		for (std::vector<Channel>::iterator it = _serv->channels.begin(); it != _serv->channels.end(); ++it)
		{
			if (it->has_member(_nick_name) == true)
				it->part(this, leave_msg);
		}
		return;
	}
	while (it != target_split.end())
	{
		channel = this->get_channel(*it);
		msg = ":irc_serv.42HN.de 442 " + *it + " :not on channel\r\n";
		if (channel != this->_serv->channels.end() && channel->has_member(this->_nick_name) == true)
			channel->part(&get_user(this->_nick_name)->second, leave_msg);
		else
			send(this->_fd, msg.c_str(), msg.length(), 0);
		++it;
	}
}

void	User::quit(std::string leave_msg)
{
	if (leave_msg.compare(":WeeChat") == 0)
		part("", ":* user disconnected");
	else 
		part("", ":* " + leave_msg);
}

void User::kick(std::string target, std::string params)
{
	std::vector<Channel>::iterator it = this->get_channel(target);
	std::string msg = ":irc_serv.42HN.de 442 " + target + " :not on channel\r\n";

	if (it != this->_serv->channels.end() && it->has_member(this->_nick_name) == true)
		it->kick(&get_user(this->_nick_name)->second, params.substr(0, params.find(' ')), params.substr(params.find(' ') + 1, NPOS));
	else
		send(this->_fd, msg.c_str(), msg.length(), 0);
}

void User::invite(std::string name, std::string target)
{
	std::vector<Channel>::iterator it = this->get_channel(target);
	std::map<int, User>::iterator usr = this->get_user(name);
	std::string msg = ":irc_serv.42HN.de 442 " + target + " :not on channel\r\n";

	if (usr == this->_serv->users.end())
		msg = ERR_NOSUCHNICK + name + " :User not found.\r\n";
	else if (it != this->_serv->channels.end() && it->has_member(this->_nick_name) == true)
		return (it->invite(&get_user(this->_nick_name)->second, &usr->second));
	send(this->_fd, msg.c_str(), msg.length(), 0);
}

void User::list(void)
{
	std::vector<Channel>::iterator it = this->_serv->channels.begin();
	std::string msg;

	while (it != this->_serv->channels.end())
	{
		msg = RPL_LIST + it->getName() + " :" + it->getTopic() + CRLF;
		send(this->_fd, msg.c_str(), msg.length(), 0);
		++it;
	}
	msg = RPL_LISTEND;
	send(this->_fd, msg.c_str(), msg.length(), 0);
}
