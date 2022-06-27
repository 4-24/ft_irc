#ifndef REPLY_HPP
# define REPLY_HPP

#include <sstream>
# include "Server.hpp"

/*
 *  Error Replies
 */

# define ERR_NOSUCHNICK(nick)						":ircserv 401 " + nick + " :No such nick/channel\n"
# define ERR_NOSUCHSERVER(serv)						":ircserv 402 " + serv + " :No such server\n"
# define ERR_NOSUCHCHANNEL(nick, channel)			":ircserv 403 " + nick + " " + channel + " :No such channel\n"
# define ERR_CANNOTSENDTOCHAN(nick, channel)		":ircserv 404 " + nick + " " + channel + " :Cannot send to channel\n"
# define ERR_NOORIGIN								":ircserv 409 * :No origin specified\n"
# define ERR_NORECIPIENT(nick, command)				":ircserv 411 " + nick + " :No recipient given " + command + "\n"
# define ERR_UNKNOWNCOMMAND							":ircserv 421 * :Command not found\n"
# define ERR_NOTEXTTOSEND(nick)						":ircserv 412 " + nick + " :No text to send\n"
# define ERR_NONICKNAMEGIVEN						":ircserv 431 :No nickname given\n"
# define ERR_ERRONEUSNICKNAME(nick)					":ircserv 432 " + nick + " :Erroneus nickname\n"
# define ERR_NICKNAMEINUSE(nick)					":ircserv 433 " + nick + " :Nickname is already in use\n"
# define ERR_NICKCOLLISION(nick)					":ircserv 436 " + nick + " :Nickname collision KILL\n"
# define ERR_NOTONCHANNEL(nick, channel)			":ircserv 442 " + nick + " " + channel + " :You're not on that channel\n"
# define ERR_NOTREGISTERED							":ircserv 451 * :You have not registered\n"
# define ERR_NEEDMOREPARAMS(nick, command)			":ircserv 461 " + nick + " " + command + " :Not enough parameters\n"
# define ERR_ALREADYREGISTRED(nick)					":ircserv 462 " + nick + " :You may not reregister\n"
# define ERR_PASSWDMISMATCH(nick)					":ircserv 464 " + nick + " :Password incorrect\n"
# define ERR_CHANNELISFULL(nick, channel)			":ircserv 471 " + nick + " " + channel + " :Cannot join channel (+l)\n"
# define ERR_UNKNOWNMODE(nick, flags)				":ircserv 472 " + nick + " " +  flags + " :is unknown mode char to me\n"
# define ERR_NOPRIVILEGES(nick)						":ircserv 481 " + nick + " :Permission Denied- You're not an IRC operator\n"
# define ERR_CHANOPRIVSNEEDED(nick, channel)		":ircserv 482 " + nick + " " + channel + " :You're not channel operator\n"
# define ERR_NOOPERHOST(nick)						":ircserv 491 " + nick + " :No O-lines for your host\n"
# define ERR_UMODEUNKNOWNFLAG(nick, channel)		":ircserv 501 " + nick + " " + channel + " :Unknown MODE flag\n"
# define ERR_USERSDONTMATCH(nick)					":ircserv 502 " + nick + " :Cant change mode for other users\n"
# define ERR_FLOOD(sec)								":ircserv 504 * :flood detected, please wait " + sec + " seconds\n"

/*
 *  Command responses
 */

# define RPL_WELCOME(nick)							":ircserv 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "\n"
# define RPL_UMODEIS(nick, mode)					":ircserv 221 " + nick + " " + mode + "\n"
# define RPL_NONE(message)							":ircserv 300 * :" + message + "\n"
# define RPL_SETTOPIC(channel, topic)				":ircserv 300 * : the topic of " + channel + " is set to " + topic + "\n"
# define RPL_AWAY(nick, message)					":ircserv 301 * " + nick + " :" + message + "\n"
# define RPL_UNAWAY(nick)							":ircserv 305 * " + nick + " :You are no longer marked as being away\n"
# define RPL_NOWAWAY(nick)							":ircserv 306 * " + nick + " :You have been marked as being away\n"
# define RPL_LISTSTART(nick)						":ircserv 321 " + nick + " Channel :Users Name\n"
# define RPL_LIST(nick, channel, numusers, topic)	":ircserv 322 " + nick + " " + channel + " " + numusers + " : " + topic + "\n"
# define RPL_LISTEND(nick)							":ircserv 323 " + nick + " :End of /LIST\n"
# define RPL_NOTOPIC(nick, channel)					":ircserv 331 " + nick + " " + channel + " :No topic is set\n"
# define RPL_TOPIC(nick, channel, topic)			":ircserv 332 "  + nick + " " + channel + " :" + topic + "\n"
# define RPL_NAMREPLY(nick, channel, users)			":ircserv 353 " + nick + " = " + channel + " :" + users + "\n"
# define RPL_ENDOFNAMES(nick, channel)				":ircserv 366 " + nick + " " + channel + " :End of /NAMES list\n"
# define RPL_MOTD(nick, comment) 					":ircserv 372 " + nick + " :- " + comment + "\n"
# define RPL_MOTDSTART(nick)						":ircserv 375 " + nick + " :- IRC Message of the day - \n"
# define RPL_ENDOFMOTD(nick)						":ircserv 376 " + nick + " :End of MOTD command\n"
# define RPL_YOUREOPER(nick)						":ircserv 381 " + nick + " :You are now an IRC operator\n"

#endif
