#include "../../headers/server.hpp"

void Server::bot(Client &client, request &p)
{
	if (client.nickName.empty())
	{
		send_message(client.socket_fd, ERR_NO_CLIENT());
		return;
	}
	srand(time(0));
	std::cout << "fds----->" << client.socket_fd << std::endl;
	// std::cout << "fds_bot----->" << bot_fds[0] << std::endl;
	if (p.arg.empty() && client.bot_authenticated == false)
	{
		std::cout << "HERE----------" << "\n";
		if (bot_fds.empty())
		{
			bot_fds.push_back(client.socket_fd);
			send_message(client.socket_fd, ERR_WELCOME());
			return ;
		}
		for (size_t i = 0; i < bot_fds.size(); i++)
		{
			std::cout << "HERE----------1" << "\n";
			std::cout << "bot_fds------->"<< bot_fds[i] << "\n";
			if (bot_fds[i] == client.socket_fd)
			{
				send_message(client.socket_fd, ERR_BOT());
				client.bot_authenticated = true;
				break;
			}
			else if (i + 1 == bot_fds.size())
			{
				bot_fds.push_back(client.socket_fd);
				send_message(client.socket_fd, ERR_WELCOME());
			}
			std::cout << "HERE----------2" << "\n";
		}

		// for (std::vector<int>::iterator it = bot_fds.begin(); it != bot_fds.end(); it++)
		// {
		// 	std::cout << "HERE----------1" << "\n";
		// 	std::cout << *it << "\n";
		// 	if (*it == client.socket_fd)
		// 	{
		// 		send_message(client.socket_fd, ERR_BOT());
		// 		client.bot_authenticated = true;
		// 		break;
		// 	}
		// 	else if (it + 1 == bot_fds.end())
		// 	{
		// 		bot_fds.push_back(client.socket_fd);
		// 		send_message(client.socket_fd, ERR_WELCOME());
		// 	}
		// 	std::cout << "HERE----------2" << "\n";
		// }
	}
	else if (p.arg.empty() && client.bot_authenticated == true)
	{
		if (client.bot_authenticated == true && client.bot_count > 5)
			send_message(client.socket_fd, ERR_BOT4());
		else if (client.bot_authenticated == true && rand() % 3)
			send_message(client.socket_fd, ERR_BOT3());
		else if (client.bot_authenticated == true && rand() % 2)
			send_message(client.socket_fd, ERR_BOT2());
		client.bot_count++;
	}
	else
	{
		if (rand() % 2)
			send_message(client.socket_fd, BOT_ANSWER());
		else if (rand() % 3)
			send_message(client.socket_fd, BOT_ANSWER1());
		else
			send_message(client.socket_fd, BOT_ANSWER2());
	}
	// else if (p.arg.size() > 1)
	// {
	// 		send_message(client.socket_fd, ERR_QUESTIONS());
	// }
}

