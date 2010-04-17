/*
 * File:   Server.cpp
 * Author: necto
 *
 * Created on March 29, 2010, 1:52 AM
 */

#include <errno.h>

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
		throw Exception ("Could not accept socket.");
	s.set_non_blocking (true);
	clients.push_back (s);
}
//--------------------------------------------------------------------------------------------------
void Server::Add_pack (package imp)
{
	for (list<Clie_sock>::iterator i = clients.begin(); i != clients.end(); ++i)
		i->undelivered.push_back (imp);
}
//--------------------------------------------------------------------------------------------------
void Server::send (char* data, int size)
{
	for (list<Clie_sock>::iterator i = clients.begin(); i != clients.end(); ++i)
		i->Send_if_possible (data, size);
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
void Server::Clie_sock::Check_confirmation()
{
	int code = 0;
	if (-1 == recv ((char*) & code, sizeof (code)))
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return;
		throw Exception ("Could not get confirmation");
	}
	if (code == 1) --packets_in_net;
}
//--------------------------------------------------------------------------------------------------
bool Server::Clie_sock::Send_if_possible (char* data, int size)
{
	Check_confirmation();
	if (packets_in_net < max_packets_in_net)
	{
		if (-1 == send (data, size))
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				return false;
			throw Exception ("Could not send");
		}
		++packets_in_net;
		return true;
	}
	return false;
}
//--------------------------------------------------------------------------------------------------
void Server::Clie_sock::Try_send_undelivered (char* buffer)
{
	if (undelivered.empty ()) return;

	int writed = 0;
	for (vector <package>::iterator j = undelivered.begin (); j != undelivered.end (); ++j)
	{
		int size = j->copy_to (buffer + writed, Buffer_size - writed);
		GODFORBIDlf (-1 == size, "can\'t write important packet")
		writed += size;
	}
	if (Send_if_possible (buffer, writed))
		undelivered.clear (); //all important packages are delivered
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
Server::package::package (int size_, char* data_)
:UniId<pkg_data>(new pkg_data, 0)
{
	data()->size = size_;
	data()->data = new char[size_];
	GODFORBIDlf(data()->data == 0, "can\'t allocate memory");
	memcpy (data()->data, data_, data()->size);
}
//--------------------------------------------------------------------------------------------------
Server::package::package (Transmitted* from)
:UniId<pkg_data>(new pkg_data, 0)
{
	char buffer[Buffer_size];
	buffer[0] = from->Id ();
	data()->size = from->Export (buffer + 1, Buffer_size - 1) + 1;

	//if size == 0, export returned -1 = error
	GODFORBIDlf(0 == data()->size, "can\'t export to buffer");

	data()->data = new char[data()->size];
	GODFORBIDlf(data()->data == 0, "can\'t allocate memory");

	memcpy (data()->data, buffer, data()->size);
}
//--------------------------------------------------------------------------------------------------
int Server::package::copy_to (char* dst, int max_size)
{
	GODFORBIDlf(max_size < data()->size, "package is too big for this small buffer");
	memcpy (dst, data()->data, data()->size);
	return data()->size;
}
//--------------------------------------------------------------------------------------------------
void Server::package::Delete_data()
{
	if (data()->data) delete [] data()->data;
	delete data();
}
//--------------------------------------------------------------------------------------------------
