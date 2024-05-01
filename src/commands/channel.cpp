#include "../../headers/server.hpp"

Channel::Channel(){}

Channel::Channel(std::string &channelName, Client *cl)
{
	_name = channelName;
	_members.push_back(cl);
	_topic = "No topic";
}
Channel::~Channel(){}
void Channel::update_onlinemembers()
{
	_onlinemembers = _members.size();
}
