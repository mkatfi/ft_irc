#include "../../headers/server.hpp"
#include <string>

void Server::send_message(int sockfd, const std::string& message)
{
    send(sockfd, message.c_str(), message.size(), 0);
}

