// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   Authentification.cpp                               :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: hchaguer <hchaguer@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2024/04/25 16:55:12 by hchaguer          #+#    #+#             */
// /*   Updated: 2024/04/26 17:32:29 by hchaguer         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// /*  step 1 : connecting to the server
//     step 2 : authentification the user
//     step 3 : join the channel
// */

#include "../../headers/server.hpp"


int Server::getAuthentified(Client& cli, request& req)
{
    static int count;

    if (req.cmd == "PASS" || req.cmd == "pass")
    {
        count = 1;
        pass(cli, req, &count);
    }
    else if ((req.cmd == "NICK" || req.cmd == "nick") && count == 1)
    {
        count = 2;
        Nick(cli, req, &count);
    }
    else if ((req.cmd == "USER" || req.cmd == "user") && count == 2)
    {
        count = 3;
        user(cli, req, &count);
    }
    else if (req.cmd == "./JOIN" || req.cmd == "./join")
    {
        join(cli, req);
    }
    else if (req.cmd == "./KICK" || req.cmd == "./kick")
    {
        kick(cli, req);
    }
    else if (req.cmd == "./INVITE" || req.cmd == "./invite")
    {
        invite(cli, req);
    }
    else
        std::cout << req.cmd << " not a command" << std::endl;
    std::cout << req.cmd << "\n";
    return (count);
}
