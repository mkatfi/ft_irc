/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   authentication_cmd.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchaguer <hchaguer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 16:47:59 by hchaguer          #+#    #+#             */
/*   Updated: 2024/05/17 23:49:20 by hchaguer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/server.hpp"


int Server::pass(Client& client, request &p)
{
    if (p.arg.empty())
    {
        send_message(client.socket_fd, ERR_NEEDMOREPARAMS(p.cmd));
        client.count = 0;
    }
    else if (p.arg.size() != 1)
    {
        send_message(client.socket_fd, ERR_NEEDMOREPARAMS(p.cmd));
        client.count = 0;
    }
    else if (p.arg[0] != this->password)
    {
        send_message(client.socket_fd, ERR_PASSWDMISMATCH());
        client.count = 0;
    }
    else
    {
        send_message(client.socket_fd, "please enter the nickname\r\n");
    }
    return 0;
}


void Server::Nick(Client& cli, request &p)
{
    std::string nick = p.cmd;
    bool etat = true;
    std::map<int,Client>::iterator it;
    
    if (p.arg.empty())
    {
        send_message(cli.socket_fd, ERR_NONICKNAMEGIVEN());
        cli.count = 1;
    }
    else
    {
        for (it = clients.begin(); it != clients.end(); ++it)
        {
            if (it->second.nickName == p.arg[0])
            {
                etat = false;
                cli.count = 1;
                send_message(cli.socket_fd, ERR_NICKNAMEINUSE(p.arg[0]));
            }
        }
        if (etat == true)
        {
            cli.nickName = p.arg[0];
            clients[cli.socket_fd] = cli;
            send_message(cli.socket_fd, "please enter the user\r\n");
        }
    }
}

void Server::user(Client& cli, request &p)
{
    std::string user = p.cmd;
    if (p.arg.size() != 4)
    {
        cli.count = 2;
        send_message(cli.socket_fd, ERR_NEEDMOREPARAMS(user));
    }
    else if (p.arg.size() == 4)
    {
        cli.userName = p.arg[0];
        cli.hostName = p.arg[1];
        cli.serverName = p.arg[2];
        if (p.arg[3].find(":") != std::string::npos)
            cli.realName = p.arg[3];
        else
        {
            cli.count = 2;
            send_message(cli.socket_fd, ":localhost 462 " + cli.nickName + " :You may not reregister\r\n");
        }
    }
}
