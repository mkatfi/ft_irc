/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   authentication_cmd.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkatfi <mkatfi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 16:47:59 by hchaguer          #+#    #+#             */
/*   Updated: 2024/04/29 18:44:08 by mkatfi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/server.hpp"


int Server::pass(Client& client, request &p, int *count)
{
    if (p.arg.empty())
    {
        send_message(client.socket_fd, ERR_NEEDMOREPARAMS(p.cmd));
        *count = 0;
    }
    else if (p.arg.size() != 1)
    {
        send_message(client.socket_fd, ERR_NEEDMOREPARAMS(p.cmd));
        *count = 0;
    }
    else if (p.arg[0] != this->password)
    {
        send_message(client.socket_fd, ERR_PASSWDMISMATCH());
        *count = 0;
    }
    else
    {
        std::cout << "|" << p.arg[0] << "| |" << this->password << "|\n";
        std::cout << "hello from pass cmd " << std::endl;
        send_message(client.socket_fd, ":localhost 462 " + client.nickName + " :You may not reregister\r\n");
    }
    return 0;
}


void Server::Nick(Client& cli, request &p, int *count)
{
    std::string nick = p.cmd;
    bool etat = true;
    std::map<int,Client>::iterator it;

    if (p.arg.empty())
    {
        send_message(cli.socket_fd, ERR_NONICKNAMEGIVEN());
        *count = 1;
    }
    else
    {
        for (it = clients.begin(); it != clients.end(); ++it)
        {
            if (it->second.nickName == p.arg[0])
            {
                etat = false;
                send_message(cli.socket_fd, ERR_NICKNAMEINUSE(p.arg[0]));
            }
        }
        if (etat == true)
        {
            cli.nickName = p.arg[0];
            clients[cli.socket_fd] = cli;
            send_message(cli.socket_fd, ":localhost 462 " + cli.nickName + " :You may not reregister\r\n");
        }
    }
}

void Server::user(Client& cli, request &p, int *count)
{
    std::string user = p.cmd;
    if (p.arg.size() != 4)
    {
        *count = 2;
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
            *count = 2;
            send_message(cli.socket_fd, ":localhost 462 " + cli.nickName + " :You may not reregister\r\n");
        }
    }
}
