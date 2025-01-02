
# ⚡ FT_IRC ⚡
![welcome](https://github.com/user-attachments/assets/e7c3d537-f72d-48c4-83d8-62e451b9e8a3)
Proudly coded by @tmanolis, @QnYosa, and @marineks ❤️

## 💫 Summary
**ft_IRC** is a project designed to replicate the functionality of an IRC server, allowing users to interact in real-time through text messaging. It follows the Internet Relay Chat (IRC) protocol, implementing both the server-side logic and client-side communication.

### 🔮 Definition
"IRC (Internet Relay Chat) is a protocol for real-time text messaging between internet-connected computers created in 1988. It is mainly used for group discussion in chat rooms called 'channels,' although it supports private messages between two users, data transfer, and various server-side and client-side commands."

![irc-shema](https://github.com/user-attachments/assets/9d141f25-9a13-4541-adc3-fd207ac2ecb1)

### 💾 Subject Requirements
1. Code the IRC server in **C++98**.
2. The binary should look like: `./ircserv <port> <password>`.
3. Use the `poll()` function exactly once.
4. Implement client/server communication over TCP/IP (v4 or v6).
5. Mandatory features:
   - Registering a user (nickname, username).
   - Joining a channel, sending, and receiving private messages.
   - All messages sent in a channel must be received by all users in that channel.
   - Implement both normal users and operators with specific commands.
6. Handle signals (CTRL+C, CTRL+D, CTRL+Z) and avoid memory leaks.
7. (Bonus) Implement a bot and file transfer feature.

### 🔆 Commands Implemented:
| Command  | Description |
|----------|-------------|
| **Invite**  | Invites a user to a channel. |
| **Join**    | Joins a given channel with the specified key. |
| **Kick**    | Removes a user from a channel. |
| **Kill**    | Terminates a client’s connection to the server (IRC Operators only). |
| **List**    | Lists all channels and their information. |
| **Mode**    | Sets or removes modes for users or channels. |
| **Motd**    | Displays the "Message of the Day." |
| **Names**   | Shows nicknames of users in a channel. |
| **Nick**    | Changes a client’s nickname. |
| **Notice**  | Sends notices to users or channels (different from PRIVMSG). |
| **Oper**    | Grants IRC operator privileges to a user. |
| **Part**    | Removes a client from a channel. |
| **Pass**    | Sets a connection password. |
| **Ping**    | Checks if the server/client connection is alive. |
| **Privmsg** | Sends a private message to a user or channel. |
| **Quit**    | Terminates a client’s connection to the server. |
| **Topic**   | Changes or views the topic of a channel. |
| **User**    | Specifies a user’s details during the connection. |


![network](https://github.com/user-attachments/assets/99eb751d-e308-4723-93e2-5bffd7613f19)

### 🎁 Some Tips, Graphs, and Other Resources:
- **Helpful Links:**
  - To understand IRC: [Chirc (An IRC subject from a Chicago uni)](http://...)
  - For client/server connections: [Beej's Guide to Network Programming](http://beej.us/guide/bgnet/)
  - Modern IRC Client Protocol: [IRC Protocol Documentation](http://...)

- **Tips:**
  1. Start with a working client/server socket connection.
  2. Implement signal handling (CTRL+C, CTRL+D, CTRL+Z).
  3. Proceed with client registration (NICK, USER, PASS).
  4. Build the server commands (PING, OPER, KILL).
  5. Implement channel operations (JOIN, PART, INVITE, KICK, PRIVMSG, NOTICE).

- **Debugging Tip:**
  - If your code works with irssi but not with nc, remember that irssi handles messages differently by sending everything at once, including CRLF endings. With `nc`, you need to manage message concatenation and ensure that each message is correctly formatted.

- **Dynamic Macros:**
  - Using macros for clean and efficient code:
    ```cpp
    #define user_id(nickname, username) (":" + nickname + "!" + username + "@localhost")
    #define RPL_INVITE(user_id, invited, channel) (user_id + " INVITE " + invited + " #" + channel + "\r\n")
    ```

### 📫 Contact:
Feel free to reach out to @tmanolis, @QnYosa, and @marineks for questions or feedback on the project.

---

Thank you for checking out **ft_IRC**! 🙌
