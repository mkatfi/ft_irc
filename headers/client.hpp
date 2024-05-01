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
	std::vector< std::string> _channel;
  	bool authenticated;

	Client() {

		this->step = C_ESTABLISHING_CONNECTION;
		this->authenticated = false;
	}
	// Client(int fd)
	// {
	// 	socket_fd = fd;
	// }
};
