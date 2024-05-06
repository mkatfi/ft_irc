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
#include <errno.h>
#include "irc.hpp"
#include "channel.hpp"
#include <cstdlib>

struct request {

	std::string cmd;
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
		std::vector<int> bot_fds;


	public :

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
		// int parss_Request(const std::string &command, request &p);
		// void parse_and_process_command(Client &client, const std::string &command);

		int pass(Client &client, request &p);
		int getAuthentified(Client& cli, request&);
		void Nick(Client& Client, request &p);
		void user(Client& client, request &p);

		std::string join(Client &client, request &p);
		// void createChannel(std::string& channel, int fd, Client &t);
		void createChannel(std::string &channel, Client &t);
		void joinChannel(std::string &channel, Client &t);
		std::string join_message(std::string channel, int fd);

		void commands(request& req, Client& client);
		int searchForDestination(request& req);
		void sendMessageToClient(request& req, Client& cli, int client_dest);

		std::string kick(Client &client, request &p);
		std::string invite(Client &client, request &p);
		void addmember(std::string &channel, Client *t);
		std::string Topic(Client &client, request &p);
		void sendMSGToChannel(Client &, request &);
		void Mode(Client&, request&);
		void send_all_member(int sockfd, const std::string &message);

		void bot(Client &client, request &p);
};
