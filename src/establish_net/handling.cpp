#include "../../headers/server.hpp"

void Server::send_message(int sockfd, const std::string& message)
{
    send(sockfd, message.c_str(), message.size(), 0);
}

std::string Server::split(std::string const &str)
{
    std::string tokens;
    std::string::size_type start = 0;
    std::string::size_type end = str.find('/', start);
    while (end != std::string::npos)
    {
        tokens = str.substr(start, end - start);
        start = end + 1;
        end = str.find('/', start);
    }
    tokens = str.substr(start);
    return tokens;
}

int  Server::parss_Request(const std::string& command, request& p)
{
    std::istringstream iss(split(command));
    std::string token;
    if (command.find("./") != std::string::npos)
    {
        std::getline(iss, token, ' ');
        p.commands = token;

        while (std::getline(iss, token, ' '))
        {
            p.arg.push_back(token);
        }
        return (0);
    }
    std::getline(iss, token, ' ');
    p.commands = token;
    return 1;
}

void Server::parse_and_process_command(Client& client, const std::string& command)
{
    request p;
    if (parss_Request(command, p) != 1)
    {
        // std::cout << "<<" << p.commands << ">>\n";
        if (p.commands == "PASS" || p.commands == "pass")
        {
            pass(client, p);
        }
        else if (p.commands == "join")
        {
            std::cout << "<<" << p.commands << ">>\n";
            join(client, p);
            std::map<std::string, Channel *>::iterator it = channels.begin();
            while (it != channels.end())
            {
                std::cout << it->first << "<>" << it->second->_name << ">>\n";
                it++;
            }
        }

        else
            std::cout << "this " << p.commands << " is Incorrect\n";
    }
    else
        std::cout << "this " << p.commands << " not command \n";

    //  else if (p.commands == "NICK") {
    //     if (p.arg.size() == 2) {
    //         // Check if nickname is available
    //         bool nickname_available = true;
    //         // for (const auto& cl : clients)
	// 		// {
    //         //     if (cl.nickname == p.arg[1])
	// 		// 	{
    //         //         nickname_available = false;
    //         //         break;
    //         //     }
    //         // }

    //         if (nickname_available) {
    //             client.nickName = p.arg[1];
    //         } else {
    //             send_message(client.socket_fd, ":localhost 433 * " + client.nickName + " :Nickname is already in use\r\n");
    //         }
    //     }
    // }
    //  else if (p.commands == "USER")
    //  {
    //      user(client, p);
    //  }
    // } else if (tokens[0] == "JOIN") {
    //     if (tokens.size() == 2) {
    //         // Add client to the specified channel
    //         client.channels.push_back(tokens[1]);

    //         // Notify all clients in the channel about the new member
    //         for (const auto& cl : clients) {
    //             if (cl.sockfd != client.socket_fd) {
    //                 send_message(cl.sockfd, ":" + client.nickName + " JOIN " + tokens[1] + "\r\n");
    //             }
    //         }
    //     }
    // } else if (tokens[0] == "PRIVMSG") {
    //     if (tokens.size() >= 3) {
    //         std::string receiver = tokens[1];
    //         std::string message = command.substr(command.find(tokens[2]));

    //         // Send the message to the specified receiver or channel
    //         for (const auto& cl : client) {
    //             if (cl.nickname == receiver || std::find(cl.channels.begin(), cl.channels.end(), receiver) != cl.channels.end()) {
    //                 send_message(cl.sockfd, ":" + client.nickName + " PRIVMSG " + receiver + " :" + message + "\r\n");
    //             }
    //         }
    //     }
    // }
}

