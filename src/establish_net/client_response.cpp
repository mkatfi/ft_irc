
#include "../../headers/server.hpp"
#include <sys/socket.h>


void Server::commands(request& req, Client& client)
{
    if (req.arg[0] == "PRIVMSG")
    {
        (void)client;
        std::cout << req.arg[0] << std::endl;
        std::cout << req.arg[1] << std::endl;
        std::cout << req.arg[2] << std::endl;
    }
}