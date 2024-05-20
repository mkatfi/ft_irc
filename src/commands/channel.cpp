#include "../../headers/server.hpp"

Channel::Channel() { }

Channel::Channel(std::string &channelName, Client *cl)
{
	_name = channelName;
	_pass = "";
	_password = "";
	_members.push_back(cl);
	// admins.push_back(cl);
	_topic = "";
	inviteOnly = false;
	hasPassword = false;
	changeTopic = false;
	isLimit = false;
	maxsize = 0;
	invited = "";
	member_str = "";
}

Channel::~Channel(){}

// void Channel::update_onlinemembers()
// {
// 	_onlinemembers = _members.size();
// }

void Channel::set_topic(std::string top)
{
	this->_topic = top;
}
std::string Channel::get_topic()
{
	return (this->_topic);
}
