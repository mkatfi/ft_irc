#pragma once

#include <iostream>
#include <map>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <stdexcept>
#include <limits.h>
#include <vector>
#include <netinet/in.h>
#include "client.hpp"
#include "channel.hpp"
#include <errno.h>

struct request {

	std::string commands;
	std::vector<std::string> arg;
	int statu;
};

class Server {

	private:
		int			port;
		std::string	password;
		int 		server_fd;
		std::map<int, Client>	clients;
		std::map<std::string, Channel*>	channels;
		sockaddr_in address;


	public:
		void	importConfig(std::string importedPort, std::string importedPassword);
		void	hostServer();
		void	awaitingTraffic();
		void	clientResponse();
		void	clearClients(std::vector<int> clientsToBeRemoved, fd_set &totalfds);
		void	handleResponseRequest(Client &client);
		void	handleReadRequest(Client &client);
	/*-------------------------------CPMMAND-----------------------------------------------------*/
		void send_message(int sockfd, const std::string &message);
		std::string split(std::string const &str);
		int parss_Request(const std::string &command, request &p);
		void parse_and_process_command(Client &client, const std::string &command);

		int pass(Client &client, request &p);

		std::string join(Client &client, request &p);
		void createChannel(std::string& channel, int fd, Client &t);
		void joinChannel(std::string &channel, int fd, Client &t);
};
