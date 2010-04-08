/*
 * File:   Server.cpp
 * Author: necto
 *
 * Created on March 29, 2010, 1:52 AM
 */

#include "Server.h"
#include "Exception.h"

Server::Server (int port)
{
	if (!listening.create ())
	{
		throw Exception ("Could not create server socket.");
	}

	if (!listening.bind (port))
	{
		throw Exception ("Could not bind to port.");
	}

	if (!listening.listen ())
	{
		throw Exception ("Could not listen to socket.");
	}
}
//--------------------------------------------------------------------------------------------------
Server::~Server () { }
//--------------------------------------------------------------------------------------------------
void Server::accept_one()
{
	Socket s;
	if (!listening.accept (s))
	{
		throw Exception ("Could not accept socket.");
	}
	clients.push_back (s);
}
//--------------------------------------------------------------------------------------------------
void Server::send (char* data, int size)
{
	for (list<Socket>::iterator i = clients.begin(); i != clients.end(); ++i)
		if (!i->send (data, size))
			throw Exception ("Could not send");
}
//--------------------------------------------------------------------------------------------------
int Server::Get_confirmation()
{
	int code = 0;
	clients.begin()->recv ((char*)&code, sizeof(code));
	return code;
}
//--------------------------------------------------------------------------------------------------
