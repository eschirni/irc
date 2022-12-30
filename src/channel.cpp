#include "../include/channel.hpp"
#include <netinet/in.h>

Channel::Channel(std::string name, User *creator): _name(name)
{
	this->_ops.push_back(creator);
	this->join(creator);
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
}

std::string Channel::getName(void)
{
	return this->_name;
}