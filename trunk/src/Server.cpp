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
	{
		int code = 0;
		if (-1 == i->recv ((char*) & code, sizeof (code)))
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				continue;
			throw Exception ("Could not get confirmation");
		}
		if (code == 1)
			if (!i->send (data, size))
			{
				if (errno == EAGAIN || errno == EWOULDBLOCK)
					return;
				throw Exception ("Could not send");
			}
	}
}
//--------------------------------------------------------------------------------------------------
void Server::send_important()
{
	char buffer[Buffer_size];
	int writed = 0;
	for (list<Clie_sock>::iterator i = clients.begin(); i != clients.end(); ++i)
	{
//		int code = 0;
//		if (-1 == i->recv ((char*) & code, sizeof (code)))
//		{
//			if (errno == EAGAIN || errno == EWOULDBLOCK)
//				continue;
//			throw Exception ("Could not get confirmation");
//		}
//		if (code == 1)
//			if (!i->send (data, size))
//			{
//				if (errno == EAGAIN || errno == EWOULDBLOCK)
//					return;
//				throw Exception ("Could not send");
//			}
		if (i->undelivered.empty ()) continue;
		
		writed = 0;
		for (vector <package>::iterator j = i->undelivered.begin(); j != i->undelivered.end(); ++j)
		{
			int size = j->copy_to (buffer + writed, Buffer_size - writed);
			if (-1 == size) throw Exception ("can\'t write important packet", __LINE__, __FILE__);
			writed += size;
		}
		if (-1 == i->send (buffer, writed))
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				continue;
			else
				throw Exception ("Could not send");
		}
		else
			i->undelivered.clear();//all important packages are delivered
	}
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
			if (size == -1) throw Exception ("can\'t export to package");

			writed += size;
		}
//		if (writed >= Buffer_size)//if package full (it's impossible) quickly look through, and take important
//		{
//			for (;i != end(); ++i)
//				if ((**i).Important ())
//					important.push_back (*i);
//			break;
//		}
	}
	send (buffer, writed);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
Server::package::package (int size_, char* data_)
:UniId<pkg_data>(new pkg_data, 0)
{
	data()->size = size_;
	data()->data = new char[size_];
	if (data()->data == 0) throw Exception ("can\'t allocate memory", __LINE__, __FILE__);
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
	if (0 == data()->size) throw Exception ("can\'t export to buffer", __LINE__, __FILE__);

	data()->data = new char[data()->size];
	if (data()->data == 0) throw Exception ("can\'t allocate memory", __LINE__, __FILE__);

	memcpy (data()->data, buffer, data()->size);
}
//--------------------------------------------------------------------------------------------------
int Server::package::copy_to (char* dst, int max_size)
{
	if (max_size < data()->size) throw Exception ("package is too big for this small buffer");
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
