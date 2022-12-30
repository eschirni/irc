#include "../include/channel.hpp"
#include <netinet/in.h>

Channel::Channel(std::string name, User *creator): _name(name), _topic("")
{
	this->_ops.push_back(creator);
	this->join(creator);
}

std::string Channel::get_list(void)
{
	std::string ret = "";
	std::vector<User *>::iterator mem = this->_members.begin();
	std::vector<User *>::iterator ops = this->_ops.begin();

	while (mem != this->_members.end())
	{
		ret += (*mem)->getNickName() + " ";
		++mem;
	}
	while (ops != this->_ops.end())
	{
		ret += "@" + (*ops)->getNickName() + " ";
		++ops;
	}
	return ret;
}

void Channel::send_all(std::string msg, std::string self)
{
	std::vector<User *>::iterator it = this->_members.begin();

	while (it != this->_members.end())
	{
		if (((*it)->getNickName() != self) || self == "")
			send((*it)->getFd(), msg.c_str(), msg.length(), 0);
		++it;
	}
}

std::vector<User *>::iterator Channel::get_member(std::string nick)
{
	std::vector<User *>::iterator it = this->_members.begin();

	while (it != this->_members.end() && (*it)->getNickName() != nick)
		++it;
	return it;
}

void Channel::join(User *usr)
{
	if (this->get_member(usr->getNickName()) != this->_members.end()) //user already in channel
		return ;
	this->_members.push_back(usr);
	std::string msg = ":" + usr->getNickName() + " JOIN :" + this->_name + "\r\n";
	this->send_all(msg);
	if (_topic != "")
	{
		msg = ":irc_serv.42HN.de 332 " + usr->getNickName() + " " + this->_name + " :" + this->_topic + "\r\n";
		send(usr->getFd(), msg.c_str(), msg.size(), 0);
	}
	msg = ":irc_serv.42HN.de 353 " + usr->getNickName() + " = " + this->_name + " : " + this->get_list() + "\r\n";
	send(usr->getFd(), msg.c_str(), msg.size(), 0);
	msg = ":irc_serv.42HN.de 366 " + usr->getNickName() + " " + this->_name + " :End of Names list\r\n";
	send(usr->getFd(), msg.c_str(), msg.size(), 0);
}

std::string Channel::getName(void)
{
	return this->_name;
}

bool Channel::has_member(std::string name)
{
	if (this->get_member(name) == this->_members.end())
		return false;
	return true;
}
