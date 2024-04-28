#include "../../headers/server.hpp"

// std::string Server::join(Client& client, request &p)
// {
//     if (p.arg.empty())
//     {
//         send_message(client.socket_fd, ":localhost 461 " + client.nickName + " JOIN :Not connected to the server\r\n");
//         return "";
//     }

//     if (p.arg.size() < 2)
//     {
//         send_message(client.socket_fd, ":localhost 461 " + client.nickName + " JOIN :Not enough parameters\r\n");
//         return "";
//     }

//     std::string channel_list = p.arg[0];
//     std::string key_list;

//     if (p.arg.size() > 1)
//     {
//         key_list = p.arg[1];
//     }

//     std::vector<std::string> channels;
//     std::vector<std::string> keys;

//     // Splitting the channel and key list
//     size_t pos = 0;
//     std::string delimiter = ",";
//     while ((pos = channel_list.find(delimiter)) != std::string::npos)
//     {
//         std::string channel = channel_list.substr(0, pos);
//         channels.push_back(channel);
//         channel_list.erase(0, pos + delimiter.length());
//     }
//     channels.push_back(channel_list);  // Adding the last channel

//     pos = 0;
//     while ((pos = key_list.find(delimiter)) != std::string::npos)
//     {
//         std::string key = key_list.substr(0, pos);
//         keys.push_back(key);
//         key_list.erase(0, pos + delimiter.length());
//     }
//     keys.push_back(key_list);  // Adding the last key

//     if (channels.size() != keys.size() && !keys.empty())
//     {
//         send_message(client.socket_fd, ":localhost 461 " + client.nickName + " JOIN :Invalid number of keys\r\n");
//         return "";
//     }

//     for (size_t i = 0; i < channels.size(); ++i)
//     {
//         std::string channel = channels[i];
//        std::string key;
//         if (i < keys.size())
//             key = keys[i];
//         else
//             key = "";

//         // Check the validity of the channel name
//         if (channel[0] != '#')
//         {
//             send_message(client.socket_fd, ":localhost 403 " + client.nickName + " " + client._channel + " :Invalid channel name\r\n");
//             continue;
//         }

//         // Check the length of the channel name
//         if (channel.length() > 200)
//         {
//             send_message(client.socket_fd, ":localhost 403 " + client.nickName + " " + client._channel + " :Channel name too long\r\n");
//             continue;
//         }

        // std::map<std::string, Channel*>::iterator it = _channels.find(client._channel);
        // if (it == _channels.end())
        // {
        //     createChannel(channel, fd);
        // }
        // else
        // {
        //     // Check channel invite
        //     if (it->second->get_invite() == "+i")
        //     {
        //         if (!it->second->isMember(client[fd]))
        //         {
        //             send_message(fd, ":localhost 473 " + client[fd]->get_nickname() + " " + channel + " :You are not invited to " + channel + "\r\n");
        //             continue;
        //         }
        //     }
        //     else
        //     {
        //         // Check channel limit
        //         if (it->second->get_limit() != 0 && it->second->get_online_members() >= it->second->get_limit())
        //         {
        //             send_message(fd, ":localhost 471 " + client[fd]->get_nickname() + " " + channel + " :" + channel + " is full\r\n");
        //             continue;
        //         }

        //         // Check channel key
        //         if (!it->second->get_key().empty())
        //         {
        //             if (keys.size() <= i || keys[i] != it->second->get_key())
        //             {
        //                 send_message(fd, ":localhost 475 " + client[fd]->get_nickname() + " " + channel + " :Invalid key\r\nSORRY YOU CAN'T JOIN...THIS CHANNEL IS PROTECTED BY A KEY\r\n");
        //                 continue;
        //             }
        //         }
            // }

        //     it->second->addMember(client[fd]);
        //     it->second->update_online_members();
        //     client[fd]->set_channel(channel);

        //     // Send JOIN message to the client
        //     send_message(fd, ":" + client[fd]->get_nickname() + " JOIN :" + channel + "\r\n");

        //     // Send channel's topic if available
        //     std::string topic = "";  // Fetch topic from server, if available
        //     if (!topic.empty())
        //     {
        //         send_message(fd, ":localhost 332 " + client[fd]->get_nickname() + " " + channel + " :" + topic + "\r\n");
        //     }

        //     // Send list of users joined to the channel
        //     std::vector<std::string> users;  // Fetch list of users from server, if available
        //     send_message(fd, ":localhost 353 " + client[fd]->get_nickname() + " = " + channel + " :" + join(users, ' ') + "\r\n");
        //     send_message(fd, ":localhost 366 " + client[fd]->get_nickname() + " " + channel + " :End of /NAMES list.\r\n");
        // }
//     }

//     return "";
// }


/*___________________channel________*/

// void Server::join(Client& client, request &p)
// {
//     int i = 1;
//     if (p.arg.empty())
//         send_message(client.socket_fd, ":localhost 461 " + client.nickName + " join :Not enough parameters\r\n");
//     while(i < p.arg.size())
//     {
//         if (p.arg[i][0] != '#')
//         {
//             send_message(client.socket_fd, ":localhost 461 " + client.nickName + " join :Not enough parameters\r\n");
//             exit(1);
//         }
//         else if (p.arg.size() > 200)
//         {
//             send_message(client.socket_fd, ":localhost 403 " + client.nickName +  " :Channel name too long\r\n");
//             exit(1);
//         }

//         i++;
//     }
// }

int Server::pass(Client& client, request &p)
{
    if (p.arg.empty())
    {
        send_message(client.socket_fd, ":localhost 461 " + client.nickName + " PASS :Not enough parameters\r\n");
        return(p.statu = 1);
    }

    if (client.authenticated)
    {
        send_message(client.socket_fd, ":localhost 462 " + client.nickName + " :You may not reregister\r\n");
        return(p.statu = 1);
    }

    if (this->password != p.arg[0])  // Assuming _srv->get_password() returns the server's password
    {
        send_message(client.socket_fd, ":localhost 464 " + client.nickName + " :Password incorrect\r\n");
        close(client.socket_fd);
        return(p.statu = 1);
    }

    client.authenticated = true;
    send_message(client.socket_fd, ":localhost 001 " + client.nickName + " :Welcome to the IRC Server\r\n");

    return (0);
}
