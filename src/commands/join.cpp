#include "../../headers/server.hpp"
#include <iostream>
#include <algorithm>

void Server::createChannel(std::string &channel, int fd, Client &t)
{
    channels[channel] = new Channel(channel, t);
    channels[channel]->update_onlinemembers();
    channels[channel]->admin = &t;
    clients[fd] = t;
    t._channel.push_back(channel);

    // std::map<std::string, Channel *>::iterator it = channels.begin();
    // while (it != channels.end())
    // {
    //     std::cout << it->first << "<>" << it->second->_name << ">>\n";
    //     it++;
    // }
    // channels[channel]->setop(client[fd]);
}

void Server::joinChannel(std::string &channel, int fd, Client &t)
{
    Channel *newch = channels[channel];
    channels[channel] = newch;
    channels[channel]->update_onlinemembers();
    clients[fd] = t;
    t._channel.push_back(channel);
    // channels[channel]->setop(client[fd]);
}


std::string Server::join(Client &client, request &p)
{
    if (p.arg.empty())
    {
        send_message(client.socket_fd, ":localhost 461 " + client.nickName + " join :Not enough parameters\r\n");
        return "";
    }

    if (p.arg.size() < 1)
    {
        send_message(client.socket_fd, ":localhost 461 " + client.nickName + " join : Not enough arguments\r\n");
        return "";
    }

    int i = 0;
    while (i < (int)p.arg.size())
    {
        std::string channel = p.arg[i];
        if (channel[0] != '#')
        {
            send_message(client.socket_fd, ":localhost 461 " + client.nickName + " join : Invalid channel name\r\n");
            continue;
        }

        if (channel.length() > 20)
        {
            send_message(client.socket_fd, ":localhost 461 " + client.nickName + " join : Channel name too long\r\n");
            continue;
        }

        if (channels.count(p.arg[i]))
        {
            std::cout << "chanee sla d as \n";
            if ((std::find(client._channel.begin(), client._channel.end(), p.arg[i])) != client._channel.end())
            {
                send_message(client.socket_fd, ":localhost 461 " + client.nickName + " join : You are already in " + channel + "\r\n");
                ++i;
                continue;
            } else {
                std::cout << "channel kaina walkin machi member !\n";
                send_message(client.socket_fd, ":localhost 461 " + client.nickName + " join : You are now a memeber in " + channel + "\r\n");

                joinChannel(p.arg[i], client.socket_fd, client);
                ++i;
                continue;
            }
        }

        std::map<std::string, Channel *>::iterator it = channels.find(p.arg[i]);
        if (it == channels.end())
        {
            createChannel(p.arg[i], client.socket_fd, client);
            send_message(client.socket_fd, ":localhost 461 " + client.nickName + " join : the channel is created " + channel + "\r\n");
        }
        i++;
    }
    return "";
}

// void Server::join_message(std::string channel, int fd)
// {
//     std::string response = "";
//     response += this->client[fd]->getnickname() + " has joined " + channel + "\n";
//     response += "Welcome to " + channel + " " + this->client[fd]->getnickname() + "\n";
//     response += "Topic for " + channel + " is " + this->_channels[channel]->getTopic() + "\n";
//     response += "Channel created on " + this->_channels[channel]->getCreationDate() + "\n";
//     response += "There are " + this->_channels[channel]->intToString(this->_channels[channel]->getonlinemembers()) + " users on this channel\n";
//     send_to_all_members(channel, response);
// }
