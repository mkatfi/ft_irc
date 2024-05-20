/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   host_server.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchaguer <hchaguer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 16:34:44 by oryadi            #+#    #+#             */
/*   Updated: 2024/05/16 22:57:40 by hchaguer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/server.hpp"

void	Server::hostServer()
{
	sockaddr_in address; //  Structures for handling internet addresses
	
	address.sin_family = AF_INET; // tells the system that the socket address in myaddr will be an IPv4 address

	address.sin_port = htons(this->port); // converting the port to network byte so it can be understood by other machines on the network

	address.sin_addr.s_addr = htonl(INADDR_ANY); 

	
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw std::runtime_error("Cannot create a socket.");

	if (bind(server_fd,(struct sockaddr *)&address,sizeof(address)) < 0)
		throw std::runtime_error("Cannot bind the socket.");

	if (listen(server_fd, SOMAXCONN) < 0)
		throw std::runtime_error("Cannot listen to socket.");
	
	std::cout << "irc is listening at 0.0.0.0:" << port << std::endl;

	
}
