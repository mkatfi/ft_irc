#include "../../headers/server.hpp"
#include <cstddef>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

void Server::createChannel(std::string &channel, Client &t, request& p)
{
    channels[channel] = new Channel(channel, &t);
    channels[channel]->changeTopic = true;
    channels[channel]->admin = &t;
    channels[channel]->admins.push_back(&t);
    t._channel.push_back(channel);
    send_message(t.socket_fd,  ":" + t.nickName + "!" + t.serverName + "@localhost JOIN :"+ channel + "\r\n");
    send_message(t.socket_fd, RPL_TOPIC(t.nickName, p.arg[0], channels[p.arg[0]]->get_topic()));
    send_message(t.socket_fd, ": 333 " + t.nickName  + " " + channels[p.arg[0]]->_name + " " + channels[p.arg[0]]->admin->nickName + "\r\n");
    send_message(t.socket_fd , ":localhost 353 " + t.nickName + " = " + channel + " :@" + t.nickName + "\r\n"); 
    send_message(t.socket_fd, ":localhost 366 " + t.nickName + " " + channel + " :End of /NAMES list.\r\n");
}

void Server::joinChannel(std::string &channel, Client &t, request& p)
{
    std::vector<Client*>::iterator it;

    channels[channel]->_members.push_back(&t);
    t._channel.push_back(channel);

    channels[channel]->member_str = ":localhost 353 " + t.nickName + " = " + channel + " :@" + channels[channel]->admin->nickName + " ";
    for (it = channels[channel]->_members.begin(); it != channels[channel]->_members.end(); ++it)
    {
        if ((*it)->nickName == channels[channel]->admin->nickName)
            continue;
        else
            channels[channel]->member_str += (*it)->nickName + " ";
    }
    channels[channel]->member_str += "\r\n";
    send_message(t.socket_fd,  ":" + t.nickName + "!" + t.serverName + "@localhost JOIN :"+ channel + "\r\n");
    send_message(t.socket_fd, RPL_TOPIC(t.nickName, p.arg[0], channels[p.arg[0]]->get_topic()));
    send_message(t.socket_fd, ": 333 " + t.nickName  + " " + channels[p.arg[0]]->_name + " " + channels[p.arg[0]]->admin->nickName + "\r\n");
    send_message(t.socket_fd , channels[channel]->member_str);
    send_message(t.socket_fd, ":localhost 366 " + t.nickName + " " + channel + " :End of /NAMES list.\r\n");
}

bool Server::checkLimits(request& req, int *user)
{
    int size = channels[req.arg[0]]->maxsize;

    if (*user > size)
        return true;
    else
        return false;
}

std::string Server::join(Client &client, request &p)
{
    static int user_size = 0;

    user_size += 1;
    std::map<std::string, Channel *>::iterator it = channels.find(p.arg[0]);
    if (p.arg.empty() || p.arg.size() < 1)
    {
        send_message(client.socket_fd, ERR_NEEDMOREPARAMS(p.cmd));
        return "";
    }
    else if (p.arg[0][0] != '#')
    {
        send_message(client.socket_fd, ERR_NOSUCHCHANNEL(p.arg[0]));
        return "";
    }
    else if ((std::find(client._channel.begin(), client._channel.end(), p.arg[0])) != client._channel.end())
    {
        send_message(client.socket_fd, RPL_ALREAYREGISTRED(client.nickName, p.arg[0]));
        return "";
    }
    else if (it == channels.end())
    {
        std::cout << "creating channel " << std::endl;
        createChannel(p.arg[0], client, p);
        return "";
    }
    else if ((*it).second->isLimit == true && checkLimits(p, &user_size) == true)
    {
        (*it).second->inviteOnly = false;
        send_message(client.socket_fd, ERR_CHANNELISFULL(client.nickName, p.arg[0]));
        return "";
    }
    else if ((*it).second->inviteOnly == true)
    {
        std::cout << "invite only condition" << std::endl;
        send_message(client.socket_fd, ERR_INVITEONLYCHAN(client.nickName, p.arg[0]));
        return "";
    }
    else
    {
        if ((*it).second->inviteOnly == false && channels[p.arg[0]]->invited == client.nickName)
        {
            (*it).second->inviteOnly = true;
            if (joinClient(client, p, it) == 1)
                return "";
        }
        else if ((*it).second->inviteOnly == false && channels[p.arg[0]]->invited != client.nickName && !channels[p.arg[0]]->invited.empty())
        {
            send_message(client.socket_fd, ERR_INVITEONLYCHAN(client.nickName, p.arg[0]));
            return "";
        }
        else
        {
            if (joinClient(client, p, it) == 1)
                return "";
        }
    }
    return "";
}


int Server::joinClient(Client& client, request& p, std::map<std::string,Channel*>::iterator it)
{
    (void)it;
    (void)p;
    if (channels[p.arg[0]]->hasPassword == true)
    {
        if (p.arg.size() != 2)
        {
            send_message(client.socket_fd, ERR_BADCHANNELKEY(p.arg[0]));
            return 1;
        }
        else if (p.arg[1] != channels[p.arg[0]]->_password)
        {
            send_message(client.socket_fd, ERR_INVALIDKEY(p.arg[0]));
            return 1;
        }
        else if (p.arg[1] == channels[p.arg[0]]->_password)
        {
            if (!channels[p.arg[0]]->_topic.empty())
            {
                joinChannel(p.arg[0], client, p);
                send_just_member(RPL_JOINMSG(client.nickName, client.userName, p.arg[0]), p.arg[0]);
            }
            else
            {
                joinChannel(p.arg[0], client, p);
                send_just_member(RPL_JOINMSG(client.nickName, client.userName, p.arg[0]), p.arg[0]);
            }
        }
    }
    else
    {
        if (!channels[p.arg[0]]->_topic.empty())
        {
            joinChannel(p.arg[0], client,p);
            send_just_member(RPL_JOINMSG(client.nickName, client.userName, p.arg[0]), p.arg[0]);
        }
        else
        {
            joinChannel(p.arg[0], client,p);
            send_just_member(RPL_JOINMSG(client.nickName, client.userName, p.arg[0]), p.arg[0]);
        }
    }
    return 0;
}

void Server::sendMSGToChannel(Client& cli, request& req)
{
    std::string msg;
    std::string str;
    bool etat = false;
    std::vector<Client*>::iterator it;

    for(it = channels[req.arg[0]]->_members.begin(); it != channels[req.arg[0]]->_members.end(); ++it)
    {
        if ((*it)->socket_fd == cli.socket_fd)
        {
            etat = 1;
            break;
        }
    }
    if (etat)
    {
        for (size_t i = 1; i < req.arg.size(); i++)
            str += req.arg[i] + " ";

        str.erase(0,1);
        msg = ":" + cli.nickName + " PRIVMSG " + req.arg[0] + " :" + str + "\r\n";
        for(it = channels[req.arg[0]]->_members.begin(); it != channels[req.arg[0]]->_members.end(); ++it)
        {
            if ((*it)->socket_fd == cli.socket_fd)
                continue;
            else
                send((*it)->socket_fd, msg.c_str(), msg.size(), 0);
        }
    }
}

// kick <channel> <nickname>

std::string Server::kick(Client &client, request &p)
{
    bool etat = false;
    std::cout << channels[p.arg[0]]->inviteOnly << std::endl;
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
    else
    {
        std::vector<Client*> ::iterator it1;
        for (it1 = channels[p.arg[0]]->admins.begin(); it1 != channels[p.arg[0]]->admins.end(); ++it1)
        {
            if ((*it1)->nickName == client.nickName)
            {
                etat = true;
                bool isExist = 0;
                if (client.nickName == p.arg[1])
                {
                    send_message(client.socket_fd, ERR_KICKYOUSELF(client.nickName));
                    continue;
                }
                std::vector<Client *>::iterator it = channels[p.arg[0]]->_members.begin();
                for (; it != channels[p.arg[0]]->_members.end(); ++it)
                {
                    if (p.arg[1] == (*it)->nickName)
                    {
                        channels[p.arg[0]]->inviteOnly = false;
                        send_message((*it)->socket_fd, ": " + (*it1)->nickName + " KICK " + p.arg[0] + " " + p.arg[1] + " :You have been kicked\r\n");
                        (*it)->_channel.erase(std::find((*it)->_channel.begin(), (*it)->_channel.end(), p.arg[0]));
                        channels[p.arg[0]]->_members.erase(it);
                        send_just_member(KICKUSER(client.nickName, client.userName, p.arg[0], p.arg[1]), p.arg[0]);
                        isExist = 1;
                        break;
                    }
                }
                if (!isExist)
                    send_message(client.socket_fd, ERR_NOSUCHNICK(p.arg[1]));
            }
        }
        if (!etat)
        {
            send_message(client.socket_fd, ERR_CHANOPRIVSNEEDED(p.arg[0]));
            return ("");
        }
    }
    return ("");
}

// invite <nickname> <channel>
std::string Server::invite(Client &client, request &p)
{
    if (p.arg.size() < 2)
    {
        send_message(client.socket_fd, ERR_NEEDMOREPARAMS(p.cmd));
        return "";
    }
    else if (std::find(client._channel.begin(), client._channel.end(), p.arg[1]) == client._channel.end())
    {
        send_message(client.socket_fd, ERR_NOSUCHCHANNEL(p.arg[1]));
        return "";
    }
    else if (client.nickName == p.arg[0])
    {
        send_message(client.socket_fd, ERR_USERONCHANNEL(client.nickName, p.arg[1]));
        return "";
    }
    else if (isAdmin(p, client) == false)
    {
        send_message(client.socket_fd, ERR_CHANOPRIVSNEEDED(p.arg[0]));
        return "";
    }
    else {
    
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
        if (isMember)
        {
            send_message(client.socket_fd, RPL_ALREAYREGISTRED(client.nickName, p.arg[1]));
            return ("");
        }
        if (!isExist)
        {
            send_message(client.socket_fd, ERR_NOSUCHNICK(client.nickName));
            return ("");
        }
        
        std::map<std::string, Channel*>::iterator it;
        for (it = channels.begin(); it != channels.end(); ++it)
        {
            if (channels[p.arg[1]])
            {
                channels[p.arg[1]] ->inviteOnly = false;
                channels[p.arg[1]] ->hasPassword = false;
                channels[p.arg[1]]->invited = p.arg[0];
                send_message((it_1)->second.socket_fd, RPL_INVITING(client.nickName, p.arg[1], client.userName));
            } 
        }
    }
    return ("");
}

std::string Server::Topic(Client &client, request &p)
{

    if (p.arg.size() < 2)
    {
        send_message(client.socket_fd, ERR_NEEDMOREPARAMS(p.cmd));
        return ("");
    }
    else if (std::find(client._channel.begin(), client._channel.end(), p.arg[0]) == client._channel.end())
    {
        send_message(client.socket_fd, ERR_NOSUCHCHANNEL(p.arg[0]));
        return ("");
    }
    else if (is_admin(p, client) == false && channels[p.arg[0]]->changeTopic == true)
    {
        send_message(client.socket_fd, ERR_CHANOPRIVSNEEDED(p.arg[0]));
        return "";
    }
    else {
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
                send_message(client.socket_fd, ERR_NOTONCHANNEL(client.nickName, p.arg[0]));
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
                send_all_member(client.socket_fd, RPL_TOPIC(client.nickName, p.arg[0], channels[p.arg[0]]->get_topic()));
                return ("");
            }
    }
    return ("");
}

bool Server::is_admin(request& req, Client& cli)
{
    std::vector<Client*>::iterator it;
    bool etat = false;
    
    for (it = channels[req.arg[0]]->admins.begin(); it !=  channels[req.arg[0]]->admins.end(); ++it)
    {
        if ((*it)->nickName == cli.nickName)
        {
            etat = true;
            break;
        }
    }
    if (!etat)
        return false;
    return true;
}

void Server::Mode(Client& cli, request& req)
{
    std::vector<std::string>::iterator it = std::find(cli._channel.begin(), cli._channel.end(), req.arg[0]);
    
    if (req.arg.size() < 2)
        return;
    if (req.arg[0] == cli.nickName && req.arg[1] == "+i")
    {
        send_message(cli.socket_fd , "MODE " + req.arg[0] + " +i\r\n");
        return;
    }
    if (is_admin(req, cli) == false)
    {
        send_message(cli.socket_fd, ERR_CHANOPRIVSNEEDED(req.arg[0]));
        return;
    }
    else
    {
        for (size_t i = 0; i < channels.size(); i++)
        {
            if (channels[req.arg[0]])
            {
                if (it != cli._channel.end() && req.arg[1] == "+i")
                {
                    std::cout << "mode i actived " << std::endl;
                    channels[req.arg[0]]->inviteOnly = true;
                    send_message(cli.socket_fd , ":" + cli.nickName + " MODE " + req.arg[0] + " +i\r\n");
                }
                else if (it != cli._channel.end() && req.arg[1] == "-i")
                {
                    std::cout << "mode i desactived " << std::endl;
                    channels[req.arg[0]]->inviteOnly = false;
                    send_message(cli.socket_fd , ":" + cli.nickName + " MODE " + req.arg[0] + " -i\r\n");
                }
                else if (it != cli._channel.end() && req.arg[1] == "+t")
                {
                    channels[req.arg[0]]->changeTopic = true;
                    send_message(cli.socket_fd , ":" + cli.nickName + " MODE " + req.arg[0] + " +t\r\n");
                }
                else if (it != cli._channel.end() && req.arg[1] == "-t")
                {
                    channels[req.arg[0]]->changeTopic = false;
                    send_message(cli.socket_fd , ":" + cli.nickName + " MODE " + req.arg[0] + " -t\r\n");
                }
                else if (it != cli._channel.end() && req.arg[1] == "+k")
                {
                    channels[req.arg[0]]->hasPassword = true;
                    channels[req.arg[0]]->_password = req.arg[2];
                    send_message(cli.socket_fd , ":" + cli.nickName + " MODE " + req.arg[0] + " +k " + req.arg[2] + "\r\n");
                }
                else if (it != cli._channel.end() && req.arg[1] == "-k")
                {
                    channels[req.arg[0]]->hasPassword = false;
                    send_message(cli.socket_fd , ":" + cli.nickName + " MODE " + req.arg[0] + " -k\r\n");
                }
                else if (it != cli._channel.end() && req.arg[1] == "+o")
                {
                    std::vector<Client*> ::iterator it;
                    for (it = channels[req.arg[0]]->_members.begin(); it != channels[req.arg[0]]->_members.end(); ++it)
                    {
                        if ((*it)->nickName == req.arg[2])
                        {
                            std::cout << "+O : " << (*it)->nickName << std::endl;
                            (*it)->nickName = "@" + (*it)->nickName;
                            channels[req.arg[0]]->admins.push_back(*it);
                            send_message(cli.socket_fd , ":" + cli.nickName + " MODE " + req.arg[0] + " +o " + req.arg[2] + "\r\n");
                            channels[req.arg[0]]->member_str = ":localhost 353 " + cli.nickName + " = " + req.arg[0] + " :" + (*it)->nickName + "\r\n";
                            return;
                        }
                    }
                }
                else if (it != cli._channel.end() && req.arg[1] == "-o")
                {
                    std::vector<Client*> ::iterator it;
                    for (it = channels[req.arg[0]]->admins.begin(); it != channels[req.arg[0]]->admins.end(); ++it)
                    {
                        if ("@" + req.arg[2] == (*it)->nickName)
                        {
                            (*it)->nickName.erase(0,1);
                            std::cout << "-O : " << (*it)->nickName << std::endl;
                            send_message(cli.socket_fd , ":" + cli.nickName + " MODE " + req.arg[0] + " -o " + req.arg[2] + "\r\n");
                            channels[req.arg[0]]->admins.erase(it);
                            return;
                        }
                    }
                }
                else if (it != cli._channel.end() && req.arg[1] == "+l")
                {
                    channels[req.arg[0]]->maxsize = std::atoi(req.arg[2].c_str());
                    channels[req.arg[0]]->isLimit = true;
                    send_message(cli.socket_fd , ":" + cli.nickName + " MODE " + req.arg[0] + " +l " + req.arg[2] + "\r\n");
                }
                else if (it != cli._channel.end() && req.arg[1] == "-l")
                {
                    channels[req.arg[0]]->isLimit = false;
                    send_message(cli.socket_fd , ":" + cli.nickName + " MODE " + req.arg[0] + " -l " + req.arg[2] + "\r\n");
                }
                else
                    return ;
            }
        }
    }
}
