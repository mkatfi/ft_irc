#include "../../headers/server.hpp"
#include <string>

void Server::send_message(int sockfd, const std::string& message)
{
    if (send(sockfd, message.c_str(), message.size(), 0) == -1)
        std::cerr << "send() faild" << std::endl;
}

void Server::send_all_member(int sockfd, const std::string &message)
{
    std::map<int, Client>::iterator it = clients.begin();
    for (; it != clients.end();it++)
    {
        if (it->second.socket_fd != sockfd)
        {
            if (send(it->second.socket_fd, message.c_str(), message.size(), 0) == -1)
                std::cerr << "send() faild" << std::endl;
        }
    }
}

void Server::send_just_member(const std::string &message, std::string channel)
{
    std::vector<Client*> ::iterator it = channels[channel]->_members.begin();
    for (; it != channels[channel]->_members.end(); it++)
    {
        if (send((*it)->socket_fd, message.c_str(), message.size(), 0) == -1)
            std::cerr << "send() faild" << std::endl;
        break;
    }
}

bool Server::isAdmin(request& req, Client& cli)
{
    std::vector<Client*>::iterator it;
    bool etat = false;
    
    for (it = channels[req.arg[1]]->admins.begin(); it !=  channels[req.arg[1]]->admins.end(); ++it)
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
