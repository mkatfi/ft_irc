#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#pragma once

#include "server.hpp"
#include <string>


class   Channel {


    public:
        std::string _name;
        std::string _pass;
        std::string _topic;
        bool    hasPassword;
        int _onlinemembers;
        bool _t;

        std::vector<Client *> _members;
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
