#pragma once

#define C_ESTABLISHING_CONNECTION 1
#define C_READ_REQUEST 2
#define C_RESPONSE_TO_REQUEST 3
#define C_CLOSE_CONNECTION 4


// send_message(client.socket_fd, ":irc.server.com 001 nickname :Welcome to the IRC Server, nickname!\r\n");
// 		send_message(client.socket_fd, ":irc.server.com 002 nickname :Your host is irc.server.com, running version IRCd-1.0\r\n");
// 		send_message(client.socket_fd, ":irc.server.com 003 nickname :This server was created on January 1, 2022\r\n");
// 		send_message(client.socket_fd, ":irc.server.com 004 nickname irc.server.com IRCd-1.0 aoOirw abeIklmnoOpqrstv\r\n");

#define RPL_WELCOME(nickname)   (":irc.server.com 001 " + nickname + " :Welcome to the IRC Server, " + nickname + "!\r\n")
#define RPL_YOURHOST(client, servername) (":irc.server.com 002 " + client + " :Your host is " + servername + ", running version IRCd-1.0\r\n")
#define RPL_CREATED(client) (":irc.server.com 003 " + client + " :This server was created on January 1, 2022\r\n")
#define RPL_MYINFO(nickname, servername)    (":irc.server.com 004 " + nickname + servername + " IRCd-1.0 <available user modes> <available channel modes> [<channel modes with a parameter>]\r\n")

#define ERR_PASSWDMISMATCH()    (" ERROR (464) :Password incorrect\r\n")
#define ERR_NEEDMOREPARAMS(cmd) (cmd + " ERROR (461) :Not enough parameters\r\n")
#define ERR_NICKNAMEINUSE(nick) (nick + " Error (443) :Nickname is already in use\r\n")
#define ERR_NONICKNAMEGIVEN() (" ERROR (431) :No nickname given\r\n")
#define ERR_NOSUCHCHANNEL(channel) (channel + " ERROR (403) :No such channel\r\n")
#define ERR_USERNOTINCHANNEL(channel, c) (channel + " "+ c +" ERROR (441) :They aren't on that channel\r\n")
