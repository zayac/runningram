/*
 * File:   Server.cpp
 * Author: necto
 *
 * Created on March 29, 2010, 1:52 AM
 */

#include <errno.h>

#include "Server.h"
#include "Exception.h"
#include "Control.h"
#include "Player_manager.h"

			#include <iostream>

Server::Server (int port):pm (0)
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
		throw Exception ("Could not accept socket.");
	s.set_non_blocking (true);
	clients.push_back (s);
}
//--------------------------------------------------------------------------------------------------
void Server::Add_pack (Socket::package imp)
{
	for (list<Clie_sock>::iterator i = clients.begin(); i != clients.end(); ++i)
		i->undelivered.push_back (imp);
}
//--------------------------------------------------------------------------------------------------
void Server::send (char* data, int size)
{
	static int pid = 0;
	pid++;
	*(data + size++) = 'N';
	*(int*)(data + size) = pid;
	size += sizeof (int);
//	std::cerr <<"sid = " <<pid <<std::endl;
	for (list<Clie_sock>::iterator i = clients.begin(); i != clients.end(); ++i)
		i->Send_if_possible (data, size, pm);
}
//--------------------------------------------------------------------------------------------------
void Server::send_important()
{
	char buffer[Buffer_size];
	for (list<Clie_sock>::iterator i = clients.begin(); i != clients.end(); ++i)
		i->Try_send_undelivered (buffer);
}
//--------------------------------------------------------------------------------------------------
void Server::send_next()
{
	send_important();
	int writed = 0;
	for (iterator i = begin(); i != end(); ++i)
	{
		if ((**i).Important()) Add_pack (*i);
		else
		{
			*(buffer + writed++) = (**i).Id();
			int size = (**i).Export (buffer + writed, Buffer_size - writed);
			GODFORBIDlf(size == -1, "can\'t export to package");
			writed += size;
		}
	}
	send (buffer, writed);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void Server::Clie_sock::Receive_answer (Player_manager* pm)
{
	int code = 0;
	char buffer[Buffer_size];

	int size = recv (buffer, Buffer_size);
	if (-1 == size)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return;
		throw Exception ("Could not get confirmation");
	}
	int offset = 0;
	while (offset < size)
	{
		char pkg_id = *(buffer + offset++);
		if (pkg_id == 'c')//"confirmation" id
		{
			code = *(int*)(buffer + offset);
			offset += sizeof (int);
			continue;
		}
		if (pkg_id == 'e')
		{
			int len = pm->Import_events (buffer + offset, size - offset);
			GODFORBIDlf(len == -1, "events not imported");
			offset += len;
		}
	}
	if (code == 1) --packets_in_net;
}
//--------------------------------------------------------------------------------------------------
bool Server::Clie_sock::Send_if_possible (char* data, int size, Player_manager* pm)
{
	Receive_answer (pm);
	if (packets_in_net < max_packets_in_net)
	{
		if (-1 == send (data, size))
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				return false;
			throw Exception ("Could not send", __LINE__, __FILE__);
		}
		++packets_in_net;
		return true;
	}
	return false;
}
//--------------------------------------------------------------------------------------------------
void Server::Clie_sock::Try_send_undelivered (char* buffer, Player_manager* pm)
{
	if (undelivered.empty ()) return;

	int writed = 0;
	for (vector <package>::iterator j = undelivered.begin (); j != undelivered.end (); ++j)
	{
		int size = j->copy_to (buffer + writed, Buffer_size - writed);
		GODFORBIDlf (-1 == size, "can\'t write important packet")
		writed += size;
	}
	if (Send_if_possible (buffer, writed, pm))
		undelivered.clear (); //all important packages are delivered
}
//--------------------------------------------------------------------------------------------------