#include "../../headers/server.hpp"
#include <iostream>
#include <algorithm>
#include <vector>

void Server::createChannel(std::string &channel, Client &t)
{
    channels[channel] = new Channel(channel, &t);
    // channels[channel]->update_onlinemembers();
    channels[channel]->admin = &t;
    // clients[fd] = t;// useless
    t._channel.push_back(channel);
    // for (auto it = channels.begin(); it != channels.end(); ++it)
    // {
    //     std::cout << "channel : " << it->first << " client : " << it->second->admin->socket_fd << std::endl;
    // }
}

void Server::joinChannel(std::string &channel, Client &t)
{
    channels[channel]->_members.push_back(&t);
    t._channel.push_back(channel);
    // channels[channel]->setop(client[fd]);
    // std::cout<< "<< " << t.socket_fd << " >>\n";

    //  std::vector<Client *>:: iterator it = channels[channel]->_members.begin();
    //  for(; it != channels[channel]->_members.end(); it++)
    //     std::cout<< (*it)->socket_fd << std::endl;
}

std::string Server::join(Client &client, request &p)
{

    std::map<std::string, Channel *>::iterator it = channels.find(p.arg[0]);
    if (p.arg.empty() || p.arg.size() < 1)
    {
        send_message(client.socket_fd, ":localhost 461 " + client.nickName + " join :Not enough parameters\r\n");
        return "";
    }
    else if (p.arg[0][0] != '#')
    {
        send_message(client.socket_fd, ":localhost 461 " + client.nickName + " join : Invalid channel name\r\n");
    }
    else if ((std::find(client._channel.begin(), client._channel.end(), p.arg[0])) != client._channel.end())
    {
        send_message(client.socket_fd, ":localhost 461 " + client.nickName + " join : You are already in " + p.arg[0] + "\r\n");
    }
    else if (it == channels.end())
    {
        std::cout << "creating channel " << std::endl;
        createChannel(p.arg[0], client);
        send_message(client.socket_fd, RPL_JOINMSG(client.hostName, p.arg[0]));
        send_message(client.socket_fd, ":localhost 461 " + client.nickName + " join : the channel is created " + p.arg[0] + "\r\n");
        // join_message(p.arg[0], client.socket_fd);
    }
    else if (channels[p.arg[0]]->admin->inviteOnly == true)
    {
        std::cout << "arg : " << p.arg[0] << " " << channels[p.arg[0]]->admin->inviteOnly << std::endl;
        send_message(client.socket_fd, ":localhost 473 " + client.nickName + " " + p.arg[0] + " :Cannot join channel (+i)\r\n");
        return "";
    }
    else
    {
        if (!channels[p.arg[0]]->_topic.empty())
        {
            joinChannel(p.arg[0], client);
            send_message(client.socket_fd, RPL_TOPIC(client.nickName, channels[p.arg[0]]->_name , channels[p.arg[0]]->get_topic()));
            send_message(client.socket_fd, ": 333 " + client.nickName  + " " + channels[p.arg[0]]->_name + " " + channels[p.arg[0]]->admin->nickName + "\r\n");
        }
        else
        {
            std::cout << "hello 1 : " << channels[p.arg[0]]->get_topic() << std::endl;
            send_message(client.socket_fd, ":localhost 461 " + client.nickName + " join : You are now a memeber in " + p.arg[0] + "\r\n");
            joinChannel(p.arg[0], client);
            // join_message(p.arg[0], client.socket_fd);
        }
    }
    return "";
}
std::string Server::join_message(std::string chanel, int fd)
{
    std::string response = "";
    response += clients[fd].nickName + " has joined " + chanel + "\n";
    response += "Welcome to " + chanel + " " + this->clients[fd].nickName + "\n";
    response += "Topic for " + chanel + " is " + this->channels[chanel]->get_topic() + "\n";
    response += "Channel created on " + this->channels[chanel]->admin->nickName + "\n";
    // response += "There are " + this->channels[chanel]->intToString(this->channels[chanel]->getonlinemembers()) + " users on this channel\n";
    send_all_member(fd, response);
    return ("");
}

void Server::sendMSGToChannel(Client& cli, request& req)
{
    std::string msg;
    std::string str;
    std::vector<Client*>::iterator it;

    if (channels[req.arg[0]]->admin->inviteOnly == false)
    {
        for (size_t i = 1; i < req.arg.size(); i++)
            str += req.arg[i] + " ";

        for(it = channels[req.arg[0]]->_members.begin(); it != channels[req.arg[0]]->_members.end(); it++)
        {
            if ((*it)->socket_fd == cli.socket_fd)
                continue;
            else
            {
                msg = ":" + cli.nickName + " PRIVMSG " + req.arg[0] + " :" + str + "\r\n";
                send((*it)->socket_fd, msg.c_str(), msg.size(), 0);
            }
        }
    }
}

// kick <channel> <nickname>
std::string Server::kick(Client &client, request &p)
{
    if (p.arg.size() < 2 || p.arg.empty())
    {
        send_message(client.socket_fd, ERR_NEEDMOREPARAMS(p.cmd));
        // send_message(client.socket_fd, ":localhost 461 " + client.nickName + " kick : Not enough arguments!\r\n");
        return ("");
    }
    else if (std::find(client._channel.begin(), client._channel.end(), p.arg[0]) == client._channel.end())
    {
        send_message(client.socket_fd, ERR_NOSUCHCHANNEL(p.arg[0]));
        // send_message(client.socket_fd, ":localhost 461 " + client.nickName + " kick : You are not a member in this channel\r\n");
        return ("");
    }
    else if (client.socket_fd != channels[p.arg[0]]->admin->socket_fd)
    {
        send_message(client.socket_fd, ":localhost 461 " + client.nickName + " kick : You are not the admin of the channel\r\n");
        return ("");
    }
    else
    {
        bool isExist = 0;
        for (size_t i = 1; i < p.arg.size(); i++)
        {
            // ta loop 3la ga3 users
            if (client.nickName == p.arg[i])
            {
                // lakan hadak clinet howa li m3ayt 3la rasso
                send_message(client.socket_fd, ":localhost 461 " + client.nickName + " kick : You can not kick yourself\r\n");
                // i++;
                continue;
            }
            std::vector<Client *>::iterator it = channels[p.arg[0]]->_members.begin();
            // tan9alb 3la kila user fi channel
            for (; it != channels[p.arg[0]]->_members.end(); ++it)
            {
                // std::cout << p.arg[i] << "==" << (*it)->nickName << ">>\n";
                if (p.arg[i] == (*it)->nickName)
                {
                    // remove client mn channael
                    (*it)->_channel.erase(std::find((*it)->_channel.begin(), (*it)->_channel.end(), p.arg[0]));
                    send_message((*it)->socket_fd, ":localhost 461 " + p.arg[i] + " You have been kicked !\r\n");
                    // remove client mn member
                    channels[p.arg[0]]->_members.erase(it);
                    isExist = 1;
                    break;
                }
            }
            if (!isExist)
            {
                send_message(client.socket_fd, ":localhost 461 " + p.arg[i] + " makaynch dak khona !\r\n");
                // i++;
            }
        }
    }
    return ("");
}
// invite <nickname> <channel>
std::string Server::invite(Client &client, request &p)
{

    if (p.arg.size() < 2 || p.arg.empty())
    {
        send_message(client.socket_fd, ERR_NEEDMOREPARAMS(p.cmd));
        // send_message(client.socket_fd, ":localhost 461 " + client.nickName + " invite : Not enough arguments!\r\n");
        return "";
    }
    else if (std::find(client._channel.begin(), client._channel.end(), p.arg[1]) == client._channel.end())
    {
        send_message(client.socket_fd, ERR_NOSUCHCHANNEL(p.arg[0]));
        // send_message(client.socket_fd, ":localhost 461 " + client.nickName + " invite : You are not a member in this channel\r\n");
        return "";
    }
    else
    {
        if (client.nickName == p.arg[0])
        {
            send_message(client.socket_fd, ":localhost 461 " + client.nickName + " invite : You can not invite yourself\r\n");
        }
        bool isMember = 0;
        std::vector<Client *>::iterator itt;  // wax khona member
        std::map<int, Client>::iterator it_1; // khona li ghadi t invite
        for (itt = channels[p.arg[1]]->_members.begin(); channels[p.arg[1]]->_members.end() != itt; itt++)
        {
            if ((*itt)->nickName == client.nickName)
            {
                isMember = 1;
                break;
            }
        }
        bool isExist = 0;
        for (it_1 = clients.begin(); it_1 != clients.end(); ++it_1)
        {
            if ((*it_1).second.nickName == p.arg[0])
            {
                isExist = 1;
                break;
            }
        }
        if (!isExist)
        {
            send_message(client.socket_fd, " :localhost 461 " + p.arg[0] + " makaynch dak khona !\r\n");
            return ("");
        }
        channels[p.arg[1]]->admin->inviteOnly = false;
        channels[p.arg[1]]->_members.push_back(&it_1->second);
        it_1->second._channel.push_back(p.arg[1]);
        send_message((it_1)->second.socket_fd, ":localhost 461 " + p.arg[0] + " invite in channel!\r\n");
    }
    //******************************************************************************
    // std::vector<Client *>::iterator it_2 = channels[p.arg[1]]->_members.begin();
    // while (it_2 != channels[p.arg[1]]->_members.end())
    // {
    //     std::cout << (*it_2)->nickName << ">>\n";
    //     it_2++;
    // }
    //******************************************************************************
    return ("");
}
//  /Topic <channel> [<topic>]

std::string Server::Topic(Client &client, request &p)
{

    if (p.arg.size() < 2 || p.arg.empty())
    {
        send_message(client.socket_fd, ERR_NEEDMOREPARAMS(p.cmd));
        return ("");
    }
    else if (std::find(client._channel.begin(), client._channel.end(), p.arg[0]) == client._channel.end())
    {
        send_message(client.socket_fd, ERR_NOSUCHCHANNEL(p.arg[0]));
        return ("");
    }
    else if (this->channels[p.arg[0]]->admin->nickName != client.nickName)
    {
        send_message(client.socket_fd, ERR_CHANOPRIVSNEEDED( p.arg[0]));
    }
    else
    {
        bool isMember = 0;
        std::vector<Client *>::iterator itt; // wax khona member
        for (itt = channels[p.arg[0]]->_members.begin(); channels[p.arg[0]]->_members.end() != itt; itt++)
        {
            if ((*itt)->nickName == client.nickName)
            {
                isMember = 1;
                break;
            }
        }
        if (!isMember)
        {
            send_message(client.socket_fd, " :localhost 461 " + p.arg[0] + " makaynch dak khona !\r\n");
            return ("");
        }
        else
        {
            std::string topic;
            for (size_t i = 1; i < p.arg.size(); i++)
            {
                topic += p.arg[i];
                topic += " ";
            }
            topic.erase(0, 1);
            channels[p.arg[0]]->set_topic(topic);
            send_message(client.socket_fd, RPL_TOPIC(client.nickName, channels[p.arg[0]]->_name , topic));
            send_message(client.socket_fd, ": 333 " + client.nickName + " " + channels[p.arg[0]]->_name + " " + channels[p.arg[0]]->admin->nickName + "\r\n");
            send_all_member(client.socket_fd, RPL_TOPIC(client.nickName, channels[p.arg[0]]->_name, topic));
            return ("");
        }
    }
    return ("");
}

void Server::Mode(Client& cli, request& req)
{
    std::cout << "arg : " << req.arg[0] << " mode : " << req.arg[1] << std::endl;

    if (req.arg[0] == cli.nickName && req.arg[1] == "+i")
    {
        send_message(cli.socket_fd , "MODE " + req.arg[0] + " +i\r\n");
    }
    else if (std::find(cli._channel.begin(), cli._channel.end(), req.arg[0]) != cli._channel.end() && req.arg[1] == "+i")
    {
        std::cout << "channel find : " << req.arg[0] << std::endl;
        channels[req.arg[0]]->admin->inviteOnly = true;
        std::cout << "onlyinvite : "<< cli.inviteOnly << std::endl;
        send_message(cli.socket_fd , ":" + cli.nickName + " MODE " + req.arg[0] + " +i\r\n");
    }
    else if (std::find(cli._channel.begin(), cli._channel.end(), req.arg[0]) != cli._channel.end() && req.arg[1] == "-i")
    {
        std::cout << "channel find : " << req.arg[0] << std::endl;
        channels[req.arg[0]]->admin->inviteOnly = false;
        std::cout << "onlyinvite : "<< cli.inviteOnly << std::endl;
        send_message(cli.socket_fd , ":" + cli.nickName + " MODE " + req.arg[0] + " -i\r\n");
    }
    else if (std::find(cli._channel.begin(), cli._channel.end(), req.arg[0]) != cli._channel.end() && req.arg[1] == "+t")
    {
        std::cout << "channel find : " << req.arg[0] << std::endl;
        // channels[req.arg[0]]->admin->inviteOnly = true;
        // std::cout << "onlyinvite : "<< cli.inviteOnly << std::endl;
        send_message(cli.socket_fd , ":" + cli.nickName + " MODE " + req.arg[0] + " +t\r\n");
    }
    else if (std::find(cli._channel.begin(), cli._channel.end(), req.arg[0]) != cli._channel.end() && req.arg[1] == "-t")
    {
        std::cout << "channel find : " << req.arg[0] << std::endl;
        // channels[req.arg[0]]->admin->inviteOnly = true;
        // std::cout << "onlyinvite : "<< cli.inviteOnly << std::endl;
        send_message(cli.socket_fd , ":" + cli.nickName + " MODE " + req.arg[0] + " -t\r\n");
    }
    else
        return ;
}
