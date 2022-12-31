#include "../include/channel.hpp"
#include <netinet/in.h>
#include <iostream> //debug

Channel::Channel(std::string name, User *creator): _name(name), _topic("")
{
	this->_ops.push_back(creator);
	this->join(creator);
}

void Channel::print_list(User *usr, bool all)
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
	std::string msg = ":irc_serv.42HN.de 353 " + usr->getNickName() + " = " + this->_name + " : " + ret + "\r\n";
	if (all == true)
		send_all(msg);
	else
		send(usr->getFd(), msg.c_str(), msg.size(), 0);
	msg = ":irc_serv.42HN.de 366 " + usr->getNickName() + " " + this->_name + " :End of Names list\r\n";
	if (all == true)
		send_all(msg);
	else
		send(usr->getFd(), msg.c_str(), msg.size(), 0);
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

std::vector<User *>::iterator Channel::get_op(std::string nick)
{
	std::vector<User *>::iterator it = this->_ops.begin();

	while (it != this->_ops.end() && (*it)->getNickName() != nick)
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
	msg = ":irc_serv.42HN.de 331 " + usr->getNickName() + " " + this->_name + " :no topic is set\r\n";
	if (_topic != "")
		msg = ":irc_serv.42HN.de 332 " + usr->getNickName() + " " + this->_name + " " + this->_topic + "\r\n";
	send(usr->getFd(), msg.c_str(), msg.size(), 0);
	this->print_list(usr);
}

void Channel::topic(User *usr, std::string topic)
{
	std::string msg = ":irc_serv.42HN.de 331 " + usr->getNickName() + " " + this->_name + " :no topic is set\r\n";

	if (this->get_op(usr->getNickName()) != this->_ops.end() && topic != "")
	{
		this->_topic = topic;
		msg = ":irc_serv.42HN.de 332 " + usr->getNickName() + " " + this->_name + " " + this->_topic + "\r\n";
	}
	else if (topic != "" && this->get_op(usr->getNickName()) == this->_ops.end())
		msg = ":irc_serv.42HN.de 482 " + this->_name + " :you are not an operator of this channel\r\n";
	else if (this->_topic != "")
		msg = ":irc_serv.42HN.de 332 " + usr->getNickName() + " " + this->_name + " " + this->_topic + "\r\n";
	send(usr->getFd(), msg.c_str(), msg.length(), 0);
}

void Channel::names(User *usr)
{
	this->print_list(usr);
}

void Channel::part(User *usr, std::string leave_msg)
{
	std::string reply = ":" + usr->getNickName() + " PART " + this->_name + " " + leave_msg + "\r\n";
	this->send_all(reply);
	this->_members.erase(this->get_member(usr->getNickName()));
	if (this->get_op(usr->getNickName()) != this->_ops.end())
		this->_ops.erase(this->get_op(usr->getNickName()));
}

void Channel::op(User *usr, std::string mode, std::string name)
{
	std::string msg = "";

	if (name == mode)
		name = usr->getNickName();
	std::vector<User *>::iterator it = this->get_member(name);
	if (it == this->_members.end())
		msg = ":irc_serv.42HN.de 442 " + this->_name + " :" + name + " not on channel\r\n";
	else if (this->get_op(usr->getNickName()) == this->_ops.end())
		msg = ":irc_serv.42HN.de 482 " + this->_name + " :you are not an operator of this channel\r\n";
	else if (mode[0] == '-')
	{
		it = this->get_op(name);
		if (it != this->_ops.end())
		{
			this->_ops.erase(it);
			this->print_list(usr, true);
		}
		else
			msg = ":irc_serv.42HN.de 482 " + this->_name + " :" + name + " is not an operator of this channel\r\n";
	}
	else
	{
		if (this->get_op(name) == this->_ops.end())
		{
			this->_ops.push_back(*it);
			this->print_list(usr, true);
		}
	}
	send(usr->getFd(), msg.c_str(), msg.length(), 0);
}

void Channel::kick(User *usr, std::string name, std::string kick_msg)
{
	std::string msg = ":" + usr->getNickName() + " KICK " + this->_name + " " + name + " " + kick_msg + "\r\n";
	std::vector<User *>::iterator it = this->get_member(name);

	if (this->get_op(usr->getNickName()) == this->_ops.end())
		msg = ":irc_serv.42HN.de 482 " + this->_name + " :you are not an operator of this channel\r\n";
	else if (it == this->_members.end())
		msg = ":irc_serv.42HN.de 442 " + this->_name + " :" + name + " not on channel\r\n";
	if (this->get_op(usr->getNickName()) == this->_ops.end() || it == this->_members.end())
		send(usr->getFd(), msg.c_str(), msg.length(), 0);
	else
	{
		send_all(msg);
		(*it)->part(this->_name, kick_msg);
	}
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
