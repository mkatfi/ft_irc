/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   awaiting_traffic.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oryadi <oryadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 00:20:44 by hchaguer          #+#    #+#             */
/*   Updated: 2024/05/17 16:56:12 by oryadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/server.hpp"
#include <cstddef>
#include <cstdlib>
#include <sstream>
#include <string>

void	acceptIncomingConnection(std::map<int, Client> &clients, fd_set &totalfds, int server_fd)
{
	sockaddr client_s;

	socklen_t client_length;

	int newfd = accept(server_fd, &client_s, &client_length);

	if (newfd == -1)
		return;

	if (newfd < 0) throw std::runtime_error("Systemcall `accept` failed.");

	std::cout << "\033[1;32mClient : " << newfd << " connected...\033[0m" << std::endl;

	FD_SET(newfd, &totalfds);

	Client client;

	client.socket_fd = newfd;

	clients.insert(std::make_pair(newfd, client));
}

void	Server::clearClients(std::vector<int> BeRemoved, fd_set &totalfds)
{
	for (std::vector<int>::iterator it = BeRemoved.begin(); it != BeRemoved.end(); it++)
	{
		std::map<int, Client>::iterator clientFound = clients.find(*it);

		close(clientFound->first);

		clients.erase(clientFound);

		FD_CLR(*it, &totalfds);
	}
}

void    Server::handleReadRequest(Client &client)
{
    char buf[1024];
	request req;

	std::memset(buf, 0, sizeof(buf));
    int bytes_received = recv(client.socket_fd, buf, sizeof(buf) - 1, 0);

	if (bytes_received <= 0)
	{
		std::cout << "\e[0;31mClient " << client.socket_fd << " disconnected " << std::endl;
		client.step = C_CLOSE_CONNECTION;
		return;
	}
    if (bytes_received > 0)
	{
		std::string str;
		static std::string str1;
        buf[bytes_received] = '\0';
		str1 += buf;
		size_t pos = str1.find_first_of("\r\n");
		if (pos != std::string::npos)
		{
			str = str1.substr(0, pos);
			str1.clear();
		}
		std::stringstream iss(str);
		std::string line;
		iss >> req.cmd;
		
		while (iss >> line)
			req.arg.push_back(line);
    }
	if (getAuthentified(client, req) == 3)
	{
		if (client.authenticated == false)
		{
			send_message(client.socket_fd, RPL_WELCOME(client.nickName));
			send_message(client.socket_fd, RPL_YOURHOST(client.nickName, client.serverName));
			send_message(client.socket_fd, RPL_CREATED(client.nickName));
			send_message(client.socket_fd, RPL_MYINFO(client.nickName, client.serverName));
			std::cout << client.nickName << " Welcome to irc server!" << std::endl;
			client.authenticated = true;
		}
	}
}

void	Server::awaitingTraffic()
{
	fd_set totalfds, readfds, writefds;

	FD_ZERO(&totalfds);

	FD_SET(server_fd, &totalfds);

	while(true) {

		readfds = writefds = totalfds;

		int	res = select(FD_SETSIZE, &readfds, &writefds, NULL, NULL);

		if (res < 0) throw std::runtime_error("Systemcall `select()` failed.");

		if (!res) continue ;

		if (FD_ISSET(server_fd, &readfds)) acceptIncomingConnection(this->clients, totalfds, server_fd);

		std::vector<int> clientsReadyToBeRemoved;

		for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); it++)
		{
			if(it->second.step == C_CLOSE_CONNECTION) clientsReadyToBeRemoved.push_back(it->first);
			else if (FD_ISSET(it->first, &readfds))  // function of handling multiple file descriptors or sockets
				handleReadRequest(it->second);
		}

		clearClients(clientsReadyToBeRemoved, totalfds);
	}
}
