#pragma once

#define C_ESTABLISHING_CONNECTION 1
#define C_READ_REQUEST 2
#define C_RESPONSE_TO_REQUEST 3
#define C_CLOSE_CONNECTION 4


#define RPL_WELCOME(nickname)   (":irc.server.com 001 " + nickname + " :Welcome to the IRC Server, " + nickname + "!\r\n")
#define RPL_YOURHOST(client, servername) (":irc.server.com 002 " + client + " :Your host is " + servername + ", running version IRCd-1.0\r\n")
#define RPL_CREATED(client) (":irc.server.com 003 " + client + " :This server was created on January 1, 2022\r\n")
#define RPL_MYINFO(nickname, servername)    (":irc.server.com 004 " + nickname + servername + " IRCd-1.0 <available user modes> <available channel modes> [<channel modes with a parameter>]\r\n")

#define ERR_PASSWDMISMATCH()  (" ERROR 464 :Password incorrect\r\n")
#define ERR_NEEDMOREPARAMS(cmd) (": 461 " + cmd + " :Not enough parameters\r\n")
#define ERR_NICKNAMEINUSE(nick) (nick + " Error 433 :Nickname is already in use\r\n")
#define ERR_NONICKNAMEGIVEN() (" ERROR 431 :No nickname given\r\n")
#define ERR_NOSUCHNICK(nick) (nick + " 401 :No such nick/channel\r\n")
#define ERR_NOSUCHCHANNEL(channel) (": 403 " + channel + " :No such channel\r\n")
#define RPL_ENDOFWHOIS(nickname) (nickname + " 318 :End of /WHOIS list\r\n")

#define ERR_WELCOME() ("Kamen: I am the masked bot of fate, choose your question wisely\r\n")
#define ERR_BOT() ("Kamen: Hello? Can you specify what do you really want to know ?\r\n")
#define ERR_BOT2() ("Kamen: Hello?? i said choose your QUESTION ?\r\n")
#define ERR_BOT3() ("Kamen: well hit me when you choose instead of wasting my time?\r\n")
#define ERR_BOT4() ("Kamen: ..........\r\n")
#define ERR_QUESTIONS() ("Kamen: Oh come on! just one question at once please ....\r\n")
#define ERR_NO_CLIENT() ("Kamen: Show yourself before asking a question\r\n")
#define BOT_ANSWER() ("Kamen: yes probably\r\n")
#define BOT_ANSWER1() ("Kamen: Definitly yes\r\n")
#define BOT_ANSWER2() ("Kamen: That's a BIG no\r\n")
#define KICKUSER(nick,user,channel, kicked) (":" + nick + "!" + user+ "@localhost KICK " + channel + " " + kicked + " :you have been kicked\r\n")

#define RPL_TOPIC(nick, channel, topic) (":localhost 332 " + nick + " " + channel + " :" + topic + "\r\n")
#define ERR_CHANOPRIVSNEEDED(channel) (":localhost 482 " + channel + " :You're not channel operator\r\n")
#define RPL_JOINMSG(nick, username, channel) (":" + nick + "!" + username + "@localhost JOIN :" + channel + "\r\n")
// :john!john_user@localhost JOIN :#yox  :<nick>!<user>@<host> JOIN :<channel>
#define ERR_INVALIDMODEPARAM(channel) (":localhost 696 " + channel + " :Invalid mode parameter\r\n")
#define ERR_INVALIDKEY(channel) (":localhost 525 " + channel + " :Invalid channel key\r\n")
#define ERR_BADCHANNELKEY(channel) (":475 " + channel + " :Cannot join channel (+k)\r\n")
#define ERR_USERNOTINCHANNEL(nick,channel) (": 441 " + nick + " " + channel + " :You are not in that channel\r\n")
#define ERR_CHANNELISFULL(nick, channel) (": 471 " + nick + " " + channel + " :Channel is full\r\n")
#define RPL_ALREAYREGISTRED(nick, channel) (": 462 " + nick + " " + channel + " :already on that channel\r\n")
#define ERR_INVITEONLYCHAN(nick, channel) (": 473 " + nick + " " + channel + " :Cannot join channel (+i)\r\n")
#define ERR_KICKYOUSELF(nick) (": 488 " + nick + " :You can not kick yourself\r\n")
#define ERR_USERONCHANNEL(nick, channel) (": 443 " + nick + " " + channel + " :is already on channel\r\n")
#define ERR_NOTONCHANNEL(nick, channel) (": 442 " + nick + " " + channel + " :You're not on that channel\r\n")
#define RPL_INVITING(nick, channel,user) (":localhost 341 " + nick + " " + channel + user +" :invite in channel!\r\n")
#define RPL_TOPICWHOTIME(nick, channel, admin) (": 333 " + nick + " " + channel + " " + admin + "\r\n")
#define QUIT(fd, nick) (":" + fd + "!~" + nick + "@localhost QUIT leaving the channel\r\n")

