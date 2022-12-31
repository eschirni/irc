#ifndef USER_HPP
# define USER_HPP

/******************************** INCLUDES *************************************/

# include <string>
# include <map>
# include <vector>

/******************************* USER-CLASS ************************************/

typedef struct s_serv t_serv;
class Channel;
class User
{
	typedef typename std::map<int, User>::iterator	mapite_t;
	typedef typename std::vector<Channel>::iterator	vecite_t;
	private:
		/********************** PIRVATE METHODS ************************/

		void						send_all(std::string msg);
		bool						check_nickname(std::string nick);
		int							initiate_handshake(std::string msg);
		int							process_handshake(void);
		int							send_welcome_reply(void);
		int							get_current_command(void);
		mapite_t					get_user(std::string nick);
		vecite_t					get_channel(const std::string name);
		std::vector<std::string>	get_command_arguments(void);


		/************************ COMMANDS *****************************/

		void	info(void);
		void 	oper(std::string nick, std::string pwd);
		void	nick(const std::string nick);
		void	ping(std::string msg);
		void	kill(std::string nick, std::string reason);
		void	privmsg(std::string target, std::string text, bool notice = false);
		void	lusers(void);
		void	away(std::string reply);
		void	mode(std::string target, std::string mode);
		void	join(std::string target);
		void	topic(std::string target, std::string topic);
		void	names(std::string target);
		void	quit(std::string leave_msg);
		void	kick(std::string target, std::string params);
		void	invite(std::string name, std::string target);
    

		/********************** DATA-MEMBERS ***************************/

		const int	_fd;
		t_serv		*_serv;
		bool		_first_msg;
		bool		_approved;
		char		_mode;
		std::string	_client_msg;
		std::string	_nick_name;
		std::string	_user_name;
		std::string	_real_name;
		std::string	_away_msg;

	public:
		/********************** CONSTRUCTION ***************************/

		User(int fd, t_serv *serv);
		~User(void);


		/********************** PUBLIC METHODS *************************/

		std::string	getClientMsg(void) const;
		std::string	getNickName(void) const;
		bool		getApproved(void) const;
		bool		getFirstMsg(void) const;
		int			getFd(void) const;
		void		setApproved(bool approval);
		int			process_msg(void);

		/************************ COMMANDS *****************************/

		void	part(std::string target, std::string leave_msg);
};


/********************************* ENDIF ***************************************/

#endif
