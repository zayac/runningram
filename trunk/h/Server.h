/*
 * File:   Server.h
 * Author: necto
 *
 * Created on March 29, 2010, 1:52 AM
 */

#ifndef _SERVER_H
#define	_SERVER_H

#include "Socket.h"
#include <list>

using std::list;

class Server
{public:
	Socket listening;
	list<Socket> clients;

public:
	Server (int port);
	virtual ~Server();

	void accept_one();

	void send (char* data, int size);
	int Get_confirmation();

};

#endif	/* _SERVER_H */

