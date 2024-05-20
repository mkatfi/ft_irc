#include "irc.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <sstream>

#define BUFFER_SIZE 1024

struct Client {

	int	socket_fd;
	int step;
	

	/* authentification of client */
	std::string pass;
	std::string nickName;
	std::string hostName;
	std::string serverName;
	std::string userName;
	std::string realName;
	std::string buffer;
	int count;
	std::vector<std::string> _channel;
  	bool authenticated;
	bool bot_authenticated;
	int bot_count;


	Client() 
	{
		this->count = 0;
		this->step = C_ESTABLISHING_CONNECTION;
		this->authenticated = false;
		this->bot_authenticated = false;
		this->bot_count = 0;
	}

	// Client(int fd)
	// {
	// 	socket_fd = fd;
	// }
};
