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

		std::vector<User::mapite_t>	_ops;
		std::vector<User::mapite_t>	_members;
		std::string				_name;

	public:
		/********************** CONSTRUCTION ***************************/

		Channel(std::string name, User::mapite_t creator);

		/********************** PUBLIC METHODS *************************/



};

#endif