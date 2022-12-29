#ifndef CHANNEL_HPP
# define CHANNEL_HPP

/******************************** INCLUDES *************************************/

# include "user.hpp"
# include <vector>


/******************************* CHANNEL-CLASS ************************************/

class Channel
{
	private:
		/********************** PIRVATE METHODS ************************/



		/********************** DATA-MEMBERS ***************************/

		std::vector<std::string>	_ops;
		std::vector<User *>			_members;
		std::string					_name;

	public:
		/********************** CONSTRUCTION ***************************/

		Channel(std::string name, User *creator);

		/********************** PUBLIC METHODS *************************/



};

#endif