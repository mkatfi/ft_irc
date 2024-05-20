#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#pragma once

#include <string>
#include "server.hpp"

class   Channel {


    public:
        std::string _name;
        std::string _pass;
        std::string _topic;
        std::string _password;
        
        // int _onlinemembers;
	    bool inviteOnly;
        bool    hasPassword;
        bool changeTopic;
        bool isLimit;
        int maxsize;
        std::string invited;
         std::string member_str;

        std::vector<Client *> _members;
        std::vector<Client *> admins;
        Client* admin;

        Channel();
        Channel(std::string& channelName, Client *cl);
        ~Channel();
        // void    removeMember(const std::string& nickname);
        void update_onlinemembers();
        void set_topic(std::string top);
        std::string get_topic();
};

#endif
